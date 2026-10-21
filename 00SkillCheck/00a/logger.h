#pragma once

#include <fstream>

class SecretLogger
{
  private:
    unsigned char random_values[5];
    size_t number_of_random_values = 5;

  protected:
    std::ofstream file_stream;

  public:
    SecretLogger(const char * const log_path);
    ~SecretLogger();

    void LogValue(const unsigned char &value);
};
