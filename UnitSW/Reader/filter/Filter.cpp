//
// Created by Jakub Trubka on 06.02.20.
//

#include "Filter.h"


Filter::Filter(Config config){
    this->config = config;
}

void Filter::start() {
    SHMRawRead *SHMRead = new SHMRawRead();
    SHMRead->openSharedMemory();

    SHMAggregationWrite *SHMWrite =  new SHMAggregationWrite(config);
    SHMWrite->initializeSharedMemory(config);
    SHMWrite->openSharedMemory();

    TCPClient *client = new TCPClient(config);

    while(true){
        SHMRead->readBuffer((uint8_t *)channelBlocks, 2048000 * config.bufferLengthInSeconds);
        std::time_t dataReceiveTime = std::time(nullptr)-config.bufferLengthInSeconds;
        printf("Buffer received\n");
        if(config.sdf){
            printf("applying filter %lu\n", dataReceiveTime);
            for(int sensorIndex = 0; sensorIndex < NUMBER_OF_SENSORS; sensorIndex++){
                arm_fir_decimate_q31(&firInstance[sensorIndex], &channelBlocks[sensorIndex*128000*config.bufferLengthInSeconds], &outputData[sensorIndex*8000*config.bufferLengthInSeconds], blockSize);
            }
        }

        if(config.raw){
            rawCounter -= config.bufferLengthInSeconds;
            if(rawCounter <= 0){
                rawCounter = config.rawFrequency;
                printf("sending data to server %lu\n", dataReceiveTime);
                client->sendMessage((uint8_t *)channelBlocks, dataReceiveTime);
            }
        }
        if(config.sdf){
            SHMWrite->writeBlock((uint8_t *)outputData);
        }
    }
}


void Filter::initialize() {
    rawCounter = config.rawFrequency;
    blockSize = 128000*config.bufferLengthInSeconds;
    outputData = (q31_t *)malloc(sizeof(q31_t)*32000*config.bufferLengthInSeconds); //32 000 -> frekvence/num_taps*pocet senzoru
    channelBlocks = (q31_t *)malloc(sizeof(q31_t)*512000*config.bufferLengthInSeconds); //512 000 frekvence * pocet senzoru
    for(int index = 0; index < NUMBER_OF_SENSORS; index++){
        pState[index] = (q31_t *)malloc((num_taps + (blockSize - 1)) * sizeof(q31_t));

        if(int i = arm_fir_decimate_init_q31(&firInstance[index], num_taps, M, P_COEFFS, this->pState[index], blockSize) != arm_status::ARM_MATH_SUCCESS){
            Logger::logError("Failed to initialize decimate filter, response code:" + std::to_string(i));
        }
    }
}

Filter::~Filter() {
    for(auto & index : pState){
        free(index);
    }
}