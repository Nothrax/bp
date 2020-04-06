//
// Created by Jakub Trubka on 21.10.19.
//

#ifndef BP_FILE_H
#define BP_FILE_H

#include <string>
#include <fstream>
#include <thread>
#include <zconf.h>
#include <iostream>
#include <pigpio.h>
#include <bcm2835.h>
#include "../abstractClass/InterfaceReader.h"
#include "structures.h"
#include "sharedMemory/SHMRawWrite.h"
#include <Logger/Logger.h>

class File : public InterfaceReader{
public:
    void setUp() override ;
    void start() override ;
    explicit File(Config config);
    ~File();
private:
    std::fstream input;
    Config config;
    unsigned char default_rx[18] = {0, };

};


#endif //BP_FILE_H
