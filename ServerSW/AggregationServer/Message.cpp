//
// Created by Jakub Trubka on 01.08.19.
//

#include <cstring>
#include <iomanip>
#include "Message.h"

Message::Message(uint8_t *data, unsigned int bytesReceived, std::string address, int port) {
    if(bytesReceived < 0 || bytesReceived > MAX_MESSAGE_SIZE){
        throw std::runtime_error("Received message outside data range of <0, 512>Bytes");
    }

    bzero(this->data, MAX_MESSAGE_SIZE);
    memcpy(this->data, data, MAX_MESSAGE_SIZE);

    this->bytesReceived = bytesReceived;
    this->clientIp = address;
    this->clientPort = port;
}

void Message::printMessageIntoFile() {
    FILE * pFile;
    pFile = fopen ((getUidString() + ".txt").c_str(), "wb");

    fwrite ( data, 1, bytesReceived, pFile );

    fclose (pFile);
}

std::string Message::getUidString() {
    std::stringstream stream;
    stream <<  std::setfill('0') << std::setw(8) <<  std::hex << formattedMessage.uid;
    return stream.str();
}


UDPMessage Message::getFormattedMessage() {
    return formattedMessage;
}


void Message::parseMessage(){
    int offset = 0;

    memcpy(&formattedMessage.version, &data[offset], sizeof(formattedMessage.version));
    offset += sizeof(formattedMessage.version);

    memcpy(&formattedMessage.uid, &data[offset], sizeof(formattedMessage.uid));
    offset += sizeof(formattedMessage.uid);

    memcpy(&formattedMessage.timestamp, &data[offset], sizeof(formattedMessage.timestamp));
    offset += sizeof(formattedMessage.timestamp);

    memcpy(&formattedMessage.numberOfSensors, &data[offset], sizeof(formattedMessage.numberOfSensors));
    offset += sizeof(formattedMessage.numberOfSensors);

    if(formattedMessage.numberOfSensors > 0){
        formattedMessage.sensor = new Sensor[formattedMessage.numberOfSensors];
    }

    for(int i = 0; i < formattedMessage.numberOfSensors; i++){
        memcpy(&formattedMessage.sensor[i].sensorId, &data[offset], sizeof(formattedMessage.sensor[i].sensorId));
        offset += sizeof(formattedMessage.sensor[i].sensorId);

        memcpy(&formattedMessage.sensor[i].dataType, &data[offset], sizeof(formattedMessage.sensor[i].dataType));
        offset += sizeof(formattedMessage.sensor[i].dataType);

        memcpy(&formattedMessage.sensor[i].dataSize, &data[offset], sizeof(formattedMessage.sensor[i].dataSize));
        offset += sizeof(formattedMessage.sensor[i].dataSize);

        if(formattedMessage.version == 1){
            if(formattedMessage.sensor[i].dataSize > 0){
                formattedMessage.sensor[i].data = new SensorData[formattedMessage.sensor[i].dataSize];
            }

            for(int j = 0; j < formattedMessage.sensor[i].dataSize; j++){
                memcpy(&formattedMessage.sensor[i].data[j].offset, &data[offset], sizeof(formattedMessage.sensor[i].data[j].offset));
                offset += sizeof(formattedMessage.sensor[i].data[j].offset);

                memcpy(&formattedMessage.sensor[i].data[j].value, &data[offset], sizeof(formattedMessage.sensor[i].data[j].value));
                offset += sizeof(formattedMessage.sensor[i].data[j].value);
            }
        }
    }
}

Message::~Message() {
    for(int i = 0; i < formattedMessage.numberOfSensors; i++){
        delete [] formattedMessage.sensor[i].data;
    }
    delete [] formattedMessage.sensor;
}



