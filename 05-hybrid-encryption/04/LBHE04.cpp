#include <iostream>
#include <gmpxx.h>
#include <sodium.h>
#include "dbghelp.h"
#include "certhelp.h"

int main(const int argc, const char * const argv [])
{
  if (sodium_init() == -1)
    return 1;

  if (argc != 1)
  {
    std::cerr << "Description: " << std::endl; //TODO
    std::cerr << "Usage: " << argv[0] << std::endl; //TODO
    return 2;
  }
  //TODO: Functionality
  return 0;
}
