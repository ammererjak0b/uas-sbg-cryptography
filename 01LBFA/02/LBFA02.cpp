/*
- LB-FA02: 
- AUTHOR: Jakob Ammerer
- SOURCES: Lecture-Slides (introduction_cryptography.pdf), https://www.youtube.com/watch?v=yHXOnCKh4iE
- AI-USAGE: /

# The Goal of this Task: decrypt Veginere-encrypted file "input_en.txt"

- Caeser is basis:
    - A=0, B=1, ... Z=25
    - encrypt: c = (m + k) mod 26
    - decrypt: m = (c - k) mod 26

- Veginere:
    - Caeser with Key of "l" Letters which is repeating
    - c_i = (m_i + k_(i mod l)) mod 26 //info: _ for "an der stelle"
    - keylen = 3 (4th letter uses key letter 1 again so ITS;ITS;ITS;...;ITS;)    

- Additional Info:
    - A-Z decrypt (no comma, space etc..)       
*/

#include <iostream>
#include <fstream>
#include <string>

const int MAX_LETTERS = 26;

bool IsKeyValid(std::string const key); 
bool IsFileStreamValid(std::ifstream& inputFile, std::ofstream& outputFile);
void Decrypt(std::ifstream& inputFile, std::ofstream& outputFile, std::string const key); // decrypt loop also writes to outfile

int main(int argc, char* argv[]) {

    if(argc != 4){
        std::cerr << "ERROR: usage: <inputFile> <outputFile> <key>" << std::endl;
        return 1;
    }

    std::string key = argv[3];
    
    if(!IsKeyValid(key)){
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    std::ofstream outputFile(argv[2]);

    if(!IsFileStreamValid(inputFile, outputFile)){
        return 1;
    }

    Decrypt(inputFile, outputFile, key);

    inputFile.close();
    outputFile.close();
    
    return 0;
}

bool IsKeyValid(std::string const key){
    if(key.empty()){
        std::cerr << "ERROR: key is empty" << std::endl;
        return false;
    }
    for(char keyChar : key){ // auto make string but i need char
        if (keyChar < 'A' || keyChar > 'Z'){
            std::cerr << "ERROR: key has to contain caps letters A-Z" << std::endl;
            return false;
        }        
    }
    return true;
}

bool IsFileStreamValid(std::ifstream& inputFile, std::ofstream& outputFile){
    if(!inputFile.is_open() || !outputFile.is_open()){
        std::cerr << "ERROR: could not open input / output file" << std::endl;
        return false;
    }
    return true;
}

void Decrypt(std::ifstream& inputFile, std::ofstream& outputFile, std::string const key){
    char currentChar = ' ';
    int letterIndex = 0;

    
    while(inputFile.get(currentChar)){ // get keeps spaces and newlines 
        if(currentChar >= 'A' && currentChar <= 'Z'){
            int shift = key[letterIndex % key.length()] - 'A'; // explain via ascii in exam 
            currentChar = (currentChar - 'A' - shift + MAX_LETTERS) % MAX_LETTERS + 'A'; // +26 so no negative modulo
            letterIndex ++;
        }
        outputFile << currentChar; // write decrypeted letters (spaces and stuff stays unchanged)
    }
}