#include <algorithm>
#include <list>
#include <sys/mman.h>

#include <pin.H>
#include <libdft_core.h>
#include <libdft_api.h>
#include <syscall_desc.h>
#include <tagmap.h>
#include <ins_helper.h>

#define QWORD_LEN 8

extern ins_desc_t ins_desc[XED_ICLASS_LAST];
extern syscall_desc_t syscall_desc[SYSCALL_MAX];

void dta_instrument_jmp_call(INS);
void dta_instrument_ret(INS);
void dta_instrument_mov(INS);
void dta_instrument_lea(INS);

void post_mmap_hook(unsigned int, syscall_ctx_t*);
void post_munmap_hook(unsigned int, syscall_ctx_t*);
void post_open_hook(unsigned int, syscall_ctx_t*);
void post_close_hook(unsigned int, syscall_ctx_t*);
void post_read_hook(unsigned int, syscall_ctx_t*);

void PIN_FAST_ANALYSIS_CALL entry_hook(unsigned int, ADDRINT);
