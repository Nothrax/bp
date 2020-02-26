//
// Created by root on 06.02.20.
//

#ifndef BP_SHMRAWREAD_H
#define BP_SHMRAWREAD_H

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>

class SHMRawRead {
public:
    void openSharedMemory();
    void readBuffer(uint8_t *destination, uint32_t bufferSize);
private:
    boost::interprocess::managed_shared_memory *rawBuffer1;
    boost::interprocess::managed_shared_memory *rawBuffer2;

    boost::interprocess::named_semaphore *semaphore1;
    boost::interprocess::named_semaphore *semaphore2;

    boost::interprocess::managed_shared_memory *actualReadBuffer;
};


#endif //BP_SHMRAWREAD_H
