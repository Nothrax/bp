#include <iostream>
#include <getopt.h>
#include <chrono>
#include "Structures.h"
#include "DeltaCompression.h"
#include "ZipCompression.h"

void checkArguments(Arguments arguments);
Arguments parseArguments(int argc, char** argv);

int main(int argc, char **argv) {
    //Arguments arguments = parseArguments(argc, argv);
    //checkArguments(arguments);

    auto start_time = std::chrono::high_resolution_clock::now();
    DeltaCompression deltaCompression;
    deltaCompression.compressFile("500kB.data", "500kB.compress");
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;

    std::cout << "Delta compression 500kB " << time/std::chrono::milliseconds(1) << "ms\n";

    start_time = std::chrono::high_resolution_clock::now();
    deltaCompression.compressFile("3MB.data", "3MB.compress");
    end_time = std::chrono::high_resolution_clock::now();
    time = end_time - start_time;

    std::cout << "Delta compression 3MB " << time/std::chrono::milliseconds(1) << "ms\n";

    start_time = std::chrono::high_resolution_clock::now();
    deltaCompression.compressFile("10MB.data", "10MB.compress");
    end_time = std::chrono::high_resolution_clock::now();
    time = end_time - start_time;

    std::cout << "Delta compression 10MB " << time/std::chrono::milliseconds(1) << "ms\n";

    start_time = std::chrono::high_resolution_clock::now();
    ZipCompression zipCompression;
    zipCompression.compressFile("500kB.data", "500kB.zip");
    end_time = std::chrono::high_resolution_clock::now();
    time = end_time - start_time;

    std::cout << "Deflate compression 500kB " << time/std::chrono::milliseconds(1) << "ms\n";

    start_time = std::chrono::high_resolution_clock::now();
    zipCompression.compressFile("3MB.data", "3MB.zip");
    end_time = std::chrono::high_resolution_clock::now();
    time = end_time - start_time;

    std::cout << "Deflate compression 3MB " << time/std::chrono::milliseconds(1) << "ms\n";

    start_time = std::chrono::high_resolution_clock::now();
    zipCompression.compressFile("10MB.data", "10MB.zip");
    end_time = std::chrono::high_resolution_clock::now();
    time = end_time - start_time;

    std::cout << "Deflate compression 10MB " << time/std::chrono::milliseconds(1) << "ms\n";


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