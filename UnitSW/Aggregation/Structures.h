//
// Created by root on 17.02.20.
//
#include <string>

#ifndef AGGREGATION_STRUCTURES_H
#define AGGREGATION_STRUCTURES_H
#define NUMBER_OF_SENSORS 4
#define DATA_SOURCE_FILE 0
#define DATA_SOURCE_SPI 1

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

struct DataPoint{
    float value;
    uint32_t time;
    uint32_t timeOffset;
    uint8_t sensorNumber;
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

#endif //AGGREGATION_STRUCTURES_H
