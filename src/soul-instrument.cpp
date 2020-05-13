#include "soul-instrument.hpp"
#include "soul-hook.hpp"

/**
 * Instrument all instructions
 */
VOID ins_instrument(INS ins, VOID *arg)
{
  INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)ins_hook,
                 IARG_INST_PTR,
                 IARG_END);
}
