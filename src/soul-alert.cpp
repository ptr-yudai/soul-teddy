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
  tag_t tag_base = tagmap_getb_reg(tid, reg_base, 0);
  tag_t tag_index = tagmap_getb_reg(tid, reg_index, 0);

  if ((reg_index != GRP_NUM) && (reg_base != GRP_NUM)) {
    /* registers are used for both base and index */
    return TAINTED(tag_base | tag_index)
      && !POINTER(tag_base | tag_index);

  } else if (reg_index == GRP_NUM) {
    /* register is used only for base */
    return TAINTED(tag_base) && !POINTER(tag_base);

  } else {
    /* register is used only for index */
    return TAINTED(tag_index) && !POINTER(tag_index);
  }
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
  std::exit(1);
}
