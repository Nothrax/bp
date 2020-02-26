/**
 * @file Message.cpp
 *
 * @brief
 *
 * @date 24.7.2019
 * @author Jakub Trubka
 * @contact: trubka.jakub@gmail.com
 *
 */

#include <stdexcept>
#include <sstream>
#include <iomanip>
#include "Message.h"


void Message::processMessage(TCPMessage message) {
    logger = spdlog::get("Raw_server");
    this->binaryMessage = message;
    setChannels();
    validateHeader();
}


void Message::validateHeader() {

    if(numberOfChannels == 0){
        throw std::logic_error("Message contains 0 channelMask.");
    }

    if(binaryMessage.dataSize == 0){
        throw std::logic_error("Message contains no data.");
    }
    if(binaryMessage.version == 0){
        if(binaryMessage.dataSize % PROTOCOL_0_VALUE_SIZE != 0){
            throw std::logic_error("Message dataSize is not divisible by 24 bits - size of one value in protocol version 0.");
        }
    }

    if(binaryMessage.version == 1){
        if(binaryMessage.dataSize % PROTOCOL_1_VALUE_SIZE != 0){
            throw std::logic_error("Message dataSize is not divisible by 4 bytes - size of one value in protocol version 1.");
        }
    }


    if(binaryMessage.dataSize % numberOfChannels != 0){
        throw std::logic_error("Message dataSize is not divisible by number of channelMask.");
    }
}

void Message::setChannels() {
    channels[0] = binaryMessage.channelMask & 0x0001;
    channels[1] = binaryMessage.channelMask & 0x0002;
    channels[2] = binaryMessage.channelMask & 0x0004;
    channels[3] = binaryMessage.channelMask & 0x0008;

    for(bool &channel: channels){
        if(channel){
            numberOfChannels++;
        }
    }
}

std::string Message::getUIDString() {
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(8) << std::hex << this->getUID();
    return stream.str();
}

time_t Message::getStartTime() {
    return binaryMessage.timestamp;
}

uint8_t * Message::getRawData() {
    return binaryMessage.data;
}

uint32_t Message::getRawDataSize() {
    return binaryMessage.dataSize;
}

uint32_t Message::getUID() {
    return binaryMessage.uid;
}

bool* Message::getChannels() {
    return channels;
}

int Message::getNumberOfChannels() const {
    return numberOfChannels;
}

uint16_t Message::getMeasureDuration() {
    return binaryMessage.duration;
}

float Message::getAdcConstant() {
    return binaryMessage.adcConstant;
}

float Message::getChannelSensitivity(int channel) {
    return (float)binaryMessage.sensitivity[channel];
}

uint8_t Message::getProtocolVersion() {
    return this->binaryMessage.version;
}