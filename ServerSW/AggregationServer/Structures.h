//
// Created by Jakub Trubka on 01.08.19.
//

#ifndef AGGREGATIONSERVER_STRUCTURES_H
#define AGGREGATIONSERVER_STRUCTURES_H

#include <string>
#include <bitset>

#define MAX_MESSAGE_SIZE 512

struct Database {
    std::string db_user = "";
    std::string db_password = "";
    std::string db_address = "";
    std::string db_database = "";
};

struct Config{
    std::string configFileName = "config";

    int port;
    bool verbose;

    Database database;
};


//UDP message start
struct SensorData{
    uint32_t offset = 0;
    float value = 0;
};

struct Sensor{
    uint8_t sensorId = 0;
    uint8_t dataType = 0;
    uint16_t dataSize = 0;
    SensorData *data = nullptr;
};

struct UDPMessage{
    uint16_t version = 0;
    uint32_t uid = 0;
    uint32_t timestamp = 0;
    uint16_t numberOfSensors = 0;
    Sensor *sensor = nullptr;
};
//UDP message end





#endif //AGGREGATIONSERVER_STRUCTURES_H
