/*
- LB-FA02: 
- AUTHOR: Jakob Ammerer
- SOURCES: 
- AI-USAGE: 
*/

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {

    if(argc != 4){
        std::cerr << "ERROR: usage: <inputFile> <outputFile> <key>" << std::endl;
        return 1;
    }

    std::string key = argv[3];

    if(key.empty()){
        std::cerr << "ERROR: key is empty";
    }
}

