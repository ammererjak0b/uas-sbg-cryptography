#include <cstdlib>
#include <ctime>
#include <climits>
#include <cassert>

#include "logger.h"

SecretLogger::SecretLogger(const char * const log_path) : file_stream(log_path)
{
  srand(time(0)); //Initialize PRNG (note: do not use this if you really need secrecy)
}

SecretLogger::~SecretLogger()
{
  unsigned char sum = 0;
  for (size_t i = 0; i < number_of_random_values; i++) // remove <=
    sum += random_values[i]; //Deliberate wrap-around
  file_stream << "Sum of random values: " << static_cast<unsigned int>(sum) << std::endl;
}

void SecretLogger::LogValue(const unsigned char &value)
{
  const unsigned char random_number = rand() % UCHAR_MAX;
  random_values[number_of_random_values++] = random_number; // reason why it breaks. array is len 5 (0...4) but tries writing on pos 6 which means it writes somewhere in mem. if that is already allocated seg error comes
  const unsigned char masked_value = value + random_number; //Deliberate wrap-around
  file_stream << static_cast<unsigned int>(masked_value) << std::endl;
}
