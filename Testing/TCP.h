//
// Created by nothrax on 31.03.20.
//

#ifndef MESSAGEGENERATOR_TCP_H
#define MESSAGEGENERATOR_TCP_H

#include "Structures.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

class TCP {
public:
    TCP(Arguments arguments, uint32_t uid, uint32_t numberOfMessages);
    void startSending();
private:
    int sock;
    struct sockaddr_in server;
    Arguments arguments;
    TCPMessage tcpMessage;
    uint32_t uid;
    uint32_t numberOfMessages;

    void createTCPMessage();
    void sendTCPMessage();
};


#endif //MESSAGEGENERATOR_TCP_H
