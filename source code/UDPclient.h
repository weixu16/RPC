#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <iostream>          // cerr
#include <sys/types.h>       // socket
#include <sys/socket.h>      // socket
#include <netinet/in.h>      // inet_addr
#include <arpa/inet.h>       // inet_addr
#include <strings.h>         // bzero, strncpy
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

//using namespace std;

#define NULL_SD -1
#define HOSTBUFSIZE 1024

class UDPclient {
public:
    UDPclient();
    UDPclient(char host[], int port, int timeout);
    ~UDPclient();
    
    void SetupClient(char host[], int port, int timeout);
    int getClientSocket();
    bool sendto(char* buf, size_t size);
    bool recvResult(char* buf, int size);
    bool recv(char* buf, int size, sockaddr_in *src_addr, socklen_t *addrlen);
    
private:
    int clientSd;
    int port;
    int timeoutInSec;
    char host[HOSTBUFSIZE];
};

#endif
