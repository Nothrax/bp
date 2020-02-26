//
// Created by nothrax on 20.01.20.
//
#include <string>

#ifndef BP_STRUCTURES_H
#define BP_STRUCTURES_H
#define DATA_SOURCE_FILE 0
#define DATA_SOURCE_SPI 1

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define SHM_NUMBER_OF_MEASUREMENTS 1280
#define NUMBER_OF_SENSORS 4

struct Config{
    int32_t frequency;
    uint32_t uid;
    int32_t spiSpeed;
    int32_t dataSource;
    int32_t restartAdcPin;
    int32_t dataReadyPin;
    bool raw;
    bool sdf;
    bool backup;
    bool verbose;
    int32_t rawFrequency;
    int32_t bufferLengthInSeconds;
    std::string logPath;
    std::string backupPath;
    std::string dataFilePath;
    std::string udpIp;
    std::string tcpIp;
    uint32_t udpPort;
    uint32_t tcpPort;
    bool sensorActive[NUMBER_OF_SENSORS];
    int32_t outputDisablePin[NUMBER_OF_SENSORS];
    int32_t sensorType[NUMBER_OF_SENSORS];
    float sensorSensitivity[NUMBER_OF_SENSORS];
    float adcConstant;
    float delta;
    int32_t period;
    int32_t average;
};


#pragma pack(push, 1)
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

#endif //BP_STRUCTURES_H
