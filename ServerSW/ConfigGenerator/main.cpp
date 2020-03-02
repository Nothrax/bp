#include <iostream>
#include "ParamsGetter.h"
#include "ConfigGenerator.h"

int main(int argc, char **argv) {
    Logger::initLogger("./", true);
    Arguments arguments = parseArguments(argc, argv);

    auto configGenerator = new ConfigGenerator(arguments);
    configGenerator->generateConfig();
    std::cout << configGenerator->getConfigString();
    return 0;
}
