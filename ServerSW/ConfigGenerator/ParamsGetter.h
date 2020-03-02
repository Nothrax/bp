

#ifndef CONFIGGENERATOR_PARAMSGETTER_H
#define CONFIGGENERATOR_PARAMSGETTER_H

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include "Structures.h"
#include "Logger/Logger.h"

int setVariableValue(const std::string &key, const std::string &value, Arguments &config);

Arguments parseArguments(int argc, char **argv);

int getParamsFromConfigFile(Arguments &params);
#endif //CONFIGGENERATOR_PARAMSGETTER_H
