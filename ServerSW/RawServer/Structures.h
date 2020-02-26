/**
 * @file Structurers.h
 *
 * @brief
 *
 * @date 22.7.2019
 * @author Jakub Trubka
 * @contact: trubka.jakub@gmail.com
 *
 */

#include <cstdio>
#include <unistd.h>
#include <cstdint>
#include <ctime>
#include <string>

#ifndef RAWSERVER_STRUCTURES_H
#define RAWSERVER_STRUCTURES_H

#define TDMS_HEADER_SIZE 36
#define TDMS_LEAD_IN_SIZE 28
#define TDMS_META_HEADER_SIZE 101
#define PROTOCOL_0_VALUE_SIZE  3
#define PROTOCOL_1_VALUE_SIZE  4
#define NUMBER_OF_CHANNELS 4
#define TDMS_SENSOR_HEADER_SIZE 87
//#define TCP_HEADER_SIZE 28

#pragma pack(push, 1)
struct TCPMessage{
    /* Header */
    uint16_t version;
    uint32_t uid;
    uint16_t channelMask;
    uint32_t timestamp;
    uint32_t duration;
    float adcConstant;
    float sensitivity[NUMBER_OF_CHANNELS];
    /* Data */
    uint32_t dataSize;
    uint8_t *data;
};
#pragma pack(pop)

struct ChannelUnit{
    std::string description = "";
    uint32_t id = 0;
    uint32_t sf = 0;
    float adc = 0;
    std::string name = "";
    float sensitivity = 0.0;
};

#endif //RAWSERVER_STRUCTURES_H
