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
    bool verbose;
    int32_t rawFrequency;
    int32_t bufferLengthInSeconds;
    std::string logPath;
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

uint8_t xorKey[512] = {
        112,186,25,175,213,86,144,236,45,199,206,132,17,116,78,134,
        25,79,131,50,174,226,223,62,84,68,81,16,166,84,86,220,
        40,114,132,50,82,101,19,212,132,20,93,208,55,127,14,144,
        43,105,121,245,143,249,199,23,215,48,112,37,5,27,223,41,
        149,155,106,34,203,41,81,206,215,114,74,83,184,169,175,48,
        67,199,94,106,69,186,36,198,131,140,113,71,40,113,25,207,
        173,184,166,116,137,101,147,241,248,161,233,200,245,92,8,52,
        18,75,242,18,35,143,30,243,232,182,196,217,229,238,114,46,
        254,49,223,59,41,149,138,73,115,122,48,134,89,60,218,117,
        81,242,240,139,175,63,235,105,200,65,3,10,223,57,113,155,
        171,111,153,8,185,23,98,195,12,4,123,58,13,177,102,89,
        210,148,215,157,248,66,42,55,102,99,67,7,1,125,111,72,
        11,221,65,208,215,42,102,252,192,254,78,99,21,46,49,141,
        83,86,162,145,71,47,145,237,89,63,188,124,64,239,193,139,
        236,26,88,43,12,2,8,94,204,254,247,126,69,204,77,242,
        60,177,212,224,209,170,90,120,61,81,58,128,109,36,22,92,
        35,25,234,140,138,79,14,99,3,193,205,119,82,88,135,146,
        179,124,195,176,7,104,20,248,182,178,196,97,142,224,38,16,
        120,226,46,244,192,36,161,109,48,191,156,148,228,32,36,204,
        104,86,187,150,183,47,133,53,26,24,18,249,231,67,193,175,
        85,4,178,174,117,66,245,99,158,175,196,64,166,18,194,148,
        37,248,83,137,69,53,35,116,141,8,4,53,9,241,8,222,
        127,73,74,217,138,73,182,245,14,139,176,77,246,232,7,229,
        152,248,227,236,119,68,217,13,79,164,58,85,202,240,217,17,
        221,181,79,19,191,144,36,77,152,141,87,22,47,162,193,97,
        71,231,244,243,81,27,145,185,112,87,243,0,59,133,184,255,
        21,117,77,117,91,14,46,251,201,129,243,10,124,19,213,85,
        148,32,156,211,63,18,181,210,231,90,133,237,124,181,184,138,
        195,128,72,170,189,241,251,105,23,110,212,123,243,19,214,87,
        24,205,228,249,241,11,195,209,5,40,212,128,29,189,42,249,
        209,71,70,205,244,20,129,32,101,124,136,198,0,142,140,169,
        69,137,171,227,151,152,10,5,163,36,39,64,255,212,37,4
};

#endif //AGGREGATION_STRUCTURES_H
