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

class File : public InterfaceReader{
public:
    void setUp() override ;
    void start() override ;
    explicit File(Config config);
    ~File();
private:
    std::fstream input;
    Config config;

};


#endif //BP_FILE_H
