//
// Created by Jakub Trubka  on 11.04.20.
//

#include "DeltaCompression.h"


void DeltaCompression::compressFile(std::string inFilename, std::string outFilename) {
    uint32_t maxDifferenceValue = 0;
    uint32_t lastValue = 0;
    uint32_t firstValue = 0;
    uint32_t rawValue = 0;
    std::list<Difference> differences;
    uint8_t writeBuffer[WRITE_BUFFER_SIZE] = {0};
    int byteIndex = 0;
    int8_t biteIndex = 7;

    inFile.open(inFilename, std::ios::in | std::ios::binary);

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

    outFile.open(outFilename, std::ios::out | std::ios::binary);

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
            outFile.write((char *)writeBuffer, WRITE_BUFFER_SIZE);
            byteIndex = 0;
            biteIndex = 7;
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
                outFile.write((char *)writeBuffer, WRITE_BUFFER_SIZE);
                byteIndex = 0;
                biteIndex = 7;
            }
        }
    }
    outFile.write((char *)writeBuffer, byteIndex+1);
    outFile.close();
}

uint32_t DeltaCompression::readValue() {
    uint8_t buffer[4];
    uint32_t rawValue = 0;

    inFile.read((char *)buffer, 4);
    rawValue = 0;
    rawValue += buffer[3] << 24;
    rawValue += buffer[2] << 16;
    rawValue += buffer[1] << 8;
    rawValue += buffer[0];

    return rawValue;
}