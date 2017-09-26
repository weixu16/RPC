
#ifndef RPC_H
#define RPC_H
#include "UDPclient.h"
#include "MesManage.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <sys/time.h>
#include "ServerDiscoveryClient.h"
#include "MesManage.h"

#define MAX_RETRIES 3

using namespace std;


class RPCclient {
public:
    RPCclient(char host[], int port, int timeout, int max);

    int call_remote_cartesian_distance(Point* p1, Point* p2, float *result);
    void print(Point p1, Point p2, float result, Point p3);
    int call_remote_midpoint(Point* p1, Point* p2, Point* presult);
    int getTransId();
private:
    int port;
    char host[HOSTBUFSIZE];
    UDPclient udp_distance;
    UDPclient udp_midpoint;
    int transactionId;
    int maxRetry;
    int timeout;
    string PointToString(Point point);
    ServerDiscoveryClient distanceDiscoveryClient;
    ServerDiscoveryClient midpointDiscoveryClient;
    
    bool DiscoverDistanceServer();
    bool DiscoverMidpointServer();
};
    
#endif /* RPC_h */
    
