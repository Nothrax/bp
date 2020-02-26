//
// Created by Jakub Trubka on 27.11.19.
//

#ifndef BP_LOGGER_H
#define BP_LOGGER_H


#include <string>
#include <vector>
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

class Logger {
public:
    static void initLogger(std::string logPath, bool verbose);
    static void logInfo(std::string log);
    static void logWarning(std::string log);
    static void logError(std::string log);
};


#endif //BP_LOGGER_H
