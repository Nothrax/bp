//
// Created by Jakub Trubka on 01.08.19.
//

#include "UDPServer.h"
#include "spdlog/spdlog.h"


UDPServer* UDPServer::startUDPServer(int port) {
    return new UDPServer(port);
}

UDPServer::UDPServer(int port) {
    this->port = port;
    initializeUDPServer();
}

void UDPServer::initializeUDPServer() {
    std::shared_ptr<spdlog::logger> logger = spdlog::get("Aggregation_server");
    struct sockaddr_in server_address;
    int optval;

    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
    {
        logger->error("Creating socket failed.");
        exit(EXIT_FAILURE);
    }

    optval = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons((unsigned short)port);

    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
    {
        logger->error("Binding to port {} failed;", this->port);
        exit(EXIT_FAILURE);
    }
    logger->info("UDP server binded to port {}.", this->port);

}

Message* UDPServer::receiveMessage() {
    unsigned char buf[MAX_MESSAGE_SIZE];
    socklen_t clientlen;
    int received;

    clientlen = sizeof(client_address);
    received = recvfrom(server_socket, buf, MAX_MESSAGE_SIZE, 0, (struct sockaddr *) &client_address, &clientlen);

    std::string address(inet_ntoa(client_address.sin_addr));
    int clientPort = ntohs(client_address.sin_port);

    return new Message(buf, received, address, clientPort);
}