//
// Created by Jakub Trubka on 01.08.19.
//

#ifndef AGGREGATIONSERVER_UDPSERVER_H
#define AGGREGATIONSERVER_UDPSERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <string.h>

#include "Message.h"
#include "Structures.h"

class UDPServer{
public:
    static UDPServer *startUDPServer(int port);
    Message *receiveMessage();
private:
    explicit UDPServer(int port);
    int server_socket;
    int port;
    struct sockaddr_in client_address;
    bool encryption = true;

    void initializeUDPServer();
};


#endif //AGGREGATIONSERVER_UDPSERVER_H
