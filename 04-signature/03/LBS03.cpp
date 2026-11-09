#include <iostream>
#include <string.h>
#include <iomanip>
#include <sodium.h>

int main(const int argc, const char * const argv[])
{

  #define MESSAGE (const unsigned char *) "test"
  #define MESSAGE_LEN 4

  if (sodium_init() == -1)
    return 1;

  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <message>" << std::endl;
    return 2;
  }
  


unsigned char pk[crypto_sign_PUBLICKEYBYTES];
unsigned char sk[crypto_sign_SECRETKEYBYTES];
crypto_sign_keypair(pk, sk);

unsigned char signed_message[crypto_sign_BYTES + MESSAGE_LEN];
unsigned long long signed_message_len;

crypto_sign(signed_message, &signed_message_len,
            MESSAGE, MESSAGE_LEN, sk);

unsigned char unsigned_message[MESSAGE_LEN];
unsigned long long unsigned_message_len;
if (crypto_sign_open(unsigned_message, &unsigned_message_len,
                     signed_message, signed_message_len, pk) != 0) {
    /* incorrect signature! */
}



  return 0;
}
