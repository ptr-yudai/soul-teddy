#include <iostream>

#include <pin.H>
#include <libdft_api.h>

#include "soul-instrument.hpp"

/**
 * Hi, I'm Soul-Teddy
 */
int main(int argc, char **argv)
{
  // Initialize PIN and libdft64
  if (PIN_Init(argc, argv)) {
    std::cerr << "PIN_Init: Initialize failure" << std::endl;
    return 1;
  }
  if (libdft_init()) {
    std::cerr << "libdft: Initialize failure" << std::endl;
    return 1;
  }

  // Instrument instructions
  INS_AddInstrumentFunction(ins_instrument, 0);

  // Run target binary
  PIN_StartProgram();

  return 0;
}
