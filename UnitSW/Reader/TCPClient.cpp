//
// Created by root on 13.02.20.
//

#include "TCPClient.h"

TCPClient::TCPClient(Config config){
    this->config = config;

    tcpMessage.version = 1;
    tcpMessage.uid = config.uid;
    tcpMessage.channelMask = 0;

    tcpMessage.channelMask |= config.sensorActive[3] << 3;
    tcpMessage.channelMask |= config.sensorActive[2] << 2;
    tcpMessage.channelMask |= config.sensorActive[1] << 1;
    tcpMessage.channelMask |= config.sensorActive[0] << 0;

    tcpMessage.duration = config.bufferLengthInSeconds;
    tcpMessage.adcConstant = config.adcConstant;

    tcpMessage.dataSize = 0;
    for(int sensorIndex = 0; sensorIndex < NUMBER_OF_SENSORS; sensorIndex++){
        tcpMessage.sensitivity[sensorIndex] = config.sensorSensitivity[sensorIndex];

        if(config.sensorActive[sensorIndex]){
            tcpMessage.dataSize += sizeof(q31_t)*128000*config.bufferLengthInSeconds;
        }
    }

}

void TCPClient::sendMessage(uint8_t *buffer, uint32_t time) {
    tcpMessage.timestamp = time;
    int sock;
    struct sockaddr_in server;

    //todo copy data to send buffer?

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        Logger::logError("Socket error");
    } else {
        server.sin_addr.s_addr = inet_addr(config.tcpIp.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(config.tcpPort);

        // Connect to remote server
        if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
            Logger::logError("TCP connect error");
        } else {
            write(sock, &tcpMessage, sizeof(TCPMessage) - sizeof(u_int8_t *));

            uint32_t sensorOffset = 0;
            for(int sensorIndex = 0; sensorIndex < NUMBER_OF_SENSORS; sensorIndex++){
                if(config.sensorActive[sensorIndex]){
                    write(sock, buffer + sensorOffset, sizeof(q31_t)*128000*config.bufferLengthInSeconds);
                }
                sensorOffset += sizeof(q31_t)*128000*config.bufferLengthInSeconds;
            }
        }
    }
    // Connection close
    close(sock);
}

