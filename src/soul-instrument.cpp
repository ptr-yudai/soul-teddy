#include "soul-instrument.hpp"
#include "soul-hook.hpp"

/**
 * Instrument some instructions
 */
void asm_instrument()
{
  ins_set_pre(&ins_desc[XED_ICLASS_RET_NEAR], dta_instrument_ret);
}

/**
 * Instrument syscalls
 */
void sys_instrument()
{
  // [TODO] Add more system calls
  syscall_set_post(&syscall_desc[__NR_read], post_read_hook);
}
