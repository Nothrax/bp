//
// Created by Jakub Trubka on 09.04.20.
//

#include <iostream>
#include <cstring>
#include "DeltaEncode.h"


void DeltaEncode::encodeFile(std::string inputFile, std::string outputFile) {
    uint32_t rawValue = 0;
    uint32_t maxDifferenceValue = 0;
    uint32_t lastValue = 0;
    uint32_t firstValue = 0;
    byteIndex = 0;
    biteIndex = 7;

    inFile.open(inputFile, std::ios::in | std::ios::binary);

    //reading from inFile
    firstValue = readValue();
    if(!inFile){
        return;
    }
    lastValue = firstValue;


    while(true){
        Difference difference;

        rawValue = readValue();
        if(!inFile){
            break;
        }

        if(lastValue > rawValue){
            difference.diff = lastValue - rawValue;
            difference.negative = false;
            differences.push_back(difference);
        }else{
            difference.diff = rawValue - lastValue;
            difference.negative = true;
            differences.push_back(difference);
        }

        lastValue = rawValue;

        if(difference.diff > maxDifferenceValue){
            maxDifferenceValue = difference.diff;
        }
    }

    inFile.close();

    uint32_t comprimatedValueSize = ceil(std::log2(maxDifferenceValue));

    //writing to inFile

    outFile.open(outputFile, std::ios::out | std::ios::binary);

    outFile.write((char *)&comprimatedValueSize, 4);
    outFile.write((char *)&firstValue, 4);

    while(!differences.empty()){
        Difference actuallDiff = differences.front();
        differences.pop_front();


        //writing sign as 1 bit + control
        writeBuffer[byteIndex] += actuallDiff.negative << biteIndex;

        biteIndex--;
        if(biteIndex < 0){
            byteIndex++;
            biteIndex += 8;
        }
        if(byteIndex > WRITE_BUFFER_SIZE){
            writeValuesBuffer();
        }
        //end writing sign

        //write diff
        for(int index = comprimatedValueSize - 1; index >= 0; index--){
            uint32_t new_num = (actuallDiff.diff >> index) & 0x01;

            writeBuffer[byteIndex] += new_num << biteIndex;

            biteIndex--;
            if(biteIndex < 0){
                byteIndex++;
                biteIndex += 8;
            }
            if(byteIndex > WRITE_BUFFER_SIZE){
                writeValuesBuffer();
            }
        }
    }
    //writing last values
    outFile.write((char *)writeBuffer, byteIndex);
}


void DeltaEncode::decodeFile(std::string inputFileName, std::string outputFileName) {
    inFile.open(inputFileName, std::ios::in | std::ios::binary);
    outFile.open(outputFileName, std::ios::out | std::ios::binary);

    uint32_t comprimatedValueSize = readValue();
    uint32_t firstValue = readValue();
    uint32_t lastValue = firstValue;
    int8_t comprimatedValueIndex = comprimatedValueSize - 1;
    bool negative = false;

    outFile.write((char *)&firstValue, sizeof(uint32_t));

    while(true){
        uint8_t buffer;
        inFile.read((char *)&buffer, sizeof(uint8_t));
        if(!inFile){
            break;
        }

        for(int bufferIndex = 7; bufferIndex >= 0; bufferIndex--){
            if(comprimatedValueIndex == comprimatedValueSize -1){
                negative =  (actuallDiff.diff >> index) & 0x01;
            }
        }


    }

    inFile.close();
    outFile.close();
}

uint32_t DeltaEncode::readValue() {
    uint8_t valueBuffer[sizeof(uint32_t)];
    inFile.read((char*)valueBuffer, sizeof(uint32_t));
    uint32_t value = 0;
    value += valueBuffer[3] << 24;
    value += valueBuffer[2] << 16;
    value += valueBuffer[1] << 8;
    value += valueBuffer[0];

    return value;
}

void DeltaEncode::writeValuesBuffer() {
    outFile.write((char *)writeBuffer, WRITE_BUFFER_SIZE);
    memset(writeBuffer, 0, WRITE_BUFFER_SIZE);
    byteIndex = 0;
    biteIndex = 7;
}