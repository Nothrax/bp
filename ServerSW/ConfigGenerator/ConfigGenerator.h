//
// Created by root on 02.03.20.
//

#ifndef CONFIGGENERATOR_CONFIGGENERATOR_H
#define CONFIGGENERATOR_CONFIGGENERATOR_H

#include "Structures.h"
#include "Connection/Database.h"

class ConfigGenerator {
public:
    ConfigGenerator(Arguments arguments);
    void generateConfig();
    std::string getConfigString();
    void saveConfig(std::string path);
private:
    Arguments arguments;
    UnitInfo unitInfo;
    std::list<SensorInfo> sensorsInfo;
    uint8_t odPins[4] = {25,23,24,4};
};


#endif //CONFIGGENERATOR_CONFIGGENERATOR_H
