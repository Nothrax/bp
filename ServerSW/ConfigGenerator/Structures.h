//
// Created by root on 02.03.20.
//

#ifndef CONFIGGENERATOR_STRUCTURES_H
#define CONFIGGENERATOR_STRUCTURES_H

#include <string>

struct Arguments {
    std::string uid = "";
    std::string db_user = "";
    std::string db_password = "";
    std::string db_address = "";
    std::string db_database = "";
};

struct UnitInfo{
    std::string uid;            //database
    std::string logPath;
    std::string logConsole;
    std::string dataSource;
    std::string filePath;
    std::string dataReadyPin;
    std::string readingSpeed;   //database
    std::string spiSpeed;
    std::string restartADCPin;
    std::string raw;            //database
    std::string aggregated;     //database
    std::string rawFrequency;   //database
    std::string bufferLength;   //database
    std::string adcConstant;    //database
    std::string udpIp;
    std::string udpPort;
    std::string tcpIp;
    std::string tcpPort;
    std::string delta;          //database
    std::string period;         //database
    std::string meanAverage;           //database
    //todo sensory
};

struct SensorInfo{
    int sensorIndex;
    std::string active;
    std::string outputDisablePin;
    std::string type;
    std::string sensitivity;
};

#endif //CONFIGGENERATOR_STRUCTURES_H
