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
    uint32_t mode = -1;
    uint32_t numberOfUnits = -1;
    uint32_t milliseconds = -1;
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

struct TCPMessage{
    /* Header */
    uint16_t version;
    uint32_t uid;
    uint16_t channelMask;
    uint32_t timestamp;
    uint32_t duration;
    float adcConstant;
    float sensitivity[NUMBER_OF_SENSORS];
    /* Data */
    uint32_t dataSize;
    uint8_t *data;
};
#pragma pack(pop)



#endif //MESSAGEGENERATOR_STRUCTURES_H
