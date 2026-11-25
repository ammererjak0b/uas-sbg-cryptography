#include <iostream>
#include <string>
#include <vector>
#include <gmpxx.h>
#include <sodium.h>
#include "dbghelp.h"
/*
  AUTHOR Jakob
  AI-USAGE Claude Sonnet 5.5
    -[AI-0]: Discussion abt. the generateSessionKey function. I was 
*/


const char * const g_as_text = "\
  f7e1a085d69b3ddecbbcab5c36b857b97994afbbfa3aea82f\
  9574c0b3d0782675159578ebad4594fe67107108180b44916\
  7123e84c281613b7cf09328cc8a6e13c167a8b547c8d28e0a\
  3ae1e2bb3a675916ea37f0bfa213562f1fb627a01243bcca4\
  f1bea8519089a883dfe15ae59f06928b665e807b552564014\
  c3bfecf492a";
const mpz_class g(g_as_text, 16);

const char * const p_as_text = "\
  fd7f53811d75122952df4a9c2eece4e7f611b7523cef4400c\
  31e3f80b6512669455d402251fb593d8d58fabfc5f5ba30f6\
  cb9b556cd7813b801d346ff26660b76b9950a5a49f9fe8047\
  b1022c24fbba9d7feb7c61bf83b57e7c6a8a6150f04fb83f6\
  d3c51ec3023554135a169132f675f3ae2b61d72aeff222031\
  99dd14801c7";
const mpz_class p(p_as_text, 16);

const unsigned char nonce[crypto_aead_aes256gcm_NPUBBYTES] = {0};

bool hexStringToArray(const std::string &hex_string, unsigned char array[], const size_t array_size);
void generatePartialKey(const std::string &secretName, const std::string &publicName);
void generateSessionKey(std::string &otherPartialText, std::string &mySecretText);

int main(const int argc, const char * const argv [])
{
  if (sodium_init() == -1){
    return 1;
  }

  if (argc < 2){
    std::cerr << "Description: " << std::endl; 
    std::cerr << "Usage: " << argv[0] << std::endl; 
    return 2;
  }

  std::string method = argv[1];

  if(method == "ServerGeneratePartialKey"){
    generatePartialKey("x", "S");
  }
  else if(method == "ClientGeneratePartialKey"){
    generatePartialKey("y", "C");
  }
  else if(method == "ServerGenerateSessionKey"){
    std::string otherPartial = argv[2];
    std::string mySecret = argv[3];
    generateSessionKey(otherPartial, mySecret);
  }
  else if(method == "ClientGenerateSessionKey"){
    std::string myPartial = argv[2];
    std::string theirSecret = argv[3];
    generateSessionKey(myPartial, theirSecret);
  }
  else{
    std::cerr << "Description: " << std::endl; 
    std::cerr << "Usage: " << argv[0] << std::endl; 
    return 3;
  }

  
  return 0;
}

bool hexStringToArray(const std::string &hex_string, unsigned char array[], const size_t array_size) // from the assignment
{
  if (hex_string.length() != 2 * array_size) // 2 hex chars = 1 byte
    return false;
  for (size_t i = 0; i < array_size; i++)
  {
    const std::string str_part(hex_string.c_str() + 2 * i, 2); // 2 chars = 1 byte
    try
    {
      const auto byte = std::stoul(str_part, nullptr, 16); // base 16
      array[i] = byte;
    }
    catch (...) 
    {
      return false;
    }
  }
  return true;
}

// pt.1 for client and server 
void generatePartialKey(const std::string &secretName, const std::string &publicName){  
  
  // copied from assignment
  gmp_randstate_t prngState;
  gmp_randinit_default(prngState); 
  gmp_randseed_ui(prngState, time(nullptr));

  mpz_class secret;

  do {
    mpz_urandomm(secret.get_mpz_t(), prngState, p.get_mpz_t()); 
  }
  while(secret == 0);
  
  mpz_class partialKey;
  mpz_powm(partialKey.get_mpz_t(), g.get_mpz_t(), secret.get_mpz_t(), p.get_mpz_t()); // g^secret mod p

  std::cout << secretName << ": " << secret << std::endl;
  std::cout << publicName << ": " << partialKey << std::endl;
}

// his partialKey merged with my secret gives our shared key (and due to arithmetic of multipl. groups) => the same occours for the other way around
// Server: C, x
// Client: S, y
void generateSessionKey(std::string &otherPartialText, std::string &mySecretText){

  mpz_class otherPartial(otherPartialText, 10);
  mpz_class mySecret(mySecretText, 10);
  mpz_class sharedSession;

  mpz_powm(sharedSession.get_mpz_t(), otherPartial.get_mpz_t(), mySecret.get_mpz_t(), p.get_mpz_t()); // otherPartial^mySecret mod p





}
