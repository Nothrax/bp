cmake_minimum_required(VERSION 3.13.4)
project(Reader)
set(CMAKE_CXX_FLAGS " -pthread -lrt" )

set(CMAKE_CXX_STANDARD 14)
set(SRC abstractClass/InterfaceReader.cpp abstractClass/InterfaceReader.h dataInterface/File.cpp dataInterface/File.h dataInterface/SPI.cpp dataInterface/SPI.h
        Logger/Logger.cpp Logger/Logger.h configReader/Configuration.cpp configReader/Configuration.h structures.h configReader/ini.h configReader/ini.c
        sharedMemory/SHMRawWrite.cpp sharedMemory/SHMRawWrite.h filter/arm_fir_decimate.h filter/arm_fir_decimate_init_q31.c filter/arm_fir_decimate_q31.c
        filter/Filter.cpp filter/Filter.h sharedMemory/SHMRawRead.cpp sharedMemory/SHMRawRead.h configReader/INIReader.h TCPClient.cpp TCPClient.h sharedMemory/SHMAggregationWrite.cpp sharedMemory/SHMAggregationWrite.h)

include_directories(./)

add_executable(Reader main.cpp ${SRC})
target_link_libraries(Reader bcm2835)
target_link_libraries(Reader pigpio)