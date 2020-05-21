#include <iostream>

#include <pin.H>
#include <libdft_api.h>
#include <tagmap.h>

void PIN_FAST_ANALYSIS_CALL alert_x(ADDRINT, ADDRINT);
void PIN_FAST_ANALYSIS_CALL alert_rw(ADDRINT, ADDRINT);
ADDRINT PIN_FAST_ANALYSIS_CALL assert_mem_clean_ptr(ADDRINT);
ADDRINT PIN_FAST_ANALYSIS_CALL assert_mem_clean(ADDRINT);
ADDRINT PIN_FAST_ANALYSIS_CALL assert_reg_clean_ptr(size_t, ADDRINT);
