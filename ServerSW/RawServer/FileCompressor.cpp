/**
 * @file ZipFile.cpp
 *
 * @brief
 *
 * @date 29.7.2019
 * @author Jakub Trubka
 * @contact: trubka.jakub@gmail.com
 *
 */

#include "FileCompressor.h"

void FileCompressor::makeZip(std::string filename) {
    int error = 0;
    zip *archive = zip_open((filename + ".zip").c_str(), ZIP_CREATE, &error);
    zip_source *source = zip_source_file(archive,(filename + ".tdms").c_str(),0,0);
    zip_file_add(archive, (filename + ".tdms").c_str(),source,  0);
    zip_close(archive);
}