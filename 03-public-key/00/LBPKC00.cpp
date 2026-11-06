#include <iostream>
#include <gmpxx.h>
#include <sodium.h>
#include "dbghelp.h"
#include "certhelp.h"

int main(const int argc, const char * const argv [])
{
  if (sodium_init() == -1){
    std::cerr << "ERROR: smth is wrong with sodium" << std::endl;
    return 1;
  }

  if (argc != 3)
  {
    std::cerr << "Description: "<< argv[0] << "<num1> <num2> " << std::endl;    
    return 2;
  }  

  mpz_class firstNumber(argv[1]); 
  mpz_class secondNumber(argv[2]);
  mpz_class product = firstNumber * secondNumber; // operator * is overloaded can also use mpz_mul

  std::cout << product << std::endl;

  return 0;
}
