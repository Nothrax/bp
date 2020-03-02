cmake_minimum_required(VERSION 3.15)
project(ConfigGenerator)

set(CMAKE_CXX_STANDARD 14)
include_directories(./)

add_executable(ConfigGenerator main.cpp Connection/Database.cpp Connection/Database.h Structures.h
        ParamsGetter.cpp ParamsGetter.h Logger/Logger.h Logger/Logger.cpp ConfigGenerator.cpp ConfigGenerator.h)


target_link_libraries(ConfigGenerator mysqlcppconn  ${CMAKE_THREAD_LIBS_INIT})