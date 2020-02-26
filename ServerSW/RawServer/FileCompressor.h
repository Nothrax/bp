/**
 * @file ZipFile.h
 *
 * @brief
 *
 * @date 29.7.2019
 * @author Jakub Trubka
 * @contact: jakub.trubka@gmail.com
 *
 */
#ifndef RAWSERVER_FILECOMPRESSOR_H
#define RAWSERVER_FILECOMPRESSOR_H


#include <string>
#include <zip.h>
#include <fstream>

class FileCompressor {
public:
    static void makeZip(std::string filename);
};


#endif //RAWSERVER_FILECOMPRESSOR_H
