//
// Created by Jakub Trubka on 17.02.20.
//

#ifndef AGGREGATION_AGGREGATION_H
#define AGGREGATION_AGGREGATION_H

#define TIME_OFFEST 15

#include "Structures.h"
#include "sharedMemory/SHMAggregationRead.h"
#include "UDPClient.h"
#include <iostream>
#include <list>
#include <Logger/Logger.h>
#include <fstream>

class Aggregation {
public:
    Aggregation(Config config);
    ~Aggregation();
    void init();
    void start();
private:
    Config config;
    int32_t *buffer;
    float *rms[NUMBER_OF_SENSORS];
    uint32_t valuesPerChannel;
    time_t receiveSecond;
   // std::list<DataPoint> pointsToSend[NUMBER_OF_SENSORS];
    std::list<DataPoint> pointsToSend;
    uint32_t lastSavedValue[NUMBER_OF_SENSORS] = {0};
    uint32_t pointsThrown[NUMBER_OF_SENSORS] = {0};
    uint32_t pointNanosecondOffset = 0;
    unsigned long long int offset = 0;

    void calculateRMS();
    void deltaTreshold();
    //void sendPoints();
};


#endif //AGGREGATION_AGGREGATION_H
