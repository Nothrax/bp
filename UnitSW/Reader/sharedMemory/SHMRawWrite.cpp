//
// Created by Jakub Trubka on 04.02.20.
//

#include "SHMRawWrite.h"


void SHMRawWrite::addMeasurement(uint8_t recieveRegister[18]) {
    int32_t rawValueSensors[NUMBER_OF_SENSORS] = {0,0,0,0};
    for(int i = 0; i < NUMBER_OF_SENSORS; i++){
        rawValueSensors[i] += recieveRegister[3+i*3] << 24;
        rawValueSensors[i] += recieveRegister[4+i*3] << 16;
        rawValueSensors[i] += recieveRegister[5+i*3] << 8;
        ((int32_t *)(actualWriteBuffer->get_address()))[sharedMemoryIndex+config.frequency*config.bufferLengthInSeconds*i] = rawValueSensors[i];
    }

    sharedMemoryIndex += 1;//sizeof(int32_t);
    if(sharedMemoryIndex == config.frequency*config.bufferLengthInSeconds){//*sizeof(int32_t)){
        switchBuffers();
    }
}

SHMRawWrite::SHMRawWrite(Config config){
    this->config = config;
}


void SHMRawWrite::openSharedMemory() {
    using namespace boost::interprocess;
    rawBuffer1 = new  managed_shared_memory( open_only
            , "RawBuffer1");
    rawBuffer2 = new managed_shared_memory ( open_only
            , "RawBuffer2");

    semaphore1 = new named_semaphore(open_only, "semaphore1");
    semaphore2 = new named_semaphore(open_only, "semaphore2");

    actualWriteBuffer = rawBuffer1;
}

void SHMRawWrite::initializeSharedMemory(Config config) {
    using namespace boost::interprocess;
    shared_memory_object::remove("RawBuffer1");
    shared_memory_object::remove("RawBuffer2");
    named_semaphore::remove("semaphore1");
    named_semaphore::remove("semaphore2");

    managed_shared_memory buffer1      ( create_only
            , "RawBuffer1" //Shared memory object name
            , 2048000*config.bufferLengthInSeconds);          //Shared memory object size in bytes
    managed_shared_memory buffer2      ( create_only
            , "RawBuffer2" //Shared memory object name
            , 2048000*config.bufferLengthInSeconds);          //Shared memory object size in bytes
    named_semaphore semaphore_open1(create_only, "semaphore1",0);
    named_semaphore semaphore_open2(create_only, "semaphore2",1);
}

void SHMRawWrite::switchBuffers() {
    sharedMemoryIndex = 0;

    if(rawBuffer1 == actualWriteBuffer){
        actualWriteBuffer = rawBuffer2;
        semaphore1->post();
        semaphore2->wait();
    }else{
        actualWriteBuffer = rawBuffer1;
        semaphore2->post();
        semaphore1->wait();
    }
}
