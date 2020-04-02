cmake_minimum_required(VERSION 3.15)
project(MessageGenerator)
set(CMAKE_CXX_FLAGS " -pthread " )
set(CMAKE_CXX_STANDARD 14)

add_executable(MessageGenerator main.cpp Structures.h UDP.cpp UDP.h TCP.cpp TCP.h SenderThread.cpp SenderThread.h)