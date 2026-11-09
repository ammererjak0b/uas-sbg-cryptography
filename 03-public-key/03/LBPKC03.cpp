/*
 * Author: Jakob Ammerer 
 * Sources:
    - [0] rdbuf: https://en.cppreference.com/cpp/io/basic_fstream/rdbuf
    - [1] https://gmplib.org/manual/Integer-Import-and-Export#index-mpz_005fimport
    - [2] https://gmplib.org/manual/Integer-Import-and-Export#index-mpz_005fexport
    - [3] https://gmplib.org/manual/Integer-Exponentiation#index-mpz_005fpowm
    - [4] https://gmplib.org/manual/C_002b_002b-Interface-Integers 
    - [5] https://gmplib.org/manual/Miscellaneous-Integer-Functions
    - [6] https://en.cppreference.com/cpp/string/basic_string/resize
 
    * AI-Usage Claude Sonnet 5.5: 
      - [AI-0]: Integer Division rounded Down. It suggested to add +7 so you round up
                E.g: 9 / 8 = 1 which is not true. Fix: (9+7) / 8 = 16 / 8 = 2. Even tho its only nine bit i still need two byte
                And for stuff like (16 + 7) / 8 = 2 (still what i wanted.)

                why 7?: https://stackoverflow.com/questions/2422712/rounding-integer-division-instead-of-truncating

      - [AI-1]: Correct params for mpz_import / mpz_export argument flags (1, 1, 1, 0) needed to correctly convert the raw string of bytes into the GMP in format
*/
#include <iostream>
#include <gmpxx.h>
#include "dbghelp.h"
#include <fstream> // ifstream, ofstream
#include <sstream> // stringstream
#include <string>

int main(const int argc, const char * const argv [])
{
  if (argc != 5) // program + input + output + exponent + N
  {
    std::cerr << "Usage: " << argv[0] << " <inputFile> <outputFile> <e || d> <N>" << std::endl;
    return 2;
  }

  std::string inputPath = argv[1];
  std::string outputPath = argv[2];
  std::ifstream inputFile(inputPath, std::ios::binary); //std::ios:binary makes you get rawbytes


  if(!inputFile){
    std::cerr << "ERROR: Cannot open " << inputPath << std::endl;
    return 3;
  }

  std::stringstream buffer; // whole file in one go
  buffer << inputFile.rdbuf(); // Returns pointer to the underlying raw file device object. [0]
  const auto inputText = buffer.str(); // raw bytes

  const mpz_class exponent(argv[3]);
  const mpz_class N(argv[4]);
  mpz_class message;
  
  mpz_import(message.get_mpz_t(), inputText.size(), 1, 1, 1, 0, inputText.data()); // [1], [AI-1] bytes to numbers

  if (message >= N) // RSA only works for m < N
  {
    std::cerr << "ERROR: input too long for N" << std::endl;
    return 4;
  }

  // RSA (same as in 01)
  mpz_class result;
  mpz_powm(result.get_mpz_t(), message.get_mpz_t(), exponent.get_mpz_t(), N.get_mpz_t()); // result = message^exponent mod N


  // AI-Usage CLAUDE SONNET: [AI-0]
  const size_t tempBitCount = mpz_sizeinbase(result.get_mpz_t(), 2); // gets bit count of the number [5]
  const size_t tempByteCount = (tempBitCount + 7) / 8;
  std::string outputText(tempByteCount, '\0'); // creates string with tempByteCount bytes, and fills each with null

  size_t byteCount = 0; // needed for mpz_export, it will write the real count here

  mpz_export(&outputText[0], &byteCount, 1, 1, 1, 0, result.get_mpz_t()); // same params as import [2], [AI-1] numbers to bytes

  
  outputText.resize(byteCount); // cut to real size (Resizes the string to contain count characters) [6]

  std::ofstream outputFile(outputPath, std::ios::binary); // open outfilestream
  
  if (!outputFile)
  {
    std::cerr << "ERROR: cannot write " << outputPath << std::endl;
    return 5;
  }

  outputFile << outputText; // write to file
  
  inputFile.close();
  outputFile.close();

  return 0;
}
