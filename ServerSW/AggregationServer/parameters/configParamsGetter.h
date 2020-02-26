

#ifndef AGGREGATIONSERVER_CONFIGPARAMSGETTER_H
#define AGGREGATIONSERVER_CONFIGPARAMSGETTER_H

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "Structures.h"


void createLogger(bool verbose);

int getNumber(const std::string &value, uint16_t &number);

int getNumber(const std::string &value, long &number);

int getNumber(const std::string &value, int &number);

int getNumber(const std::string &value, size_t &number);


int setVariableValue(const std::string &key, const std::string &value, Config &params);

Config parseArguments(int argc, char **argv);

int parseArgs(int argc, char **argv, Config &params);

template<class T>
int getParamsFromConfigFile(T &params, const std::string &configFileName) {// open and map config file
    std::ifstream configFile(configFileName);
    std::shared_ptr<spdlog::logger> logger = spdlog::get("Aggregation_server");

    if (!configFile.is_open()) {
        logger->error("Config file is not opened");
        return EXIT_FAILURE;
    }

    // operations
    std::string line;
    int lineNumber = 0;
    std::string firstWord;
    std::string secondWord;
    std::string extraWord;
    std::map<std::string, std::string> configMap;

    while (configFile.good() && getline(configFile, line)) {
        lineNumber++;
        // miss comments
        if (line[0] == '#') { continue; }

        std::basic_stringstream<char> iss(line);

        // skip blank line
        if (!(iss >> std::ws >> firstWord)) {
            continue;
        }

        if (!(iss >> std::ws >> secondWord)) {
            logger->error("No second word on line {}", lineNumber);
            continue;
        }

        if (iss >> extraWord) {
            if(extraWord[0] == '#'){  }
            else {
                logger->error("Extra word on line {}", lineNumber);
                continue;
            }
        }

        if (setVariableValue(firstWord, secondWord, params)) {
            logger->error("Variable in config is not assignable to any variable on line {}", lineNumber);
            return EXIT_FAILURE;
        }
    }

    configFile.close();
    return 0;
}

#endif //AGGREGATIONSERVER_CONFIGPARAMSGETTER_H
