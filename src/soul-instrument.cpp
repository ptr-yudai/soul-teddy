#include "soul-instrument.hpp"
#include "soul-hook.hpp"

/**
 * Instrument some instructions
 */
void asm_instrument()
{
  /* instrument indirect branch */
  ins_set_post(&ins_desc[XED_ICLASS_RET_NEAR],
               dta_instrument_ret);
  ins_set_post(&ins_desc[XED_ICLASS_CALL_NEAR],
               dta_instrument_jmp_call);
  ins_set_post(&ins_desc[XED_ICLASS_JMP],
               dta_instrument_jmp_call);

  /* instrument data transfer */
  ins_set_post(&ins_desc[XED_ICLASS_MOV],
               dta_instrument_mov);

  /* instrument P fountain */
  ins_set_pre(&ins_desc[XED_ICLASS_LEA],
              dta_instrument_lea);
}

/**
 * Instrument syscalls
 * [TODO] Add more system calls
 */
void sys_instrument()
{
  /* instrument T fountain */
  syscall_set_post(&syscall_desc[__NR_open], post_open_hook);
  syscall_set_post(&syscall_desc[__NR_openat], post_open_hook);
  syscall_set_post(&syscall_desc[__NR_creat], post_open_hook);
  syscall_set_post(&syscall_desc[__NR_close], post_close_hook);
  syscall_set_post(&syscall_desc[__NR_read], post_read_hook);

  /* instrument P fountain */
  syscall_set_post(&syscall_desc[__NR_mmap], post_mmap_hook);
  syscall_set_post(&syscall_desc[__NR_munmap], post_munmap_hook);
  syscall_set_post(&syscall_desc[__NR_brk], post_brk_hook);
}

/**
 * Instrument entry point
 */
void __internal_entry_hook(IMG img, void *arg)
{
  if (!IMG_Valid(img))
    return; // not a valid image

  if (!IMG_IsMainExecutable(img))
    return; // not the main executable

  ADDRINT addr_entry = IMG_EntryAddress(img);
  RTN rtn_entry = RTN_FindByAddress(addr_entry);

  if (RTN_Valid(rtn_entry)) {

    /* instrument entry point */
    RTN_Open(rtn_entry);
    RTN_InsertCall(rtn_entry, IPOINT_BEFORE, (AFUNPTR)entry_hook,
                   IARG_FAST_ANALYSIS_CALL,
                   IARG_THREAD_ID,
                   IARG_REG_VALUE, REG_RSP,
                   IARG_END);
    RTN_Close(rtn_entry);

  } else {
    /* invalid elf header? */
    std::cerr << "[-] Cannot hook entry point: 0x"
              << std::hex << addr_entry << std::endl;
  }
}

void entry_instrument()
{
  IMG_AddInstrumentFunction(__internal_entry_hook, NULL);
}
