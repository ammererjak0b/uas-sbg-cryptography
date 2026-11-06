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

  if (argc != 1)
  {
    std::cerr << "ERROR: The application "<< argv[0] << " does not take any arguments. " << std::endl;    
    return 2;
  }  

  gmp_randstate_t randomState;
  gmp_randinit_default(randomState);
  gmp_randseed_ui(randomState, 110ul); // takes seed as unsigned long 

  const size_t keyBits = 2048;

  mpz_class p;
  mpz_class q;
  mpz_class N;
  mpz_class phi;
  mpz_class e;
  mpz_class d;
  mpz_class gcd;

  while(mpz_sizeinbase(N.get_mpz_t(), 2) != keyBits || (p == q)){ // base 2 => bin

    mpz_urandomb(p.get_mpz_t(), randomState, keyBits / 2); // [AI-0]: BitLength how many binary digits a number has, multiplying two numbers (p*q) roughly adds their length so p.Len = 1024, q.Len = 1024  
    mpz_nextprime(p.get_mpz_t(), p.get_mpz_t()); // next prime nr above

    mpz_urandomb(q.get_mpz_t(), randomState, keyBits / 2);
    mpz_nextprime(q.get_mpz_t(), q.get_mpz_t());

    N = (p * q);
  }
  
  phi = (p - 1) * (q - 1); 
  e = 2; // inbetween 1 and N
  
  
  while(gcd != 1){ // counts up until e and phi have no common divisior anymore   55
    e++;
    mpz_gcd(gcd.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());    
  }

  mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());

  std::cout << "Public key: (" << e << ", " << N << ")" << std::endl;
  std::cout << "Private key: (" << d << ", " << N << ")" << std::endl;

  gmp_randclear(randomState); // free generator memory

  return 0;
}
