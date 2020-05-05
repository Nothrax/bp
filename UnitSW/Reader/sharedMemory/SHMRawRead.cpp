//
// Created by Jakub Trubka on 06.02.20.
//

#include "SHMRawRead.h"


void SHMRawRead::openSharedMemory() {
    using namespace boost::interprocess;
    rawBuffer1 = new  managed_shared_memory( open_only
            , "RawBuffer1");
    rawBuffer2 = new managed_shared_memory ( open_only
            , "RawBuffer2");
    semaphore1 = new named_semaphore(open_only, "semaphore1");
    semaphore2 = new named_semaphore(open_only, "semaphore2");

    actualReadBuffer = rawBuffer1;
}

void SHMRawRead::readBuffer(uint8_t *destination, uint32_t bufferSize) {
    if(actualReadBuffer == rawBuffer1){
        semaphore1->wait();
        memcpy(destination, rawBuffer1->get_address(), bufferSize); //sizeof(uint32_t)*SHM_NUMBER_OF_MEASUREMENTS*config.numberOfSensors
        semaphore1->post();
        actualReadBuffer = rawBuffer2;
    }else{
        semaphore2->wait();
        memcpy(destination, rawBuffer2->get_address(), bufferSize);
        semaphore2->post();
        actualReadBuffer = rawBuffer1;
    }
}