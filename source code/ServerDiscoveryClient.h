//
//  ServerDiscoveryClient.h
//  RPC
//
//  Created by weixu on 6/10/16.
//  Copyright © 2016 weixu. All rights reserved.
//

#ifndef ServerDiscoveryClient_h
#define ServerDiscoveryClient_h

#include <stdio.h>
#include <iostream>
#include "UDPclient.h"

class ServerDiscoveryClient {
public:
    ServerDiscoveryClient(char host[], int port);
    bool DiscoverServer(sockaddr_in* src_addr, socklen_t* src_addrlen, int& servicePort, int serviceType);
    
private:
    int port;
    char host[HOSTBUFSIZE];
    UDPclient udp;
};

#endif /* ServerDiscoveryClient_h */
