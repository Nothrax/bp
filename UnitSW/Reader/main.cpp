
#include <iostream>
#include <signal.h>
#include "abstractClass/InterfaceReader.h"
#include "dataInterface/SPI.h"
#include "Logger/Logger.h"
#include "configReader/Configuration.h"
#include "filter/Filter.h"
#include "sharedMemory/SHMRawWrite.h"
#define DATA_SIZE 12

//todo config path brat z argumentu

int main() {
    //todo cesta k config souboru z parametru
    printf("pred configem\n");
    Config config = Configuration::loadConfig("/home/pi/config.ini");

    //vypis configu
    if(false){
        printf("uid: %08x\n", config.uid);
        std::cout << "log path: " << config.logPath << std::endl;
        printf("console log: %d\n", config.verbose);
        printf("data source: %d\n", config.dataSource);
        std::cout << "file path: " << config.dataFilePath << std::endl;
        printf("frequency: %d\n", config.frequency);
        printf("usb backup: %d\n", config.backup);
        std::cout << "backup path: " << config.backupPath << std::endl;
        printf("data ready: %d\n", config.dataReadyPin);

        printf("spi speed : %d\n", config.spiSpeed);
        printf("restart adc: %d\n", config.restartAdcPin);
        printf("raw: %d\n", config.raw);
        printf("sdf: %d\n", config.sdf);
        printf("raw freq: %d\n", config.rawFrequency);

        for(int i = 0; i < NUMBER_OF_SENSORS; i++){
            printf("sensor active: %d\n", config.sensorActive[i]);
            printf("sensor pin: %d\n", config.outputDisablePin[i]);
        }


    }

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