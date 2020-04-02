//
// Created by nothrax on 31.03.20.
//

#include "SenderThread.h"
#include "Structures.h"

void SenderThread::startThread(Arguments arguments, uint32_t threadIndex, int numberOfMessages) {
    uint32_t uid = 0xf0010000;
    uid += threadIndex;
    TCP tcp(arguments, uid, numberOfMessages);
    UDP udp(arguments, uid, numberOfMessages);
    switch (arguments.mode){
        case MODE_UDP:
            udp.startSending();
            break;
        case MODE_TCP:
            tcp.startSending();
            break;
    }
}