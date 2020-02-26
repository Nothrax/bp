cmake_minimum_required(VERSION 3.10)

project(RawServer)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        main.cpp RawServer.cpp RawServer.h Structures.h Message.cpp Message.h
        TDMSFile.cpp TDMSFile.h FileCompressor.cpp FileCompressor.h parameters/configParamsGetter.cpp parameters/configParamsGetter.h
        communication/TCPServer.cpp communication/TCPServer.cpp)

find_package (Threads)
include_directories(.)
add_library(source ${SOURCE_FILES})


# Create logs directory
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/logs")

add_library(libs main.cpp)
find_package (Threads)
#target_link_libraries(libs source mysqlcppconn ${CMAKE_THREAD_LIBS_INIT} rt)
add_executable(RawServer ${SOURCE_FILES})
target_link_libraries(RawServer "libzip.so")
target_link_libraries(RawServer libs)
target_link_libraries(RawServer curl)



### CURLPP ##########
## load in pkg-config support
find_package(PkgConfig)
## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_ZeroMQ QUIET zmq)

find_package(CURL)
IF (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    target_link_libraries(RawServer ${CURL_LIBRARIES})
endif()
