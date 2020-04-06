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
#include <netdb.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

//open-ssl communication from https://aticleworld.com/ssl-server-client-using-openssl-in-c/
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
    uint32_t waitTime;
    bool encrypt = true;

    void createTCPMessage();
    void sendTCPMessage();
    SSL_CTX* InitCTX();
    void ShowCerts(SSL* ssl);
};


#endif //MESSAGEGENERATOR_TCP_H
