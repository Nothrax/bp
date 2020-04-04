#include <iostream>
#include <unistd.h>
#include <thread>
#include "Structures.h"
#include "SenderThread.h"

void checkArguments(Arguments arguments);
Arguments parseArguments(int argc, char** argv);

int main(int argc, char **argv) {
    Arguments arguments = parseArguments(argc, argv);
    checkArguments(arguments);

    ulong starTime = time(nullptr);
    std::thread threads[arguments.numberOfUnits];

    for(int threadIndex = 0; threadIndex < arguments.numberOfUnits; threadIndex++){
        uint32_t wait = rand() % arguments.milliseconds;
	uint32_t numberOfMessages = 600000/arguments.milliseconds;
        threads[threadIndex] = std::thread(SenderThread::startThread, arguments, threadIndex, numberOfMessages);
        std::cout << wait << " sleeping\n";
        usleep(wait*1000);
        //SenderThread::startThread(arguments, 0, 10);
    }

    for (auto & myThread : threads){
        myThread.join();
    }

    std::cout << "Sending finished in " << std::to_string(time(nullptr)-starTime) << " seconds\n";

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
            case 'm':
                mode = std::string(optarg);
                if(mode == "UDP"){
                    arguments.mode = MODE_UDP;
                }
                if(mode == "TCP"){
                    arguments.mode = MODE_TCP;
                }
                break;
            case 'u':
                arguments.numberOfUnits = atoi(optarg);
                break;
            case 'f':
                arguments.milliseconds = atoi(optarg);
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
    if(arguments.mode != MODE_UDP && arguments.mode != MODE_TCP){
        std::cerr << "Wrong or missing mode, argument should be -m UDP or -m TCP";
        exit(EXIT_FAILURE);
    }

    if(arguments.numberOfUnits == -1){
        std::cerr << "Wrong or missing number of units, should be -u <number of units>";
        exit(EXIT_FAILURE);
    }

    if(arguments.milliseconds == -1){
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
