//
// Created by nothrax on 11.04.20.
//

#ifndef DATACOMPRESS_ZIPCOMPRESSION_H
#define DATACOMPRESS_ZIPCOMPRESSION_H


#include <string>
#include <zip.h>


class ZipCompression {
public:
    void compressFile(std::string inFilename, std::string outFilename);
};


#endif //DATACOMPRESS_ZIPCOMPRESSION_H
