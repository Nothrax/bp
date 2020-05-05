//
// Created by Jakub Trubka on 31.03.20.
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
    UDP(Arguments arguments, uint32_t uid);
    void initSocket();
    void sendPoint(uint32_t seconds, uint32_t nanosecond, float value);
private:
    Arguments arguments;
    int clientSocket;
    struct sockaddr_in serverAddr;
    struct hostent *server;
    socklen_t serverlen;
    UDPMessage udpMessage;
    uint8_t data[UDP_BUFFER_SIZE];
    uint32_t dataSize = 0;

    void createUDPMessage(uint32_t seconds, uint32_t nanoseconds, float value);
    void sendUDPMessage();


};


#endif //MESSAGEGENERATOR_UDP_H
