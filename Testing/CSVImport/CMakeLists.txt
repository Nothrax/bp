cmake_minimum_required(VERSION 3.15)
project(CSVImport)
set(CMAKE_CXX_STANDARD 14)

add_executable(CSVImport main.cpp Structures.h UDP.cpp UDP.h)

