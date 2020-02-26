cmake_minimum_required(VERSION 3.14)
project(AggregationServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcurl")

set(CMAKE_CXX_FLAGS " -pthread " )
set(SOURCE_FILES AggregationServer.cpp AggregationServer.h Structures.h Message.cpp Message.h communication/UDPServer.cpp communication/UDPServer.h parameters/configParamsGetter.h
        parameters/configParamsGetter.cpp communication/InfluxDatabase.cpp communication/InfluxDatabase.h)



include_directories(.)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/logs")

add_executable(AggregationServer main.cpp ${SOURCE_FILES})
target_link_libraries(AggregationServer ${Boost_LIBRARIES} rt)

include(FindPkgConfig)
pkg_check_modules(CURLPP REQUIRED curlpp)
target_link_libraries(AggregationServer curl)

include(FindPkgConfig)
pkg_check_modules(CURLPP REQUIRED curlpp)
target_link_libraries(AggregationServer ${CURLPP_LDFLAGS})

### CURLPP ##########
## load in pkg-config support
find_package(PkgConfig)

find_package(CURL)
IF (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    #    target_link_libraries(robotClient ${CURL_LIBRARIES})
    target_link_libraries(AggregationServer ${CURL_LIBRARIES})
endif()
