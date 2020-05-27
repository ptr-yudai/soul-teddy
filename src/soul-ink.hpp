/* Granularity is the size of pointer */
#define INK_GRANULARITY 8
#define MEM_ALIGN(addr) ((addr) & ~(INK_GRANULARITY - 1))

/* Definition of INK */
#define INK_CLEAR   0x00
#define INK_TAINT   0x01
#define INK_POINTER 0x02

#define CLEAR(tag)   ((tag) == INK_CLEAR)
#define TAINTED(tag) (((tag) & INK_TAINT) == INK_TAINT)
#define POINTER(tag) (((tag) & INK_POINTER) == INK_POINTER)

/* take majority */
/*
static bool REG_TAINTED(int tid, int reg) {
  int i, cnt;
  for(i = 0; i < 8; i++) {
    if (TAINTED(tagmap_getb_reg(tid, reg, i))) cnt++;
    if (cnt >= 4) return true;
  }
  return false;
}
static bool REG_POINTER(int tid, int reg) {
  int i, cnt;
  for(i = 0; i < 8; i++) {
    if (POINTER(tagmap_getb_reg(tid, reg, i))) cnt++;
    if (cnt >= 4) return true;
  }
  return false;
}
*/
