//
// Created by Jakub Trubka on 31.03.20.
//

#ifndef MESSAGEGENERATOR_SENDERTHREAD_H
#define MESSAGEGENERATOR_SENDERTHREAD_H

#include "Structures.h"
#include "UDP.h"
#include "TCP.h"

class SenderThread {
public:
    static void startThread(Arguments arguments, uint32_t threadIndex, int numberOfMessages);
};


#endif //MESSAGEGENERATOR_SENDERTHREAD_H
