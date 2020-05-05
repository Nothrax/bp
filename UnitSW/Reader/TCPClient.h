//
// Created by Jakub Trubka on 13.02.20.
//

#ifndef BP_TCPCLIENT_H
#define BP_TCPCLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <filter/arm_fir_decimate.h>
#include "structures.h"
#include "Logger/Logger.h"

class TCPClient {
public:
    TCPClient(Config config);
    void sendMessage(uint8_t *buffer, uint32_t time);
    ~TCPClient();
private:
    Config config;
    TCPMessage tcpMessage;
};


#endif //BP_TCPCLIENT_H
