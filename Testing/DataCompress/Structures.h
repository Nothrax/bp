//
// Created by nothrax on 11.04.20.
//

#ifndef DATACOMPRESS_STRUCTURES_H
#define DATACOMPRESS_STRUCTURES_H

struct Arguments{
    std::string file;
};

struct Difference{
    uint32_t diff;
    bool negative;
};

#endif //DATACOMPRESS_STRUCTURES_H
