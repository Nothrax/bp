
#include <iostream>
#include "Structures.h"
#include "Configuration/Configuration.h"
#include "Logger/Logger.h"
#include "Aggregation.h"

int main(int argc , char **argv) {
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
    Logger::initLogger(config.logPath, config.verbose);

    auto aggregationHandler = new Aggregation(config);
    aggregationHandler->init();
    aggregationHandler->start();

    return 0;
}
