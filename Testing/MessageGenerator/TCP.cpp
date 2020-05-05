//
// Created by Jakub Trubka on 31.03.20.
//


#include "TCP.h"

TCP::TCP(Arguments arguments, uint32_t uid, uint32_t numberOfMessages){
    this->arguments = arguments;
    this->uid = uid;
    this->numberOfMessages = numberOfMessages;
}

void TCP::startSending() {
    createTCPMessage();
    waitTime = arguments.milliseconds*1000;
    uint32_t timeCounter = time(nullptr);
    while(true){
        sendTCPMessage();
        if(time(nullptr) - timeCounter > 30){
            timeCounter = time(nullptr);
            waitTime -=waitTime/10;
        }
        usleep(waitTime);
    }
}


void TCP::createTCPMessage() {
    tcpMessage.version = 1;
    tcpMessage.uid = uid;
    tcpMessage.timestamp = time(nullptr);
    tcpMessage.channelMask = 0b1111;
    tcpMessage.duration = TCP_LENGTH;
    tcpMessage.adcConstant = 0.00000286;
    tcpMessage.sensitivity[0] = 98.86;
    tcpMessage.sensitivity[1] = 98.86;
    tcpMessage.sensitivity[2] = 98.86;
    tcpMessage.sensitivity[3] = 98.86;
    tcpMessage.dataSize = sizeof(uint32_t)*128000*TCP_LENGTH*NUMBER_OF_SENSORS;
    tcpMessage.data = (uint8_t *)malloc(sizeof(uint32_t)*128000*TCP_LENGTH*NUMBER_OF_SENSORS);
}

void TCP::sendTCPMessage() {
    sock = -1;
    struct sockaddr_in server;
    long arg;
    fd_set fdset;
    struct timeval tv;
    SSL_CTX *ctx;
    SSL *ssl;
    SSL_library_init();
    ctx = InitCTX();

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Socket error\n";
        return;
    }

    //Debugprint printf("Socket	OK\n");
    server.sin_addr.s_addr = inet_addr(arguments.ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(arguments.port);

    connect(sock, (struct sockaddr *) &server, sizeof(server));

    if(encrypt){
        ssl = SSL_new(ctx);      /* create new SSL connection state */
        SSL_set_fd(ssl, sock);    /* attach the socket descriptor */
        if (SSL_connect(ssl) == -1){   /* perform the connection */
            std::cerr << "SSL error" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    if(encrypt){
        ShowCerts(ssl);        /* get any certs */
        SSL_write(ssl,&tcpMessage, sizeof(TCPMessage) - sizeof(u_int8_t *));
        SSL_write(ssl,tcpMessage.data, tcpMessage.dataSize);
    }else{
        write(sock, &tcpMessage, sizeof(TCPMessage) - sizeof(u_int8_t *));
        write(sock, tcpMessage.data, tcpMessage.dataSize);
    }


    tcpMessage.timestamp++;

    // Connection close
    if(encrypt){
        SSL_free(ssl);
        SSL_CTX_free(ctx);
    }
    close(sock);
}



SSL_CTX* TCP::InitCTX(){
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
    SSL_load_error_strings();   /* Bring in and register error messages */
    method = TLSv1_2_client_method();  /* Create new client-method instance */
    ctx = SSL_CTX_new(method);   /* Create new context */
    if ( ctx == nullptr )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }

}

void TCP::ShowCerts(SSL* ssl){
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if ( cert != nullptr )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);       /* free the malloc'ed string */
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);       /* free the malloc'ed string */
        X509_free(cert);     /* free the malloc'ed certificate copy */
    }
    else
        printf("Info: No client certificates configured.\n");

}