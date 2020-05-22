#include <iostream>

#include "soul-hook.hpp"
#include "soul-ink.hpp"
#include "soul-alert.hpp"

std::list<int> open_fds;
REG thread_ctx_ptr;

/**
 * Instrument jmp/call
 */
void dta_instrument_jmp_call(INS ins)
{
  if (INS_IsDirectBranch(ins) || INS_IsDirectCall(ins))
    return; // branch address is fixed (impossible to be tainted)

  if (INS_OperandIsReg(ins, 0))
    return; // we'd already know if it's tainted on the previous `mov`

  if (INS_MemoryReadSize(ins) != QWORD_LEN)
    return; // :thinking_face: path (unreachable)

  /* indirect call/jmp via memory */
  INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)assert_mem_clean_ptr,
                   IARG_FAST_ANALYSIS_CALL,
                   IARG_MEMORYREAD_EA,
                   IARG_END);
  INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)alert_x,
                     IARG_FAST_ANALYSIS_CALL,
                     IARG_INST_PTR,
                     IARG_BRANCH_TARGET_ADDR,
                     IARG_END);
}

/**
 * Instrument ret
 */
void dta_instrument_ret(INS ins)
{
  INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)assert_mem_clean_ptr,
                   IARG_FAST_ANALYSIS_CALL,
                   IARG_MEMORYREAD_EA,
                   IARG_END);
  INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)alert_x,
                     IARG_FAST_ANALYSIS_CALL,
                     IARG_INST_PTR,
                     IARG_BRANCH_TARGET_ADDR,
                     IARG_END);
}

/**
 * Instrument mov
 */
void dta_instrument_mov(INS ins)
{
  if (INS_IsMemoryRead(ins)) {

    if (INS_OperandIsFixedMemop(ins, 1))
      return; // special access

    REG reg_base = INS_OperandMemoryBaseReg(ins, 1);
    REG reg_index = INS_OperandMemoryIndexReg(ins, 1);

    /* read from memory */
    INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)assert_reg_clean_ptr,
                     IARG_FAST_ANALYSIS_CALL,
                     IARG_THREAD_ID,
                     IARG_UINT32, REG_INDX(reg_base),
                     IARG_UINT32, REG_INDX(reg_index),
                     IARG_END);
    INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)alert_rw,
                       IARG_FAST_ANALYSIS_CALL,
                       IARG_INST_PTR,
                       IARG_MEMORYREAD_EA,
                       IARG_END);

  } else if (INS_IsMemoryWrite(ins)) {

    if (INS_OperandIsFixedMemop(ins, 0))
      return; // special access

    REG reg_base = INS_OperandMemoryBaseReg(ins, 0);
    REG reg_index = INS_OperandMemoryBaseReg(ins, 0);

    /* write to memory */
    INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)assert_reg_clean_ptr,
                     IARG_FAST_ANALYSIS_CALL,
                     IARG_THREAD_ID,
                     IARG_UINT32, REG_INDX(reg_base),
                     IARG_UINT32, REG_INDX(reg_index),
                     IARG_END);
    INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)alert_rw,
                       IARG_FAST_ANALYSIS_CALL,
                       IARG_INST_PTR,
                       IARG_MEMORYWRITE_EA,
                       IARG_END);

  }
}

/**
 * Instrument lea
 */
void dta_instrument_lea(INS ins)
{
  if (INS_OperandMemoryBaseReg(ins, 1) != REG_RIP)
    return;

  /* lea XXX, [rip+XXX] */
  // [TODO] support "lea XXX, [rip+XXX*Y+XXX]"
  ADDRINT src = INS_Address(ins) + INS_OperandMemoryDisplacement(ins, 1) + 7;
  tagmap_setb((uintptr_t)MEM_ALIGN(src), INK_POINTER);
}

/**
 * Callback of `mmap` syscall
 */
void post_mmap_hook(unsigned int tid, syscall_ctx_t *ctx)
{
  void *ret;
  if ((ret = (void*)ctx->ret) == MAP_FAILED) // mmap failed
    return;

  tagmap_setb((uintptr_t)ret, INK_POINTER);
}

/**
 * Callback of `munmap` syscall
 */
void post_munmap_hook(unsigned int tid, syscall_ctx_t *ctx)
{
  void *ret;
  if ((ret = (void*)ctx->ret) == (void*)-1) // munmap failed
    return;

  void *addr = (void*)ctx->arg[SYSCALL_ARG0];
  size_t length = (size_t)ctx->arg[SYSCALL_ARG1];

  tagmap_clrn((uintptr_t)addr, length);
}

/**
 * Callback of `read` syscall
 */
void post_read_hook(unsigned int tid, syscall_ctx_t *ctx)
{
  ssize_t ret;
  if ((ret = (ssize_t)ctx->ret) <= 0) // read failed
    return;

  int fd = (int)ctx->arg[SYSCALL_ARG0];

  // [!] trust fd opened by `open`/`openat`/`creat`
  if (std::find(open_fds.begin(), open_fds.end(), fd) != open_fds.end())
    return;

  void *buf = (void*)ctx->arg[SYSCALL_ARG1];

  // taint buffer on success
  for(int i = 0; i < ret; i += INK_GRANULARITY) {
    tagmap_setb(MEM_ALIGN((uintptr_t)buf + i), INK_TAINT);
  }
}

/**
 * Callback of `open`/`openat`/`creat` syscall
 */
void post_open_hook(unsigned int tid, syscall_ctx_t *ctx)
{
  ssize_t ret;
  if ((ret = (ssize_t)ctx->ret) == -1) // open failed
    return;

  open_fds.push_back(ret);
}

/**
 * Callback of `close` syscall
 */
void post_close_hook(unsigned int tid, syscall_ctx_t *ctx)
{
  ssize_t ret;
  if ((ret = (ssize_t)ctx->ret) == -1) // open failed
    return;

  int fd = (int)ctx->arg[SYSCALL_ARG0];
  open_fds.remove(fd);
}

/**
 * Hook for entry point
 */
void PIN_FAST_ANALYSIS_CALL entry_hook(unsigned int tid, ADDRINT rsp)
{
  ADDRINT ptr;
  int i;
  unsigned char c;

  /* RSP is a valid pointer here */
  for(int i = 0; i < 8; i++) {
    tagmap_setb_reg(tid, REG_RSP, i, INK_POINTER);
  }

  /* argc is tainted */
  tagmap_setb(MEM_ALIGN(rsp), INK_TAINT);

  /* check for argv and envp */
  for(i = 0; i < 2; i++) { // for argv and envp
    do {
      ptr = 0;
      rsp += QWORD_LEN;
      if (PIN_SafeCopy(&ptr, (void*)rsp, sizeof(ptr)) != QWORD_LEN) {
        std::cerr << "[-] Cannot get argv/envp" << std::endl;
      } else if (ptr) {
        /* argv[n] is a valid pointer */
        tagmap_setb(MEM_ALIGN(rsp), INK_POINTER);

        /* every character of argv[n] is tainted */
        do {
          if (PIN_SafeCopy(&c, (void*)ptr, 1) != 1) {
            std::cerr << "[-] Cannot extract string of argv/envp" << std::endl;
          } else if (c) {
            tagmap_setb(MEM_ALIGN(ptr), INK_TAINT); // taint!
          }
          ptr++;
        } while(c);
      }
    } while(ptr);
  }
}
