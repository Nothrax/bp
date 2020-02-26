#include <iostream>
#include "AggregationServer.h"
#include "parameters/configParamsGetter.h"


int main(int argc, char **argv) {
    Config config = parseArguments(argc, argv);
    auto server = new AggregationServer(config, UDPServer::startUDPServer(config.port));
    server->startAggregationServer();

    return 0;
}