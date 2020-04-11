cmake_minimum_required(VERSION 3.15)
project(DataCompress)

set(CMAKE_CXX_STANDARD 14)

add_executable(DataCompress main.cpp Structures.h DeltaCompression.cpp DeltaCompression.h ZipCompression.cpp ZipCompression.h)
target_link_libraries(DataCompress "libzip.so")