#include <iostream>

#include "soul-hook.hpp"

VOID ins_hook(VOID *rip)
{
  std::cout << (ADDRINT)rip << std::endl;
}
