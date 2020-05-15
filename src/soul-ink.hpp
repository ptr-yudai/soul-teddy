/* Granularity is the size of pointer */
#define INK_GRANULARITY 8
#define MEM_ALIGN(addr) ((addr) & ~(INK_GRANULARITY - 1))

/* Definition of INK */
#define INK_CLEAR   0x00
#define INK_TAINT   0x01
#define INK_POINTER 0x02

#define CLEAR(tag)   (tag == INK_CLEAR)
#define TAINTED(tag) ((tag & INK_TAINT) == INK_TAINT)
#define POINTER(tag) ((tag & INK_POINTER) == INK_POINTER)
