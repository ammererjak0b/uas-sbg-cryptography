#include <iostream>

#include "logger.h"

void SaveMeasurements(const char * const log_file_path)
{
  SecretLogger logger(log_file_path);
  //Electricity consumption in kWh for every day of the week
  const unsigned char consumptions[] { 4, 4, 3, 4, 3, 6, 7 };
  
  for (const auto &consumption : consumptions){
    logger.LogValue(consumption);
  }
}

int main(const int argc, const char * const argv[])
{
  if (argc != 2)
  {
    std::cerr << "Saves measurements securely to a file." << std::endl;
    std::cerr << "Usage: " << argv[0] << " <log file>" << std::endl;
    return 1;
  }
  
  const auto &log_file_path = argv[1];
  
  SaveMeasurements(log_file_path);
  
  std::cout << "Finished." << std::endl;
  
  return 0;
}
