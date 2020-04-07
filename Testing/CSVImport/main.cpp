#include <iostream>
#include <unistd.h>
#include <thread>
#include <fstream>
#include "Structures.h"
#include "UDP.h"

void checkArguments(Arguments arguments);
Arguments parseArguments(int argc, char** argv);

int main(int argc, char **argv) {
    Arguments arguments = parseArguments(argc, argv);
    checkArguments(arguments);
    UDP udp(arguments, 0xf9990001);
    udp.initSocket();

    std::fstream file;
    std::string valueString, timeString, headerString;

    file.open(arguments.file, std::ios::in);

    getline(file, headerString, '\n');
    if(headerString != "time,value"){
        file.close();
        std::cerr << "Wrong file format\n";
        exit(EXIT_SUCCESS);
    }

    while (getline(file, timeString, ',')) {
        uint32_t seconds, nanoseconds;
        float value;
        getline(file, valueString, '\n');


        value = std::stof(valueString);
        seconds = std::stoi(timeString.substr(0,10));
        nanoseconds = std::stoi(timeString.substr(10, timeString.size()));

        udp.sendPoint(time(nullptr), nanoseconds, value);
        sleep(1);
        printf("Sending point: %010d%09d, %f\n", seconds, nanoseconds, value);
    }

    file.close();

    return EXIT_SUCCESS;
}

Arguments parseArguments(int argc, char** argv){
    Arguments arguments;
    std::string mode;
    int opt;
    while((opt = getopt(argc, argv, "m:u:f:p:a:")) != -1)
    {
        switch(opt)
        {
            case 'f':
                arguments.file = std::string(optarg);
                break;
            case 'p':
                arguments.port = atoi(optarg);
                break;
            case 'a':
                arguments.ip = std::string(optarg);
                break;
        }
    }

    // optind is for the extra arguments
    // which are not parsed
    for(; optind < argc; optind++){
        printf("extra arguments: %s\n", argv[optind]);
    }

    return arguments;
}


void checkArguments(Arguments arguments){
    if(arguments.file.empty()){
        std::cerr << "Wrong or missing time between messages, should be -f <time in milliseconds>";
        exit(EXIT_FAILURE);
    }

    if(arguments.port == -1){
        std::cerr << "Wrong or missing port, should be -p <port>";
        exit(EXIT_FAILURE);
    }

    if(arguments.ip.empty()){
        std::cerr << "Wrong or missing ip, should be -a <ip>";
        exit(EXIT_FAILURE);
    }
}
