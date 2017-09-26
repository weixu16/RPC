//
//  ServerDiscoveryClient.c
//  RPC
//
//  Created by weixu on 6/10/16.
//  Copyright © 2016 weixu. All rights reserved.
//

#include "ServerDiscoveryClient.h"
#include "MesManage.h"

using namespace std;

ServerDiscoveryClient::ServerDiscoveryClient(char host[], int port) : udp(host, port, 3)
{
    strcpy(this->host, host);
    udp.getClientSocket();
}

bool ServerDiscoveryClient::DiscoverServer(sockaddr_in* src_addr, socklen_t* src_addrlen, int& servicePort, int serviceType)
{
    cout<<"Start discover server..." <<endl;
    
    char buffer[64];
    DiscoveryMes discoveryMessage;
    discoveryMessage.serviceType = serviceType;
    marshal_discovery_message(discoveryMessage, buffer);

    udp.sendto(buffer, sizeof(discoveryMessage));
    cout<<"send message to "<< this->host <<endl;
    
    char recvbuffer[64];
    
    if(udp.recv(recvbuffer, 64, src_addr, src_addrlen))
    {
        DiscoveryRes discoveryResult;
        unmarshal_discovery_result(recvbuffer, discoveryResult);
    
        cout<<"receive message from :" << inet_ntoa(src_addr->sin_addr) <<endl;
        cout<<"receive message body : servicePort is " << discoveryResult.servicePort <<endl;
        cout<<"receive message body : serviceType is " << discoveryResult.serviceType <<endl;
        
        servicePort = discoveryResult.servicePort;
        return true;
    }
    
    cerr<<"no server respond, discover failed"<<endl;
    return false;
}