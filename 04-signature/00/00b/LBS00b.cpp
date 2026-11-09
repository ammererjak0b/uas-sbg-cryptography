#include <iostream>
#include <string.h>
#include <iomanip>
#include <sodium.h>

int main(const int argc, const char * const argv[])
{
  if (sodium_init() == -1)
    return 1;

  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <message>" << std::endl;
    return 2;
  }
  const char * const message = argv[1];
  const size_t message_length = strlen(message);
  
  std::cout << "Message: " << message << std::endl;
  std::cout << "Message length: " << message_length << " characters" << std::endl;

  unsigned char hash[crypto_hash_sha512_BYTES];
  
  std::cout << "Hash length: " << 8 * sizeof hash << " bits" << std::endl;
  
  crypto_hash_sha512(hash, reinterpret_cast<const unsigned char*>(message), message_length);
  
  std::cout << "Hash: " << std::hex;
  
  for (size_t i = 0; i < sizeof hash; i++){
    
    std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
  }

  std::cout << std::endl;
  
  return 0;
}
