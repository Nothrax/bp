//
// Created by Jakub Trubka on 17.02.20.
//

#include "SHMAggregationWrite.h"

SHMAggregationWrite::SHMAggregationWrite(Config config){
    this->config = config;
}

SHMAggregationWrite::~SHMAggregationWrite() {
    using namespace boost::interprocess;
    std::cout << "closing shared memory2\n";
    shared_memory_object::remove("aggregationBuffer1");
    shared_memory_object::remove("aggregationBuffer2");
    named_semaphore::remove("aggregationSemaphore1");
    named_semaphore::remove("aggregationSemaphore2");
}

void SHMAggregationWrite::openSharedMemory() {
    std::cout << "opening shared memory\n";
    using namespace boost::interprocess;
    aggregationBuffer1 = new  managed_shared_memory(open_only
            , "aggregationBuffer1");
    std::cout << "opening shared memory2\n";
    aggregationBuffer2 = new managed_shared_memory (open_only
            , "aggregationBuffer2");
    std::cout << "opening shared memory3\n";

    semaphore1 = new named_semaphore(open_only, "aggregationSemaphore1");
    semaphore2 = new named_semaphore(open_only, "aggregationSemaphore2");

    actualWriteBuffer = aggregationBuffer1;
}

void SHMAggregationWrite::initializeSharedMemory(Config config) {
    using namespace boost::interprocess;
    shared_memory_object::remove("aggregationBuffer1");
    shared_memory_object::remove("aggregationBuffer2");
    named_semaphore::remove("aggregationSemaphore1");
    named_semaphore::remove("aggregationSemaphore2");

    std::cout << "creating\n";
    managed_shared_memory buffer1      ( create_only
            , "aggregationBuffer1" //Shared memory object name
            , 128000*config.bufferLengthInSeconds);          //Shared memory object size in bytes
    managed_shared_memory buffer2      ( create_only
            , "aggregationBuffer2" //Shared memory object name
            , 128000*config.bufferLengthInSeconds);          //Shared memory object size in bytes
    named_semaphore semaphore_open1(create_only, "aggregationSemaphore1",0);
    named_semaphore semaphore_open2(create_only, "aggregationSemaphore2",1);
}

void SHMAggregationWrite::writeBlock(uint8_t *data) {
    memcpy(actualWriteBuffer->get_address(), data, 128000*config.bufferLengthInSeconds);
    switchBuffers();
}

void SHMAggregationWrite::switchBuffers() {
    if(aggregationBuffer1 == actualWriteBuffer){
        actualWriteBuffer = aggregationBuffer2;
        semaphore1->post();
        semaphore2->wait();
    }else{
        actualWriteBuffer = aggregationBuffer1;
        semaphore2->post();
        semaphore1->wait();
    }
}