//
// Created by root on 24.02.20.
//

#ifndef AGGREGATION_UDPCLIENT_H
#define AGGREGATION_UDPCLIENT_H

#include "Structures.h"
#include "Logger/Logger.h"
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <list>

#define UDP_BUFFER_SIZE 512

class UDPClient {
public:
    UDPClient(Config config);
    void init();
    void sendData(std::list<DataPoint> dataPoints);
private:
    Config config;
    int clientSocket;
    struct sockaddr_in serverAddr;
    struct hostent *server;
    socklen_t serverlen;

    UDPMessage udpMessage;
    uint8_t data[UDP_BUFFER_SIZE];
    uint32_t dataSize = 0;

    void createUDPMessage();
    void sendUDPMessage();
};


#endif //AGGREGATION_UDPCLIENT_H
