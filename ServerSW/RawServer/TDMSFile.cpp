/**
 * @file TDMSFile.cpp
 *
 * @brief
 *
 * @date 25.7.2019
 * @author Jakub Trubka
 * @contact: trubka.jakub@gmail.com
 *
 */

#include <ctime>
#include <iostream>
#include "TDMSFile.h"

void TDMSFile::createFilename() {
    time_t startTime = message->getStartTime();
    //fixme gmtime vs localtime
    struct tm *timeInfo = gmtime(&startTime);
    char dateBuffer[14];
    strftime(dateBuffer, 14, "%y%m%d_%H%M%S", timeInfo);
    //filename is in format: raw_<UID>_<YYMMDD>_<HHMMSS>
    filename = "raw_" + message->getUIDString() + "_";
    filename.append(dateBuffer, 13);
}

string TDMSFile::saveMessageIntoFile(Message *message) {
    this->message = message;

    dataLen = 0;
    rawDataOffset = 0;

    openFile();
    fillChannelInformation();
    generateChannelDescription();
    calculateOffsets();

    writeLeadIn();
    writeMetaData();
    writeRawData();
    closeFile();

    return this->filename;
}

void TDMSFile::writeLeadIn() {
    uint32_t tocMask = TOC_MASK;
    uint32_t versionNumber = TDMS_VERSION;
    //first 4 bytes is TDSm tag
    fwrite("TDSm", sizeof(uint8_t), 4, tdmsFile);
    //4 bytes ToC mask
    fwrite(&tocMask, sizeof(uint32_t), 1, tdmsFile);
    //4 bytes version number
    fwrite(&versionNumber, sizeof(uint32_t), 1, tdmsFile);
    //8 bytes content length without lead in length
    fwrite(&dataLen, sizeof(uint64_t), 1, tdmsFile);
    //8 bytes raw data offset without lead in length
    fwrite(&rawDataOffset, sizeof(uint64_t), 1, tdmsFile);
}

void TDMSFile::writeMetaData() {
    //number of objects - 2 paths + sensors
    uint32_t binValue = 2 + message->getNumberOfChannels();
    fwrite(&binValue, sizeof(uint32_t), 1, tdmsFile);

    //first object is "/" path with two properties
    writeObject("/", 0xffffffff, 2);

    //write object properties
    writeProperty("name", filename);
    writeProperty("version", "f02-1");

    //second object /'raw' with no properties
    writeObject("/'raw'", 0xffffffff, 0);

    //writing channel objects
    bool *channelsPresense = message->getChannels();
    for(int i = 0; i < NUMBER_OF_CHANNELS; i++){
        if(channelsPresense[i]){
            string channelPath = "/'raw'/'" + to_string(i) + "'";
            writeObject(channelPath, 0x00000014, 2);
            writeProperty("unit_string", "RAW");
            writeProperty("description", channels[i].description);
        }
    }
}

void TDMSFile::writeObject(string path, uint32_t rawDataIndex, uint32_t numberOfProperties) {
    //object path length
    uint32_t binValue = path.length();
    fwrite(&binValue, sizeof(uint32_t), 1, tdmsFile);
    //object path name
    fwrite(path.c_str(), sizeof(uint8_t), path.length(), tdmsFile);
    //raw data index -> ff ff ff ff means no raw data
    fwrite(&rawDataIndex, sizeof(uint32_t), 1, tdmsFile);

    //add raw data info
    if(rawDataIndex != 0xffffffff){
        //add raw value data type -> 0x00000003 is 32bit uint
        binValue = 0x00000003;
        fwrite(&binValue, sizeof(uint32_t), 1, tdmsFile);
        //add data dimensions, should be always 1
        binValue = 1;
        fwrite(&binValue, sizeof(uint32_t), 1, tdmsFile);
        //add raw data length of one channel -> (num of bytes/num of channelMask)/num of bytes in one value
        uint64_t longBinvalue;
        switch(message->getProtocolVersion()){
            case 0:
                longBinvalue = (message->getRawDataSize()/message->getNumberOfChannels()) / PROTOCOL_0_VALUE_SIZE;
                break;
            case 1:
                longBinvalue = (message->getRawDataSize()/message->getNumberOfChannels()) / PROTOCOL_1_VALUE_SIZE;
                break;
            default:
                break;
        }
        fwrite(&longBinvalue, sizeof(uint64_t), 1, tdmsFile);

    }
    //number of object properties
    fwrite(&numberOfProperties, sizeof(uint32_t), 1, tdmsFile);
}

void TDMSFile::writeProperty(string propertyName, string propertyValue) {
    //first name length
    uint32_t binValue = propertyName.length();
    fwrite(&binValue, sizeof(uint32_t), 1, tdmsFile);
    //property name
    fwrite(propertyName.c_str(), sizeof(uint8_t), propertyName.length(), tdmsFile);
    //data type of property value -> 0x00000020 is string
    binValue = 0x20;
    fwrite(&binValue, sizeof(uint32_t), 1, tdmsFile);
    //property value len
    binValue = propertyValue.length();
    fwrite(&binValue, sizeof(uint32_t), 1, tdmsFile);
    //property value
    fwrite(propertyValue.c_str(), sizeof(uint8_t), propertyValue.length(), tdmsFile);

}

void TDMSFile::writeRawData() {
    switch(message->getProtocolVersion()){
        case 0:
            for(int i = 0; i < message->getRawDataSize();i+=PROTOCOL_0_VALUE_SIZE){
                uint32_t rawValue = 0;

                rawValue += *(message->getRawData()+i) ;
                rawValue += *(message->getRawData()+i+1) << 8;
                rawValue += *(message->getRawData()+i+2)<< 16;
                int signedValue = rawValue > 8388607 ? (rawValue - 2 * 8388607 - 2) : rawValue;

                fwrite(&signedValue, sizeof(int), 1, tdmsFile);
            }
            break;
        case 1:
            for(int i = 0; i < message->getRawDataSize()/PROTOCOL_1_VALUE_SIZE;i++){
                fwrite(&(((int32_t *)message->getRawData())[i]), sizeof(int), 1, tdmsFile);
            }
            break;
        default:
            cerr << "Unknown protocol number\n";
            break;
    }
}

void TDMSFile::openFile() {
    createFilename();
    tdmsFile = fopen((filename + ".tdms").c_str(), "wb");
    if(tdmsFile == nullptr){
        throw std::runtime_error("Unable to create tdmsFile with name: " + filename + ".tdms");
    }
}

void TDMSFile::closeFile() {
    fclose(tdmsFile);
}

void TDMSFile::generateChannelDescription(){
    bool *channelsPresense = message->getChannels();
    for(int i = 0; i < NUMBER_OF_CHANNELS; i++){
        if(channelsPresense[i]){
            std::ostringstream buff;
            buff<<channels[i].adc;
            channels[i].description = "{\"id\":" + to_string(channels[i].id)
                                    + ",\"sf\":"  + to_string(channels[i].sf)
                                    + ",\"adc\":" + buff.str()
                                    + R"(,"name":")" + channels[i].name
                                    + R"(","parameters":{"sensitivity":)" + to_string(channels[i].sensitivity) + "}}";
            rawDataOffset += channels[i].description.length() + TDMS_SENSOR_HEADER_SIZE;
        }
    }
}

void TDMSFile::fillChannelInformation() {
    bool *channelsPresense = message->getChannels();
    //(collected data/datasize)/timestamp spent collecting
    uint32_t sf = 0;
    switch(message->getProtocolVersion()){
        case 0:
            sf = ((message->getRawDataSize() / PROTOCOL_0_VALUE_SIZE) / message->getNumberOfChannels()) /
                    message->getMeasureDuration();
            break;
        case 1:
            sf = ((message->getRawDataSize() / PROTOCOL_1_VALUE_SIZE) / message->getNumberOfChannels()) /
                    message->getMeasureDuration();
            break;
        default:
            break;
    }

    for(int i = 0; i < NUMBER_OF_CHANNELS; i++){
        if(channelsPresense[i]){
            channels[i].id = i;
            channels[i].name = "sensor_" + to_string(i);
            channels[i].sf = sf;
            channels[i].sensitivity = message->getChannelSensitivity(i);
            channels[i].adc = message->getAdcConstant();
        }
    }
}

void TDMSFile::calculateOffsets() {
    rawDataOffset += TDMS_META_HEADER_SIZE;

    switch(message->getProtocolVersion()){
        case 0:
            dataLen = rawDataOffset + message->getRawDataSize() + message->getRawDataSize() / PROTOCOL_0_VALUE_SIZE;
            break;
        case 1:
            dataLen = rawDataOffset + message->getRawDataSize();// + message->getRawDataSize()/PROTOCOL_1_VALUE_SIZE;
            break;
        default:
            break;
    }

}

