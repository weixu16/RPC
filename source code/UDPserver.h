#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <iostream>          // cerr
#include <sys/types.h>       // socket
#include <sys/socket.h>      // socket
#include <netinet/in.h>      // inet_addr
#include <arpa/inet.h>       // inet_addr
#include <strings.h>         // bzero, strncpy
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <netdb.h>

using namespace std;

#define NULL_SD -1

class UDPserver {
public:
    UDPserver(int port);
    ~UDPserver();
    int getServerSocket();
    bool recv(char* buf, int size, sockaddr_in *src_addr, socklen_t *addrlen);
    bool sendBackto(char* buf, size_t size, sockaddr_in *src_addr, socklen_t* addrlen);
    int getMultiCasetServerSocket(char* group);
private:
    int serverSd;
    int port;
};

#endif
