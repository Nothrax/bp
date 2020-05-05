
#include <iostream>
#include <signal.h>
#include "abstractClass/InterfaceReader.h"
#include "dataInterface/SPI.h"
#include "Logger/Logger.h"
#include "configReader/Configuration.h"
#include "filter/Filter.h"
#include "sharedMemory/SHMRawWrite.h"
#define DATA_SIZE 12


int main(int argc, char **argv) {
    if(argc != 3){
        std::cerr << "Wrong number of arguments, should be -p [path to config.ini]" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string argument(argv[1]);
    if(argument != "-p"){

        std::cerr << "Wrong arguments, should be -p [path to config.ini]" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string configPath(argv[2]);

    Config config = Configuration::loadConfig(configPath);


    printf("pred logem\n");
    Logger::initLogger(config.logPath, config.verbose);
    printf("pred shminit\n");
    SHMRawWrite::initializeSharedMemory(config);

    printf("pred forkem\n");
    int pid = fork();

    if(pid < 0){
        Logger::logError("Failed to create new process");
        exit(EXIT_FAILURE);
    }

    if(pid > 0){ //parent
        sleep(1);
        InterfaceReader *reader;
        reader = new SPI(config);
        reader->setUp();
        reader->start();
        sleep(10);
        kill(pid, SIGKILL);
        delete reader;
    }else{  //child
        Filter *filter;
        filter = new Filter(config);
        filter->initialize();
        filter->start();
    }
    return 0;
}