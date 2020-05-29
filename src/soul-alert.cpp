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
assert_reg_clean_ptr(unsigned int tid, size_t reg_base, size_t reg_index)
{
  if ((reg_base != GRP_NUM) && (reg_index != GRP_NUM)) {
    /* registers are used for both base and index */
    return (REG_TAINTED(tid, reg_base) | REG_TAINTED(tid, reg_index))
      && !(REG_POINTER(tid, reg_base) | REG_POINTER(tid, reg_index));
    
  } else if (reg_index == GRP_NUM) {
    /* register is used only for base */
    return REG_TAINTED(tid, reg_base) && !REG_POINTER(tid, reg_base);
    
  } else {
    /* register is used only for index */
    return REG_TAINTED(tid, reg_index) && !REG_POINTER(tid, reg_index);
  }
}

/**
 * ALERT
 */
void PIN_FAST_ANALYSIS_CALL alert_x(ADDRINT rip, ADDRINT dst)
{
  std::cerr << std::hex << "[!] ALERT: @" << rip << " --> @" << dst << std::endl;
  std::exit(1);
}
void PIN_FAST_ANALYSIS_CALL alert_rw(ADDRINT rip, ADDRINT addr)
{
  std::cerr << std::hex << "[!] ALERT: @" << rip << " <=> @" << addr << std::endl;
  std::exit(1);
}
