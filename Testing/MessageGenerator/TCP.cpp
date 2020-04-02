//
// Created by nothrax on 31.03.20.
//


#include "TCP.h"

TCP::TCP(Arguments arguments, uint32_t uid, uint32_t numberOfMessages){
    this->arguments = arguments;
    this->uid = uid;
    this->numberOfMessages = numberOfMessages;
}

void TCP::startSending() {
    createTCPMessage();
    for(int messageNumber = 0; messageNumber < numberOfMessages; messageNumber++){
        sendTCPMessage();
        usleep(arguments.milliseconds*1000);
    }
}


void TCP::createTCPMessage() {
    tcpMessage.version = 1;
    tcpMessage.uid = uid;
    tcpMessage.timestamp = time(nullptr);
    tcpMessage.channelMask = 0b1111;
    tcpMessage.duration = TCP_LENGTH;
    tcpMessage.adcConstant = 0.00000286;
    tcpMessage.sensitivity[0] = 98.86;
    tcpMessage.sensitivity[1] = 98.86;
    tcpMessage.sensitivity[2] = 98.86;
    tcpMessage.sensitivity[3] = 98.86;
    tcpMessage.dataSize = sizeof(uint32_t)*128000*TCP_LENGTH*NUMBER_OF_SENSORS;
    tcpMessage.data = (uint8_t *)malloc(sizeof(uint32_t)*128000*TCP_LENGTH*NUMBER_OF_SENSORS);
}

void TCP::sendTCPMessage() {
    sock = -1;
    struct sockaddr_in server;
    long arg;
    fd_set fdset;
    struct timeval tv;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Socket error\n";
        return;
    }

    //Debugprint printf("Socket	OK\n");
    server.sin_addr.s_addr = inet_addr(arguments.ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(arguments.port);

    // Set non-blocking
    if( (arg = fcntl(sock, F_GETFL, NULL)) < 0) {
        std::cerr << "Error fcntl(..., F_GETFL)\n";
        close(sock);
        return;
    }
    arg |= O_NONBLOCK;
    if( fcntl(sock, F_SETFL, arg) < 0) {
        std::cerr << "Error fcntl(..., F_SETFL)\n";
        close(sock);
        return;
    }
    connect(sock, (struct sockaddr*)&server, sizeof(server));

    //socket timeout
    FD_ZERO(&fdset);
    FD_SET(sock, &fdset);
    tv.tv_sec = 2;             //* 10 second timeout *//*
    tv.tv_usec = 0;

    if (select(sock + 1, nullptr, &fdset, nullptr, &tv) == 1)
    {
        int so_error;
        socklen_t len = sizeof so_error;

        getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);

        if (so_error != 0) {
            std::cerr << "TCP socket timeout\n";
            close(sock);
            return;
        }
    }

    if( (arg = fcntl(sock, F_GETFL, NULL)) < 0) {
        std::cerr << "Error fcntl(..., F_GETFL) (%s)\n";
        return;
    }
    arg &= (~O_NONBLOCK);
    if( fcntl(sock, F_SETFL, arg) < 0) {
        std::cerr << "Error fcntl(..., F_SETFL) (%s)\n";
        return;
    }

    write(sock, &tcpMessage, sizeof(TCPMessage) - sizeof(u_int8_t *));
    write(sock, tcpMessage.data, tcpMessage.dataSize);

    tcpMessage.timestamp++;

    // Connection close
    close(sock);
}
