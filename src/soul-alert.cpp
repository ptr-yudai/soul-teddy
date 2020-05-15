#include "soul-alert.hpp"
#include "soul-ink.hpp"

/**
 * 64-bit memory assertion (taint/dft-sink)
 */
ADDRINT PIN_FAST_ANALYSIS_CALL assert_mem(ADDRINT paddr, ADDRINT taddr)
{
  return TAINTED(tagmap_getb(paddr)) && !POINTER(tagmap_getb(paddr));
}

/**
 * ALERT
 */
void PIN_FAST_ANALYSIS_CALL alert(ADDRINT src, ADDRINT dst)
{
  std::cerr << "[!] ALERT: @" << src << " --> @" << dst << std::endl;
}
