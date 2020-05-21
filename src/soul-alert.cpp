#include "soul-alert.hpp"
#include "soul-ink.hpp"

/**
 * 64-bit memory assertion (taint/dft-sink)
 */
ADDRINT PIN_FAST_ANALYSIS_CALL
assert_mem_clean_ptr(ADDRINT paddr)
{
  return TAINTED(tagmap_getb(MEM_ALIGN(paddr)))
    && !POINTER(tagmap_getb(MEM_ALIGN(paddr)));
}

/**
 * 64-bit memory assertion (instruction fetch)
 */
ADDRINT PIN_FAST_ANALYSIS_CALL
assert_mem_clean(ADDRINT paddr)
{
  return TAINTED(tagmap_getb(MEM_ALIGN(paddr)));
}

/**
 * 64-bit register assertion (taint/dft-sink)
 */
ADDRINT PIN_FAST_ANALYSIS_CALL
assert_reg_clean_ptr(size_t reg_id, ADDRINT addr)
{
  // [TODO] check thread info
  return TAINTED(tagmap_getb(MEM_ALIGN(addr)))
    && !POINTER(tagmap_getb(MEM_ALIGN(addr)));
}

/**
 * ALERT
 */
void PIN_FAST_ANALYSIS_CALL alert_x(ADDRINT rip, ADDRINT dst)
{
  std::cerr << std::hex << "[!] ALERT: @" << rip << " --> @" << dst << std::endl;
}
void PIN_FAST_ANALYSIS_CALL alert_rw(ADDRINT rip, ADDRINT addr)
{
  std::cerr << std::hex << "[!] ALERT: @" << rip << " <=> @" << addr << std::endl;
}
