cmake_minimum_required(VERSION 3.15)
project(Encoder)

set(CMAKE_CXX_STANDARD 14)

add_executable(Encoder main.cpp Structures.h DeltaEncode.cpp DeltaEncode.h)