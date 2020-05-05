//
// Created by Jakub Trubka on 31.03.20.
//

#include <strings.h>
#include <iostream>
#include <cstring>
#include "UDP.h"


UDP::UDP(Arguments arguments, uint32_t uid, uint32_t numberOfMessages){
    this->arguments = arguments;
    this->numberOfMessages = numberOfMessages;
    udpMessage.uid = uid;
}

void UDP::startSending() {
    createUDPMessage();
    if(encrypt){
        encryptMessage();
    }
    initSocket();

    for(int i = 0; i < numberOfMessages; i++){
        sendUDPMessage();
        usleep(arguments.milliseconds*1000);
    }
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

void UDP::createUDPMessage() {
    udpMessage.version = 1;
    udpMessage.numberOfSensors = NUMBER_OF_SENSORS;

    for(int i = 0; i < NUMBER_OF_SENSORS; i++){
        udpMessage.sensor[i].dataType = 1;
        udpMessage.sensor[i].sensorId = i;
    }

    udpMessage.timestamp = 0;

    for(int pointIndex = 0; pointIndex < 12; pointIndex++){
        for(int sensorIndex = 0; sensorIndex < NUMBER_OF_SENSORS; sensorIndex++){
            udpMessage.sensor[sensorIndex].data[pointIndex].value = 42.0;
            udpMessage.sensor[sensorIndex].data[pointIndex].offset = 83*pointIndex*1000000;
            udpMessage.sensor[sensorIndex].dataSize++;
        }
    }

    dataSize = 0;

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
    if(encrypt){
        data[6] ^= xorKey[6];
        data[7] ^= xorKey[7];
        data[8] ^= xorKey[8];
        data[9] ^= xorKey[9];
    }
    memcpy(&data[6], &udpMessage.timestamp, sizeof(udpMessage.timestamp));
    if(encrypt){
        data[6] ^= xorKey[6];
        data[7] ^= xorKey[7];
        data[8] ^= xorKey[8];
        data[9] ^= xorKey[9];
    }
    int returnCode = sendto(clientSocket, data, dataSize, 0, (struct sockaddr *)&serverAddr, serverlen);
    if (returnCode < 0){
        std::cerr << "ERROR in sendto\n";
    }

    udpMessage.timestamp++;
}

void UDP::encryptMessage() {
    for(int i = 0; i < 512; i++) {
        data[i] ^= xorKey[i];
    }
}