//
// Created by Jakub Trubka on 27.11.19.
//

#ifndef BP_CONFIGURATION_H
#define BP_CONFIGURATION_H

#include <string>
#include <iostream>
#include <sstream>
#include "structures.h"
#include "configReader/INIReader.h"

class Configuration {
public:
    static Config loadConfig(std::string configFilePath);
};


#endif //BP_CONFIGURATION_H
