#include <iostream>

#include <pin.H>
#include <libdft_api.h>
#include <tagmap.h>

void PIN_FAST_ANALYSIS_CALL alert(ADDRINT, ADDRINT);
ADDRINT PIN_FAST_ANALYSIS_CALL assert_mem_clean_ptr(ADDRINT, ADDRINT);
ADDRINT PIN_FAST_ANALYSIS_CALL assert_mem_clean(ADDRINT, ADDRINT);
ADDRINT PIN_FAST_ANALYSIS_CALL assert_reg_clean_ptr(size_t, ADDRINT);
