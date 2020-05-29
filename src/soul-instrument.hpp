#include <iostream>
#include <vector>

#include <pin.H>
#include <syscall_desc.h>

void asm_instrument();
void sys_instrument();
void entry_instrument();
void taint_relocation();

typedef struct __attribute__((packed)) {
  ADDRINT r_offset;
  ADDRINT r_info;
} Elf64_Rel;

typedef struct __attribute__((packed)) {
  ADDRINT r_offset;
  ADDRINT r_info;
  ADDRINT r_addend;
} Elf64_Rela;
