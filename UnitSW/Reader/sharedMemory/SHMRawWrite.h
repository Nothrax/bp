//
// Created by root on 04.02.20.
//

#ifndef BP_SHMRAWWRITE_H
#define BP_SHMRAWWRITE_H

#include <cstdio>
#include <iostream>
#include <fstream>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>

#include "structures.h"
#include "Logger/Logger.h"

class SHMRawWrite {
public:
    void addMeasurement(uint8_t recieveRegister[18]);
    void openSharedMemory();
    static void initializeSharedMemory(Config config);
    SHMRawWrite(Config config);
private:
    Config config;
    ulong sharedMemoryIndex = 0;
    boost::interprocess::managed_shared_memory *rawBuffer1;
    boost::interprocess::managed_shared_memory *rawBuffer2;

    boost::interprocess::named_semaphore *semaphore1;
    boost::interprocess::named_semaphore *semaphore2;

    boost::interprocess::managed_shared_memory *actualWriteBuffer;

    void switchBuffers();
};


#endif //BP_SHMRAWWRITE_H
