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
}
