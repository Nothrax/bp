cmake_minimum_required(VERSION 3.13.4)
project(Aggregation)
set(CMAKE_CXX_FLAGS " -pthread -lrt" )

set(CMAKE_CXX_STANDARD 14)
set(SRC Configuration/Configuration.cpp Configuration/Configuration.h Logger/Logger.h Logger/Logger.cpp Structures.h Aggregation.cpp Aggregation.h sharedMemory/SHMAggregationRead.cpp sharedMemory/SHMAggregationRead.h UDPClient.cpp UDPClient.h)

include_directories(./)


add_executable(Aggregation main.cpp ${SRC})