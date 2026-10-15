#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <map>
#include <iomanip>

const int ASCII_A = 65;
const int ASCII_Z = 90;
const int KEY_LEN = 3;
const int MAX_LETTERS = 26;

// mit Leon, mit Datenstruktur inspo von Susanne & Alex
// weil map per key ordered und nicht per value
typedef struct LetterProbability{
    char letter;
    float probability;
    int letterCount;
};

int main(int argc, char* argv[]) {
    
    if(argc < 2){
        std::cerr << "ERROR: application needs one argument: path to encrypted file" << std::endl;
        return 1;
    }

   
    auto inputFilePath = argv[1];    
    LetterProbability distribution[3][26];

    std::ifstream encryptedFileStream(inputFilePath);
    
   
    
    char currentChar = ' ';
    int loopCount = 1; // cause modulu 0 make no sense
    int numberOfLetters[3] = {0,0,0}; // is used to calculate propability idea from: Leon
    
    while (encryptedFileStream >> currentChar)
    {
        if(!(currentChar >= 65) && !(currentChar <= 90)){
            continue; // guards against not a CAPS
        }

        // so for 3 cols: 1%3 = 1 , 2%3 = 2, 3%3 = 0
        int col = loopCount%KEY_LEN; 
 
        if(col == 1){
            // basically => A means - 65 and so on
            distribution[0][currentChar - 'A'].letter = currentChar;
            distribution[0][currentChar - 'A'].letterCount++;    
            numberOfLetters[0]++;   
        }   
        else if(col == 2){
            distribution[1][currentChar - 'A'].letter = currentChar;
            distribution[1][currentChar - 'A'].letterCount++;    
            numberOfLetters[1]++; 
        }       
        else if(col == 0){
            distribution[2][currentChar - 'A'].letter = currentChar;
            distribution[2][currentChar - 'A'].letterCount++;    
            numberOfLetters[2]++; 
        }
        loopCount ++;
    }

    for (size_t i = 0; i < KEY_LEN; i++){
        for (size_t j = 0; j < MAX_LETTERS; j++){            
            distribution[i][j].probability = (distribution[i][j].letterCount / numberOfLetters[i]) * 100;
        }
    }

    for (size_t i = 0; i < MAX_LETTERS; i++)
    {
        std::cout << "Letter: " + distribution[0][i].letter << + "\tProbability: " << + distribution[0][i].probability << std::endl; 
        std::cout << "Letter: " + distribution[1][i].letter << + "\tProbability: " << + distribution[1][i].probability << std::endl; 
        std::cout << "Letter: " + distribution[2][i].letter << + "\tProbability: " << + distribution[2][i].probability << std::endl; 
    }
    

    encryptedFileStream.close();
    return 0;
}
