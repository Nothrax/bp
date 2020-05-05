//
// Created by Jakub Trubka on 24.02.20.
//

#include <arpa/inet.h>
#include "UDPClient.h"


UDPClient::UDPClient(Config config){
    this->config = config;
}

void UDPClient::init() {
    if ((server = gethostbyname(config.udpIp.c_str())) == nullptr) {
        Logger::logError("ERROR: no such host as " + config.udpIp);
        exit(EXIT_FAILURE);
    }

    /* 3. nalezeni IP adresy serveru a inicializace struktury server_address */
    bzero((char *) &serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serverAddr.sin_addr.s_addr, server->h_length);
    serverAddr.sin_port = htons(config.udpPort);
    serverlen = sizeof(serverAddr);

    /* Vytvoreni soketu */
    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) <= 0) {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }

    udpMessage.version = 1;
    udpMessage.uid = config.uid;
    udpMessage.numberOfSensors = NUMBER_OF_SENSORS;

    for(int i = 0; i < NUMBER_OF_SENSORS; i++){
        udpMessage.sensor[i].dataType = config.sensorType[i];
        udpMessage.sensor[i].sensorId = i;
    }
}

void UDPClient::sendData(std::list<DataPoint> dataPoints) {
/*    for(DataPoint &datapoint: dataPoints){
        printf("%d %d %d\n", datapoint.sensorNumber, datapoint.time, datapoint.timeOffset);
    }*/

    udpMessage.timestamp = dataPoints.front().time;
    int pointsCounter = dataPoints.size();
    int pointsSaved = 0;

    for(int pointIndex = 0; pointIndex < pointsCounter; pointIndex++){
        if(udpMessage.timestamp != dataPoints.front().time || pointsSaved == 56){
            createUDPMessage();
            sendUDPMessage();
            pointsSaved = 0;
            udpMessage.timestamp = dataPoints.front().time;
        }
        udpMessage.sensor[dataPoints.front().sensorNumber].data[udpMessage.sensor[dataPoints.front().sensorNumber].dataSize].value = dataPoints.front().value;
        udpMessage.sensor[dataPoints.front().sensorNumber].data[udpMessage.sensor[dataPoints.front().sensorNumber].dataSize].offset = dataPoints.front().timeOffset;
        udpMessage.sensor[dataPoints.front().sensorNumber].dataSize++;
        pointsSaved++;
        dataPoints.pop_front();
    }
    if(pointsSaved != 0){
        createUDPMessage();
        sendUDPMessage();
    }

}

void UDPClient::createUDPMessage() {
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

void UDPClient::sendUDPMessage() {
    int returnCode = sendto(clientSocket, data, dataSize, 0, (struct sockaddr *)&serverAddr, serverlen);
    if (returnCode < 0){
        Logger::logError("ERROR in sendto");
    }
    for(int i = 0; i < NUMBER_OF_SENSORS; i++){
        udpMessage.sensor[i].dataSize = 0;
    }
}

void UDPClient::encryptMessage() {
    for(int i = 0; i < 512; i++) {
        data[i] = data[i] ^ xorKey[i];
    }
}