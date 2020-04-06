//
// Created by Jakub Trubka on 21.08.19.
//

#ifndef RAWSERVER_TCPSERVER_H
#define RAWSERVER_TCPSERVER_H


#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <csignal>
#include "openssl/ssl.h"
#include "openssl/err.h"

#include "Structures.h"
#include "spdlog/spdlog.h"

class TCPServer {
public:
    TCPServer();
    ~TCPServer();
    void startTCPServer(int port);
    void waitForConnection();
    TCPMessage receiveMessage();
private:
    int serverSocketNumber;
    int clientSocketNumber;
    int port;
    TCPMessage binaryMessage;
    struct sockaddr_in6 server;
    struct sockaddr_in6 client;
    socklen_t clientLen;
    std::shared_ptr<spdlog::logger> logger;
    bool encryption = true;
    SSL_CTX *ctx;

    void bindToPort();
    void initializeServer();

    //ssl example from https://aticleworld.com/ssl-server-client-using-openssl-in-c/
    SSL_CTX* InitServerCTX();
    void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile);
    void ShowCerts(SSL* ssl);
};


#endif //RAWSERVER_TCPSERVER_H
