#include "UDPclient.h"


UDPclient::UDPclient()
{
    
}
UDPclient::UDPclient(char host[], int port, int timeout) : clientSd(NULL_SD), port(port), timeoutInSec(timeout){
    strncpy(this->host, host, HOSTBUFSIZE);
}

UDPclient::~UDPclient(){
    if(clientSd != NULL_SD)
    {
        close(clientSd);
    }
}

void UDPclient::SetupClient(char host[], int port, int timeout)
{
    this->port = port;
    this->timeoutInSec = timeout;
    strncpy(this->host, host, HOSTBUFSIZE);
}

int UDPclient::getClientSocket(){
    if ( ( clientSd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ) < 0 ) {
        perror( "socket" );
        return NULL_SD;
    }

    struct timeval timeout;
    timeout.tv_sec = timeoutInSec;
    timeout.tv_usec = 500000;
    if(setsockopt(clientSd, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&timeout,sizeof(struct timeval)) < 0){
        perror( "setsockopt" );
        return NULL_SD;
    }
    return clientSd;
}

bool UDPclient::sendto(char* buf, size_t size){
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_port = htons(port);
    if ( ::sendto( clientSd, buf, size, 0, (struct sockaddr *) &addr,
                sizeof( addr ) ) < 0) {
        perror( "sendto" );
        return false;
    }
    return true;
}

bool UDPclient::recvResult(char* buf, int size){
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_port = htons(port);

    socklen_t src_addrlen = sizeof(addr);
    if (recvfrom(clientSd, buf, size, 0, (struct sockaddr *)&addr, &src_addrlen) < 0 ) {
        perror( "recvfrom" );
        return false;
    }
    return true;
}


bool UDPclient::recv(char* buf, int size, sockaddr_in *src_addr, socklen_t *addrlen){
    bzero(buf, size);
    //struct sockaddr src_addr;
    //socklen_t src_addrlen = sizeof(*src_addr);
    bzero( (char *)src_addr, *addrlen );
    if ( recvfrom( clientSd, buf, size, 0, (struct sockaddr *)src_addr, addrlen ) < 0 ) {
        perror( "recvfrom" );
        return false;
    }
    return true;
}
