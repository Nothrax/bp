//
// Created by Jakub Trubka on 09.04.20.
//

#ifndef UNTITLED_DELTAENCODE_H
#define UNTITLED_DELTAENCODE_H

#include <string>
#include <fstream>
#include <list>
#include <cmath>
#include <bitset>
#include "Structures.h"

class DeltaEncode {
public:
    void encodeFile(std::string inputFile, std::string outputFile);
    void decodeFile(std::string inputFileName, std::string outputFileName);
private:
    std::ifstream inFile;
    std::ofstream outFile;
    std::list<Difference> differences;
    uint8_t writeBuffer[WRITE_BUFFER_SIZE] = {0};
    int byteIndex = 0;
    int8_t biteIndex = 7;

    uint32_t readValue();
    void writeValuesBuffer();

};


#endif //UNTITLED_DELTAENCODE_H
