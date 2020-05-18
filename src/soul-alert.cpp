#include "soul-alert.hpp"
#include "soul-ink.hpp"

/**
 * 64-bit memory assertion (taint/dft-sink)
 */
ADDRINT PIN_FAST_ANALYSIS_CALL assert_clean_ptr(ADDRINT paddr, ADDRINT taddr)
{
  return TAINTED(tagmap_getb(MEM_ALIGN(paddr)))
    && !POINTER(tagmap_getb(MEM_ALIGN(paddr)));
}

/**
 * 64-bit memory assertion (instruction fetch)
 */
ADDRINT PIN_FAST_ANALYSIS_CALL assert_clean(ADDRINT paddr, ADDRINT taddr)
{
  return TAINTED(tagmap_getb(MEM_ALIGN(paddr)));
}

/**
 * ALERT
 */
void PIN_FAST_ANALYSIS_CALL alert(ADDRINT src, ADDRINT dst)
{
  std::cerr << std::hex << "[!] ALERT: @" << src << " --> @" << dst << std::endl;
}
