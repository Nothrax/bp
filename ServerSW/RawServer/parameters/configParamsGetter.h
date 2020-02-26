#ifndef RAWSERVER_CONFIGPARAMSGETTER_H
#define RAWSERVER_CONFIGPARAMSGETTER_H

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

using namespace std;

struct Config {
    int port;
    bool verbose;
};

void createLogger(bool verbose);

Config parseArguments(int argc, char **argv);

#endif //RAWSERVER_CONFIGPARAMSGETTER_H
