cmake_minimum_required(VERSION 3.15)
project(MessageGenerator)
set(CMAKE_CXX_FLAGS " -pthread -lssl -lcrypto -L/usr/lib" )
set(CMAKE_CXX_STANDARD 14)

add_executable(MessageGenerator main.cpp Structures.h UDP.cpp UDP.h TCP.cpp TCP.h SenderThread.cpp SenderThread.h)

find_package(OpenSSL REQUIRED)
target_link_libraries(MessageGenerator OpenSSL::SSL)