//
// Created by Jakub Trubka on 21.08.19.
//

#include "TCPServer.h"

TCPServer::TCPServer() {
    logger = spdlog::get("Raw_server");
}

TCPServer::~TCPServer() {
    close(serverSocketNumber);
    close(clientSocketNumber);
}

void TCPServer::startTCPServer(int port) {
    this->port = port;
    initializeServer();
    bindToPort();
}

void TCPServer::bindToPort() {
    logger->info("Binding to port {0}", port);

    if (bind(serverSocketNumber, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        logger->error("Binding failed");
        exit(EXIT_FAILURE);
    }
    if ((listen(serverSocketNumber, 1)) < 0)
    {
        logger->error("Listening failed");
        exit(EXIT_FAILURE);
    }
}

TCPMessage TCPServer::receiveMessage(){
    close(serverSocketNumber);
    char str[INET6_ADDRSTRLEN];

    if(inet_ntop(AF_INET6, &client.sin6_addr, str, sizeof(str))) {
        logger->info("New connection from {0}:{1}", str, ntohs(client.sin6_port));
    }
    int rec;

    rec = recv(clientSocketNumber, &binaryMessage, sizeof(TCPMessage) - sizeof(uint8_t *), MSG_WAITALL);
    if(rec != sizeof(TCPMessage) - sizeof(uint8_t *)){
        logger->error("Message to short (insufficient header length)");
        exit(EXIT_FAILURE);
    }

    logger->info("UID: {0:08x}, timestamp: {1}, duration: {2}, channelMask: {3:04b}, data dataSize: {4}", binaryMessage.uid, binaryMessage.timestamp, binaryMessage.duration, binaryMessage.channelMask, binaryMessage.dataSize);//binaryMessage.stampSize);

    binaryMessage.data = new uint8_t[binaryMessage.dataSize];

    rec = recv(clientSocketNumber, binaryMessage.data, binaryMessage.dataSize, MSG_WAITALL);
    if(rec != binaryMessage.dataSize){
        logger->error("Message to short (insufficient data length of {0}, expected {1})", rec, binaryMessage.dataSize);
        exit(EXIT_FAILURE);
    }

    return binaryMessage;
}

void TCPServer::waitForConnection() {
    close(clientSocketNumber);
    clientSocketNumber = 0;

    while(clientSocketNumber <= 0){
        clientSocketNumber = accept(serverSocketNumber, (struct sockaddr*)&client, &clientLen);
    }
}

void TCPServer::initializeServer() {

    if ((serverSocketNumber = socket(PF_INET6, SOCK_STREAM, 0)) < 0)
    {
        logger->error("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    memset(&server,0,sizeof(server));
    server.sin6_family = AF_INET6;
    server.sin6_addr = in6addr_any;
    server.sin6_port = htons(port);
    clientLen = sizeof(client);
}