#include <pin.H>
#include <syscall_desc.h>
#include <tagmap.h>

extern ins_desc_t ins_desc[XED_ICLASS_LAST];
extern syscall_desc_t syscall_desc[SYSCALL_MAX];

void dta_instrument_ret(INS ins);
void post_read_hook(unsigned int, syscall_ctx_t*);
