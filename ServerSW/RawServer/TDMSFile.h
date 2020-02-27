/**
 * @file TDMSFile.h
 *
 * @brief
 *
 * @date 25.7.2019
 * @author Jakub Trubka
 * @contact: trubka.jakub@gmail.com
 *
 */
//http://www.ni.com/product-documentation/5696/en/

#ifndef RAWSERVER_TDMSFILE_H
#define RAWSERVER_TDMSFILE_H


#include <stdint-gcc.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>
#include "Structures.h"
#include "Message.h"
#include "parameters/configParamsGetter.h"

#define TOC_MASK 0x0000000e
#define TDMS_VERSION 0x00001269
using namespace std;



class TDMSFile {
public:
    string saveMessageIntoFile(Message *message);
    ~TDMSFile(){
        closeFile();
    }
private:
    uint64_t dataLen;
    uint64_t rawDataOffset;
    string filename;
    Message *message;
    FILE* tdmsFile;
    ChannelUnit channels[NUMBER_OF_CHANNELS];

    void openFile();
    void closeFile();
    void createFilename();
    void writeLeadIn();
    void writeMetaData();
    void writeRawData();
    void generateChannelDescription();
    void fillChannelInformation();
    void calculateOffsets();
    void writeObject(string path, uint32_t rawDataIndex, uint32_t numberOfProperties);
    void writeProperty(string propertyName, string propertyValue);
};


#endif //RAWSERVER_TDMSFILE_H
