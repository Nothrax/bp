//
// Created by Jakub Trubka on 08.04.20.
//

#ifndef UNTITLED_STRUCTURES_H
#define UNTITLED_STRUCTURES_H


#define WRITE_BUFFER_SIZE 512

struct Arguments{
    std::string file = "";
};

struct Difference{
    uint32_t diff;
    bool negative;
};

struct Buffer{
    char *data;
    uint32_t size;
};

#endif //UNTITLED_STRUCTURES_H

