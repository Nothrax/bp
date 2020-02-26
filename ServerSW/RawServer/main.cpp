/**
 * @file main.cpp
 *
 * @brief
 *
 * @date 22.7.2019
 * @author Jakub Trubka
 * @contact: trubka.jakub@gmail.com
 *
 */

#include "RawServer.h"
#include "parameters/configParamsGetter.h"


int main(int argc, char **argv) {
    Config serverArguments = parseArguments(argc, argv);
    createLogger(serverArguments.verbose);

    auto *server = new RawServer(serverArguments);

    server->startRawServer();
}