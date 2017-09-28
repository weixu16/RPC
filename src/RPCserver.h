
#ifndef Server_h
#define Server_h

#include "UDPserver.h"
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
#include <stdio.h>
#include <math.h>
#include "MesManage.h"

using namespace std;


class RPCserver {
public:
    RPCserver(int port, int serviceType);

    void replyRPCall();
    
private:
    int port;
    opCode serviceType;
    UDPserver udp;
    int cartesian_distance_server (Point *p1, Point* p2, float* result);
    int midpoint_server(Point *p1, Point* p2, Point* result);
};

#endif
