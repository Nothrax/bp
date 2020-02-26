/**
 * @file Server.h
 *
 * @brief
 *
 * @date 23.7.2019
 * @author Jakub Trubka
 * @contact: trubka.jakub@gmail.com
 *
 */
#include <netinet/in.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>

#include "Message.h"
#include "TDMSFile.h"
#include "communication/TCPServer.h"
#include "parameters/configParamsGetter.h"


#ifndef RAWSERVER_COMMUNICATION_H
#define RAWSERVER_COMMUNICATION_H


class RawServer {
public:
    explicit RawServer(Config serverArguments);
    ~RawServer(){
        free(binaryMessage.data);
    }

    void startRawServer();

private:
    TCPMessage binaryMessage;
    Config serverArguments;

    std::shared_ptr<spdlog::logger> logger;
    TCPServer *tcpServer;


    void handleNewConnection();
    void forwardMessage();
};


#endif //RAWSERVER_COMMUNICATION_H
