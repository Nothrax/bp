//
// Created by Jakub Trubka on 11.04.20.
//

#include "ZipCompression.h"

void ZipCompression::compressFile(std::string inFilename, std::string outFilename) {
    int error = 0;
    zip *archive = zip_open((outFilename).c_str(), ZIP_CREATE, &error);
    zip_source *source = zip_source_file(archive,(inFilename).c_str(),0,0);
    zip_file_add(archive, (inFilename).c_str(),source,  0);
    zip_close(archive);
}
