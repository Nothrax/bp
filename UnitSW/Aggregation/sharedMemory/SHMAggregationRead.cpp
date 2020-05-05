//
// Created by Jakub Trubka on 18.02.20.
//

#include "SHMAggregationRead.h"

void SHMAggregationRead::openSharedMemory() {
    using namespace boost::interprocess;
    buffer1 = new  managed_shared_memory( open_read_only
            , "aggregationBuffer1");
    buffer2 = new managed_shared_memory ( open_read_only
            , "aggregationBuffer2");
    semaphore1 = new named_semaphore(open_only, "aggregationSemaphore1");
    semaphore2 = new named_semaphore(open_only, "aggregationSemaphore2");

    actualReadBuffer = buffer1;
}

void SHMAggregationRead::readBuffer(uint8_t *destination, uint32_t bufferSize) {
    if(actualReadBuffer == buffer1){
        semaphore1->wait();
        memcpy(destination, buffer1->get_address(), bufferSize); //sizeof(uint32_t)*SHM_NUMBER_OF_MEASUREMENTS*config.numberOfSensors
        semaphore1->post();
        actualReadBuffer = buffer2;
    }else{
        semaphore2->wait();
        memcpy(destination, buffer2->get_address(), bufferSize);
        semaphore2->post();
        actualReadBuffer = buffer1;
    }
}
