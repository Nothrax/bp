/**
 * @file Message.h
 *
 * @brief
 *
 * @date 24.7.2019
 * @author Jakub Trubka
 * @contact: trubka.jakub@gmail.com
 *
 */

#ifndef RAWSERVER_MESSAGE_H
#define RAWSERVER_MESSAGE_H


#include "Structures.h"
#include "spdlog/spdlog.h"
#include <exception>

class Message {
public:
    void processMessage(TCPMessage message);
    time_t getStartTime();
    uint8_t *getRawData();
    uint32_t getRawDataSize();
    uint32_t getUID();
    std::string getUIDString();
    float getAdcConstant();
    uint8_t getProtocolVersion();
    float getChannelSensitivity(int channel);
    bool *getChannels();
    uint16_t getMeasureDuration();
    int getNumberOfChannels() const;
private:
    TCPMessage binaryMessage;
    bool channels[NUMBER_OF_CHANNELS] = {false};
    int numberOfChannels = 0;
    std::shared_ptr<spdlog::logger> logger;

    void setChannels();
    void validateHeader();
};


#endif //RAWSERVER_MESSAGE_H
