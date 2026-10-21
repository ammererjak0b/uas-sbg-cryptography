/*
- LB-FA01: based on codeblocks template "Small cryptographic project"    
- AUTHOR: Jakob Ammerer
- SOURCES: https://en.cppreference.com/cpp/algorithm/sort
- AI-USAGE: [0], line 93
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>

const int KEY_LEN = 3;
const int MAX_LETTERS = 26;

// weil map per key ordered und nicht per value
// came up with datastructure while talking with Leon Angerer. First we tried using a map which is sim. to KV-PAIR in c# but it orders per key not per value so that would be a problem sooner or later
// after we came to conclusion that map wont work, we got the inspo for this struct from Susanne Lastowicka, Alexander Lux, Dominik Zach. 

typedef struct LetterProbability{
    char letter;
    float probability;
    int letterCount;
};


// "does a come before b in the sorted list?"
// custom comparison function for std::sort()
bool IsComingBefore(const LetterProbability a, const LetterProbability b){ 
    if(a.probability == b.probability){
        return a.letter < b.letter; // if the same, alphabetic order 
    }
    return a.probability > b.probability; // else return greate value first
}

int main(int argc, char* argv[]) {
    
    if(argc < 2){
        std::cerr << "ERROR: usage: <inputFile>" << std::endl;
        return 1;
    }

    auto inputFilePath = argv[1];    
    std::ifstream encryptedFileStream(inputFilePath);

    LetterProbability distribution[3][26] = {};
    
    char currentChar = ' ';

    int loopCount = 1; // cause modulu 0 make no sense

    int numberOfLetters[3] = {0,0,0}; // is used to calculate propability idea from: Leon Angerer
    
    while (encryptedFileStream >> currentChar)
    {
        if(currentChar < 'A' || currentChar > 'Z'){
            continue; // guard against not a CAPS alphabtic char. 
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
            distribution[i][j].letter = 'A' + j; // all chars A-Z
            distribution[i][j].probability = 100.0 * distribution[i][j].letterCount / numberOfLetters[i];            
        }

        /*
            distribution[i] start, distribution[i] + MAX_LETTERS end (start + 26) on past start
            passing callback function "i pass function, somebody else calls it later" similar to callbacks in js, its just not called at an event
            its called by sort and many times so it is not ansynchrounus like i thought at first cause we only pass the function. It runs fully snychronous  
            sort has already my data (first and last), that it how it know that is a LetterProbability struct (AI-Usage[0]: CLAUDE SONNET 5.5 Explained how the Sort function knows my LetterProbability Struct) 
        */
        std::sort(distribution[i], distribution[i] + MAX_LETTERS, IsComingBefore); // Source: https://en.cppreference.com/cpp/algorithm/sort inspired by "(2) (constexpr since C++20)" example call
    }

    // output
    for (size_t i = 0; i < MAX_LETTERS; i++)
    {
        std::cout 
            << distribution[0][i].letter << ": " << std::round(distribution[0][i].probability) << "% ; "
            << distribution[1][i].letter << ": " << std::round(distribution[1][i].probability) << "% ; "
            << distribution[2][i].letter << ": " << std::round(distribution[2][i].probability) << "%\n";
    }

    encryptedFileStream.close();
    return 0;
}
