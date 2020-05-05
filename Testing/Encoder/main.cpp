#include <iostream>
#include <getopt.h>
#include "DeltaEncode.h"
#include "Structures.h"


void checkArguments(Arguments arguments);
Arguments parseArguments(int argc, char** argv);

int main(int argc, char **argv) {
    Arguments arguments = parseArguments(argc, argv);
    checkArguments(arguments);

    DeltaEncode deltaEncode;
    //deltaEncode.encodeFile(arguments.file, "output.txt");

    deltaEncode.decodeFile("./output.txt", "decodedOutput.txt");
    return 0;
}



Arguments parseArguments(int argc, char** argv){
    Arguments arguments;
    std::string mode;
    int opt;
    while((opt = getopt(argc, argv, "f:")) != -1)
    {
        switch(opt)
        {
            case 'f':
                arguments.file = std::string(optarg);
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
        std::cerr << "Wrong or missing ip, should be -a <ip>";
        exit(EXIT_FAILURE);
    }
}