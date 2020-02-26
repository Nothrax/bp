//
// Created by root on 17.02.20.
//

#ifndef READER_SHMAGGREGATIONWRITE_H
#define READER_SHMAGGREGATIONWRITE_H

#include <cstdio>
#include <iostream>
#include "structures.h"
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>

class SHMAggregationWrite {
public:
    void writeBlock(uint8_t *data);
    void openSharedMemory();
    static void initializeSharedMemory(Config config);
    explicit SHMAggregationWrite(Config config);
    ~SHMAggregationWrite();
private:
    Config config;
    boost::interprocess::managed_shared_memory *aggregationBuffer1;
    boost::interprocess::managed_shared_memory *aggregationBuffer2;

    boost::interprocess::named_semaphore *semaphore1;
    boost::interprocess::named_semaphore *semaphore2;

    boost::interprocess::managed_shared_memory *actualWriteBuffer;

    void switchBuffers();
};


#endif //READER_SHMAGGREGATIONWRITE_H
