//
// Created by Jakub Trubka on 21.08.19.
//

#include "TCPServer.h"

TCPServer::TCPServer() {
    logger = spdlog::get("Raw_server");
}

TCPServer::~TCPServer() {
    close(serverSocketNumber);
    close(clientSocketNumber);
}

void TCPServer::startTCPServer(int port) {
    this->port = port;
    initializeServer();
    bindToPort();
}

void TCPServer::bindToPort() {
    logger->info("Binding to port {0}", port);

    if (bind(serverSocketNumber, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        logger->error("Binding failed");
        exit(EXIT_FAILURE);
    }
    if ((listen(serverSocketNumber, 1)) < 0)
    {
        logger->error("Listening failed");
        exit(EXIT_FAILURE);
    }
}

TCPMessage TCPServer::receiveMessage(){
    close(serverSocketNumber);

    if(encryption){
        SSL *ssl;
        ssl = SSL_new(ctx);              /* get new SSL state with context */
        SSL_set_fd(ssl, clientSocketNumber);

        if ( SSL_accept(ssl) == -1 ) {    /* do SSL-protocol accept */
            logger->error("SSL connection error");
            exit(EXIT_FAILURE);
        }
        else
        {

            char str[INET6_ADDRSTRLEN];
            if(inet_ntop(AF_INET6, &client.sin6_addr, str, sizeof(str))) {
                logger->info("New connection from {0}:{1}", str, ntohs(client.sin6_port));
            }
            //ShowCerts(ssl);        /* get any certificates */
            SSL_read(ssl, &binaryMessage, sizeof(TCPMessage) - sizeof(uint8_t *)); /* get request */
            logger->info("UID: {0:08x}, timestamp: {1}, duration: {2}, channelMask: {3:04b}, data dataSize: {4}", binaryMessage.uid, binaryMessage.timestamp, binaryMessage.duration, binaryMessage.channelMask, binaryMessage.dataSize);//binaryMessage.stampSize);


            binaryMessage.data = new uint8_t[binaryMessage.dataSize];
            SSL_read(ssl, binaryMessage.data, binaryMessage.dataSize); /* get request */
        }
        SSL_free(ssl);         /* release SSL state */
        close(clientSocketNumber);
    }else{
        char str[INET6_ADDRSTRLEN];

        if(inet_ntop(AF_INET6, &client.sin6_addr, str, sizeof(str))) {
            logger->info("New connection from {0}:{1}", str, ntohs(client.sin6_port));
        }
        int rec;

        rec = recv(clientSocketNumber, &binaryMessage, sizeof(TCPMessage) - sizeof(uint8_t *), MSG_WAITALL);
        if(rec != sizeof(TCPMessage) - sizeof(uint8_t *)){
            logger->error("Message to short (insufficient header length)");
            exit(EXIT_FAILURE);
        }

        logger->info("UID: {0:08x}, timestamp: {1}, duration: {2}, channelMask: {3:04b}, data dataSize: {4}", binaryMessage.uid, binaryMessage.timestamp, binaryMessage.duration, binaryMessage.channelMask, binaryMessage.dataSize);//binaryMessage.stampSize);

        binaryMessage.data = new uint8_t[binaryMessage.dataSize];

        rec = recv(clientSocketNumber, binaryMessage.data, binaryMessage.dataSize, MSG_WAITALL);
        if(rec != binaryMessage.dataSize){
            logger->error("Message to short (insufficient data length of {0}, expected {1})", rec, binaryMessage.dataSize);
            exit(EXIT_FAILURE);
        }
    }

    return binaryMessage;
}

void TCPServer::waitForConnection() {
    close(clientSocketNumber);
    clientSocketNumber = 0;

    while(clientSocketNumber <= 0){
        clientSocketNumber = accept(serverSocketNumber, (struct sockaddr*)&client, &clientLen);
    }
}

void TCPServer::initializeServer() {
    if ((serverSocketNumber = socket(PF_INET6, SOCK_STREAM, 0)) < 0)
    {
        logger->error("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    memset(&server,0,sizeof(server));
    server.sin6_family = AF_INET6;
    server.sin6_addr = in6addr_any;
    server.sin6_port = htons(port);
    clientLen = sizeof(client);
    SSL_library_init();
    ctx = InitServerCTX();
    LoadCertificates(ctx, "/home/nothrax/Documents/bp/sslcert.pem", "/home/nothrax/Documents/bp/sslcert.pem");
}

SSL_CTX* TCPServer::InitServerCTX()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();  /* load & register all cryptos, etc. */
    SSL_load_error_strings();   /* load all error messages */
    method = TLSv1_2_server_method();  /* create new server-method instance */
    ctx = SSL_CTX_new(method);   /* create new context from method */
    if ( ctx == nullptr )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
void TCPServer::LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile)
{
    /* set the local certificate from CertFile */
    if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* set the private key from KeyFile (may be the same as CertFile) */
    if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* verify private key */
    if ( !SSL_CTX_check_private_key(ctx) )
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}
void TCPServer::ShowCerts(SSL* ssl)
{
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl); /* Get certificates (if available) */
    if ( cert != nullptr )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);
        X509_free(cert);
    }
    else
        printf("No certificates.\n");
}
