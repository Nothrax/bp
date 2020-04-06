/**
 * @file Server.cpp
 *
 * @brief
 *
 * @date 23.7.2019
 * @author Jakub Trubka
 * @contact: trubka.jakub@gmail.com
 *
 */
#include "RawServer.h"

RawServer::RawServer(Config serverArguments) {
    logger = spdlog::get("Raw_server");
    tcpServer = new TCPServer();
    this->serverArguments = serverArguments;
}

void RawServer::startRawServer() {
    tcpServer->startTCPServer(serverArguments.port);
    uint32_t receivedMessages = 0;
    uint32_t startTime = time(nullptr);
    while(true){
        tcpServer->waitForConnection();

        handleNewConnection();
        receivedMessages++;
        std::cout << receivedMessages << " " <<  time(nullptr) - startTime << std::endl;
    }
}
void RawServer::handleNewConnection() {

    int pid = fork();
    if(pid < 0){
        logger->error("Forking failed");
        exit(EXIT_FAILURE);
    }

    //todo change logic to == 0
    if(pid == 0){   //child process

        binaryMessage = tcpServer->receiveMessage();

        forwardMessage();

        exit(EXIT_SUCCESS);

    }else{ //parent process
        signal(SIGCHLD,SIG_IGN);
        //exit(EXIT_SUCCESS);
    }
}

void RawServer::forwardMessage(){
    auto *messageHandler = new Message();
    try{
        messageHandler->processMessage(binaryMessage);
    }catch (std::exception &e){
        logger->error("Exception occured: {0}", e.what());
        exit(1);
    }

    auto *tdmsHandler = new TDMSFile();
    string filename = tdmsHandler->saveMessageIntoFile(messageHandler);
}
