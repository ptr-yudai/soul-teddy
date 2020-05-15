#include <iostream>

#include "soul-hook.hpp"
#include "soul-ink.hpp"
#include "soul-alert.hpp"

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

  void *buf = (void*)ctx->arg[SYSCALL_ARG1];

  // taint buffer on success
  for(int i = 0; i < ret; i += INK_GRANULARITY) {
    tagmap_setb(MEM_ALIGN((uintptr_t)buf + i), INK_TAINT);
  }
}
