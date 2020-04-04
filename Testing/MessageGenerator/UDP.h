//
// Created by nothrax on 31.03.20.
//

#ifndef MESSAGEGENERATOR_UDP_H
#define MESSAGEGENERATOR_UDP_H

#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <list>
#include "Structures.h"


class UDP {
public:
    UDP(Arguments arguments, uint32_t uid, uint32_t numberOfMessages);
    void startSending();
private:
    Arguments arguments;
    int clientSocket;
    struct sockaddr_in serverAddr;
    struct hostent *server;
    socklen_t serverlen;
    UDPMessage udpMessage;
    uint8_t data[UDP_BUFFER_SIZE];
    uint32_t dataSize = 0;
    uint32_t numberOfMessages;
    bool encrypt = true;

    void initSocket();
    void createUDPMessage();
    void sendUDPMessage();
    void encryptMessage();
};


#endif //MESSAGEGENERATOR_UDP_H
