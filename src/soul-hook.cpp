#include <iostream>

#include "soul-hook.hpp"
#include "soul-ink.hpp"
#include "soul-alert.hpp"

std::list<int> open_fds;

/**
 * Instrument alert
 */
void dta_instrument_ret(INS ins)
{
  INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)assert_mem,
                   IARG_FAST_ANALYSIS_CALL,
                   IARG_MEMORYREAD_EA,
                   IARG_BRANCH_TARGET_ADDR,
                   IARG_END);
  INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)alert,
                     IARG_FAST_ANALYSIS_CALL,
                     IARG_INST_PTR,
                     IARG_BRANCH_TARGET_ADDR,
                     IARG_END);
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
