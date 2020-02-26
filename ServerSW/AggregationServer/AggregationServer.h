//
// Created by Jakub Trubka on 01.08.19.
//

#ifndef AGGREGATIONSERVER_AGGREGATIONSERVER_H
#define AGGREGATIONSERVER_AGGREGATIONSERVER_H

#include <string>
#include <cstdlib>
#include <csignal>
#include <unistd.h>
#include <spdlog/logger.h>
#include <thread>

#include "Structures.h"
#include "communication/UDPServer.h"
#include "Message.h"
#include "spdlog/spdlog.h"
#include "communication/InfluxDatabase.h"

#define NUM_OF_THREADS 220

class AggregationServer {
public:
    AggregationServer(Config config, UDPServer *udpServer);
    void startAggregationServer();
    ~AggregationServer();
private:
    UDPServer *udpServer;
    Config config;
    std::shared_ptr<spdlog::logger> logger;
    void waitForMessage();
    std::thread myThreads[NUM_OF_THREADS];
    int threadIndex = 0;
};


#endif //AGGREGATIONSERVER_AGGREGATIONSERVER_H
