//
// Created by Jakub Trubka on 01.08.19.
//

#ifndef AGGREGATIONSERVER_MESSAGE_H
#define AGGREGATIONSERVER_MESSAGE_H

#include <string>
#include "parameters/configParamsGetter.h"


class Message {
public:
    Message(uint8_t *data, unsigned int bytesReceived, std::string address, int port);

    void printMessageIntoFile();
    std::string getUidString();
    UDPMessage getFormattedMessage();
    void parseMessage();
    ~Message();

private:
    unsigned int bytesReceived;
    uint8_t data[MAX_MESSAGE_SIZE];
    std::string clientIp;
    int clientPort;
    UDPMessage formattedMessage;

};


#endif //AGGREGATIONSERVER_MESSAGE_H
