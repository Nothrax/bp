//
// Created by nothrax on 31.03.20.
//

#ifndef MESSAGEGENERATOR_STRUCTURES_H
#define MESSAGEGENERATOR_STRUCTURES_H

#include <cstdint>
#include <string>

#define MODE_UDP 0
#define MODE_TCP 1
#define NUMBER_OF_SENSORS 4
#define UDP_BUFFER_SIZE 512
#define TCP_LENGTH 10

struct Arguments{
    std::string file;
    uint32_t port = -1;
    std::string ip = "";
};

#pragma pack(push, 1)
struct SensorData{
    uint32_t offset = 0;
    float value = 0;
};

struct Sensor{
    uint8_t sensorId = 0;
    uint8_t dataType = 0;
    uint16_t dataSize = 0;
    SensorData data[240];
};

struct UDPMessage{
    uint16_t version = 0;
    uint32_t uid = 0;
    uint32_t timestamp = 0;
    uint16_t numberOfSensors = 0;
    Sensor sensor[NUMBER_OF_SENSORS];
};
#pragma pack(pop)



#endif //MESSAGEGENERATOR_STRUCTURES_H
