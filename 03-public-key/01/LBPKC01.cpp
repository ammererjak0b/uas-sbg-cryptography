#include <iostream>
#include <gmpxx.h>
#include "dbghelp.h"

int main(const int argc, const char * const argv [])
{
  if (argc != 4)
  {
    std::cerr << "Description: RSA encryption or decryption "<< argv[0] << " <m> <e or d> <N>" << std::endl;    
    return 2;
  }  

  mpz_class c;
  const mpz_class m(argv[1]);
  const mpz_class e(argv[2]);
  const mpz_class N(argv[3]);
  
  // Encrypting result is cipher text: 
  mpz_powm(c.get_mpz_t(), m.get_mpz_t(), e.get_mpz_t(), N.get_mpz_t());

  std::cout << c << std::endl;
  
  return 0;
}
