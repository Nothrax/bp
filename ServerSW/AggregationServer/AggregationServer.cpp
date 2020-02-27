//
// Created by Jakub Trubka on 01.08.19.
//


#include "AggregationServer.h"

//todo valgrind a uniky pamety
void AggregationServer::startAggregationServer() {
    while(true){
        waitForMessage();
    }
}

void AggregationServer::waitForMessage(){
    try{
        Message *message = udpServer->receiveMessage();
        threads[threadIndex] = std::thread(InfluxDatabase::transferDataToInflux, message, config.database);
        threadIndex++;
        if(threadIndex == NUM_OF_THREADS){
            for (auto & myThread : threads){
                myThread.join();
            }
            threadIndex = 0;
        }
    }catch(std::exception &e){
        logger->error("Exception raised in message receive or thread {}", e.what());
    }
}

AggregationServer::AggregationServer(Config config, UDPServer *udpServer) {
    this->logger = spdlog::get("Aggregation_server");
    this->config = config;
    this->udpServer = udpServer;
}
AggregationServer::~AggregationServer() {
    delete udpServer;
}




