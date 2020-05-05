//
// Created by Jakub Trubka on 11.04.20.
//

#ifndef DATACOMPRESS_DELTACOMPRESSION_H
#define DATACOMPRESS_DELTACOMPRESSION_H


#include <string>
#include <fstream>
#include <list>
#include <cmath>
#include <bitset>
#include "Structures.h"
#define WRITE_BUFFER_SIZE 512

class DeltaCompression {
public:
    void compressFile(std::string inFilename, std::string outFilename);
private:
    std::ifstream inFile;
    std::ofstream outFile;

    uint32_t readValue();
};


#endif //DATACOMPRESS_DELTACOMPRESSION_H
