//
// Created by Jakub Trubka on 18.02.20.
//

#ifndef AGGREGATION_SHMAGGREGATIONREAD_H
#define AGGREGATION_SHMAGGREGATIONREAD_H

#include <iostream>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>

class SHMAggregationRead {
public:
    void openSharedMemory();
    void readBuffer(uint8_t *destination, uint32_t bufferSize);
private:
    boost::interprocess::managed_shared_memory *buffer1;
    boost::interprocess::managed_shared_memory *buffer2;

    boost::interprocess::named_semaphore *semaphore1;
    boost::interprocess::named_semaphore *semaphore2;

    boost::interprocess::managed_shared_memory *actualReadBuffer;
};


#endif //AGGREGATION_SHMAGGREGATIONREAD_H
