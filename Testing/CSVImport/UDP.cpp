//
// Created by nothrax on 31.03.20.
//

#include <strings.h>
#include <iostream>
#include <cstring>
#include "UDP.h"


UDP::UDP(Arguments arguments, uint32_t uid){
    this->arguments = arguments;
    udpMessage.uid = uid;
}

void UDP::sendPoint(uint32_t seconds, uint32_t nanosecond, float value){
    createUDPMessage(seconds, nanosecond, value);
    sendUDPMessage();
}

void UDP::initSocket() {
    if ((server = gethostbyname(arguments.ip.c_str())) == nullptr) {
        std::cerr << "ERROR: no such host as " << arguments.ip;
        exit(EXIT_FAILURE);
    }

    /* 3. nalezeni IP adresy serveru a inicializace struktury server_address */
    bzero((char *) &serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serverAddr.sin_addr.s_addr, server->h_length);
    serverAddr.sin_port = htons(arguments.port);
    serverlen = sizeof(serverAddr);

    /* Vytvoreni soketu */
    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) <= 0) {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }
}

void UDP::createUDPMessage(uint32_t seconds, uint32_t nanoseconds, float value) {
    udpMessage.version = 1;
    udpMessage.numberOfSensors = 1;
    dataSize = 0;
    udpMessage.sensor[0].dataSize = 0;


    udpMessage.sensor[0].dataType = 1;
    udpMessage.sensor[0].sensorId = 0;
    udpMessage.timestamp = seconds;

    udpMessage.sensor[0].data[0].value = value;
    udpMessage.sensor[0].data[0].offset = nanoseconds;
    udpMessage.sensor[0].dataSize++;



    int offset = 0;
    memcpy(&data[offset], &udpMessage.version, sizeof(udpMessage.version));
    offset += sizeof(udpMessage.version);

    memcpy(&data[offset], &udpMessage.uid, sizeof(udpMessage.uid));
    offset += sizeof(udpMessage.uid);

    memcpy(&data[offset], &udpMessage.timestamp, sizeof(udpMessage.timestamp));
    offset += sizeof(udpMessage.timestamp);

    memcpy(&data[offset], &udpMessage.numberOfSensors, sizeof(udpMessage.numberOfSensors));
    offset += sizeof(udpMessage.numberOfSensors);

    for(int sensorIndex = 0; sensorIndex < udpMessage.numberOfSensors; sensorIndex++){
        memcpy(&data[offset], &udpMessage.sensor[sensorIndex].sensorId, sizeof(udpMessage.sensor[sensorIndex].sensorId));
        offset += sizeof(udpMessage.sensor[sensorIndex].sensorId);

        memcpy(&data[offset], &udpMessage.sensor[sensorIndex].dataType, sizeof(udpMessage.sensor[sensorIndex].dataType));
        offset += sizeof(udpMessage.sensor[sensorIndex].dataType);

        memcpy(&data[offset], &udpMessage.sensor[sensorIndex].dataSize, sizeof(udpMessage.sensor[sensorIndex].dataSize));
        offset += sizeof(udpMessage.sensor[sensorIndex].dataSize);

        for(int j = 0; j < udpMessage.sensor[sensorIndex].dataSize; j++){
            memcpy(&data[offset], &udpMessage.sensor[sensorIndex].data[j].offset, sizeof(udpMessage.sensor[sensorIndex].data[j].offset));
            offset += sizeof(udpMessage.sensor[sensorIndex].data[j].offset);

            memcpy(&data[offset], &udpMessage.sensor[sensorIndex].data[j].value, sizeof(udpMessage.sensor[sensorIndex].data[j].value));
            offset += sizeof(udpMessage.sensor[sensorIndex].data[j].value);
            //std::cout << udpMessage.sensor[sensorIndex].data[j].offset << " " << udpMessage.sensor[sensorIndex].data[j].value << std::endl;
        }
    }
    dataSize = offset;
}

void UDP::sendUDPMessage() {
    memcpy(&data[6], &udpMessage.timestamp, sizeof(udpMessage.timestamp));
    int returnCode = sendto(clientSocket, data, dataSize, 0, (struct sockaddr *)&serverAddr, serverlen);
    if (returnCode < 0){
        std::cerr << "ERROR in sendto\n";
    }
}