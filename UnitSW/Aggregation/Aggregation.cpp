//
// Created by root on 17.02.20.
//


#include "Aggregation.h"

void Aggregation::start() {
    auto SHMhandler = new SHMAggregationRead();
    auto udpClient = new UDPClient(config);
    udpClient->init();
    SHMhandler->openSharedMemory();
    while(true){
        pointsToSend.clear();
        SHMhandler->readBuffer((uint8_t *)buffer, 128000*config.bufferLengthInSeconds);
        receiveSecond = time(nullptr) - TIME_OFFEST;
        std::cout << "I read a buffer!\n";
        calculateRMS();
        deltaTreshold();
        std::cout << "RMS calculated!\n";
        udpClient->sendData(pointsToSend);
        std::cout << "Points sended!\n";

/*        for(int i = 0; i < NUMBER_OF_SENSORS; i++){
             std::ofstream sdf, raw;

             sdf.open("/home/pi/rawData/8khzRMS_sensor_" + std::to_string(i) + ".csv");
             for(int j = 0; j < valuesPerChannel/config.average; j++){
                 sdf << rms[i][j] <<"\n";
             }
             sdf.close();

*//*            raw.open("/home/pi/rawData/8khzbeforeRMS_sensor_" + std::to_string(i) + ".csv");
            for(int j = 0; j < valuesPerChannel; j++){
                raw << buffer[j + i*valuesPerChannel] <<"\n";
            }
            raw.close();*//*
            raw.open("/home/pi/rawData/8khzaggregRMS_sensor_" + std::to_string(i) + ".csv");
            int numOfPoints = pointsToSend[i].size();
            for(int j = 0; j < numOfPoints; j++){
                raw << pointsToSend[i].front().value << ","<< pointsToSend[i].front().time*1000000 + pointsToSend[i].front().timeOffset <<"\n";
                pointsToSend[i].pop_front();
            }
            raw.close();
         }
        exit(EXIT_SUCCESS);*/

    }
}

void Aggregation::init() {
    if(128000%config.average != 0){
        Logger::logError("128 000 have to by divisible by delta");
    }
    buffer = (int32_t *)malloc(128000*config.bufferLengthInSeconds);
    valuesPerChannel = 8000*config.bufferLengthInSeconds;
    pointNanosecondOffset = (1000000000)/(8000/config.average);
    for(auto & rm : rms){
        rm = (float *)malloc(sizeof(float)*(valuesPerChannel/config.average));
    }
}

Aggregation::Aggregation(Config config){
    this->config = config;
}

Aggregation::~Aggregation() {
    delete buffer;
    for(auto & rm : rms){
        delete rm;
    }
}

void Aggregation::calculateRMS() {
    int rmsIndex = 0;
    for(int dataIndex = 0; dataIndex < valuesPerChannel; dataIndex += config.average){
        float valueSum[NUMBER_OF_SENSORS] = {0};

        for(int blockOffset = 0; blockOffset < config.average; blockOffset++){
            for(int channelIndex = 0; channelIndex < NUMBER_OF_SENSORS; channelIndex++){
                float valueInG = (config.adcConstant*buffer[dataIndex + blockOffset + channelIndex*valuesPerChannel])*config.sensorSensitivity[channelIndex];
                valueSum[channelIndex] += valueInG*valueInG;
            }
        }
        for(int sensorIndex = 0; sensorIndex < NUMBER_OF_SENSORS; sensorIndex++){
            rms[sensorIndex][rmsIndex] = sqrtf(valueSum[sensorIndex]/config.average);
        }
        rmsIndex++;
    }
}

void Aggregation::deltaTreshold() {

    for(int dataIndex = 0; dataIndex < valuesPerChannel/config.average; dataIndex++){
        for(int sensorIndex = 0; sensorIndex < NUMBER_OF_SENSORS; sensorIndex++){
        	if(config.sensorActive[sensorIndex]){
	            if(fabsf(rms[sensorIndex][dataIndex] - lastSavedValue[sensorIndex]) > config.delta || pointsThrown[sensorIndex] > config.period || dataIndex == 0){
	                DataPoint dataPoint;
	                dataPoint.value = rms[sensorIndex][dataIndex];
	                dataPoint.time = receiveSecond;
	                dataPoint.sensorNumber = sensorIndex;

	                ulong offset = dataIndex*pointNanosecondOffset;
	                dataPoint.time += offset / 1000000000;
	                dataPoint.timeOffset = offset % 1000000000;

	                //pointsToSend[sensorIndex].push_back(dataPoint);
	                pointsToSend.push_back(dataPoint);


	                pointsThrown[sensorIndex] = 0;
	                lastSavedValue[sensorIndex] = dataPoint.value;
	            }else{
	                pointsThrown[sensorIndex]++;
	            }
	        }
        }
    }
}
