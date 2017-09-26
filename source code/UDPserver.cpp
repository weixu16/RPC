#include "UDPserver.h"

UDPserver::UDPserver(int port): port(port){
    
}

UDPserver::~UDPserver(){
    if(serverSd != NULL_SD)
    {
        close(serverSd);
    }
}


int UDPserver::getServerSocket(){
    if((serverSd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        perror("socket");
        exit(NULL_SD);
    }
    
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    
    if(::bind(serverSd, (struct sockaddr*)&addr, sizeof(addr)) < 0){
        perror("bind");
        return NULL_SD;
    }
    
    return serverSd;
}


bool UDPserver::recv(char* buf, int size, sockaddr_in *src_addr, socklen_t *addrlen){
    bzero(buf, size);
    //struct sockaddr src_addr;
    //socklen_t src_addrlen = sizeof(*src_addr);
    bzero( (char *)src_addr, *addrlen );
    if ( recvfrom( serverSd, buf, size, 0, (struct sockaddr *)src_addr, addrlen ) < 0 ) {
        perror( "recvfrom" );
        return false;
    }
    return true;
}

bool UDPserver::sendBackto(char* buf, size_t size, sockaddr_in *src_addr, socklen_t* addrlen){
    if ( ::sendto( serverSd, buf, size, 0, (struct sockaddr *)src_addr, *addrlen) < 0) {
        perror( "sendto" );
        return false;
    }
    return true;
}

int UDPserver::getMultiCasetServerSocket(char* group ) {
    // create what looks like an ordinary UDP socket
    if ( ( serverSd = socket( AF_INET, SOCK_DGRAM, 0 ) ) < 0 ) {
        perror( "socket" );
        exit( NULL_SD );
    }
    // set up destination address
    struct sockaddr_in addr;
    bzero( &addr, sizeof( addr ) );
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl( INADDR_ANY );
    addr.sin_port=htons( port );
    
    // bind to receive address
    if ( ::bind( serverSd,( struct sockaddr * )&addr, sizeof( addr ) ) < 0 ) {
        perror( "bind" );
        return NULL_SD;
    }
        
    // use setsockopt() to request that the kernel join a multicast group
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr=inet_addr( group );
    mreq.imr_interface.s_addr=htonl( INADDR_ANY );
    if ( setsockopt( serverSd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                    &mreq, sizeof( mreq ) ) < 0 ) {
        perror( "setsockopt" );
        return NULL_SD;
    }
    return serverSd;
}























