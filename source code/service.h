

#ifndef service_h
#define service_h

#include "RPCclient.h"
#include "MesManage.h"



int cartesian_distance(RPCclient* rpc, Point *p1, Point* p2, float* result)
{
    return rpc->call_remote_cartesian_distance(p1, p2, result);
}

int midpoint_server(RPCclient* rpc, Point *p1, Point* p2, Point* result)
{
    return rpc->call_remote_midpoint(p1, p2, result);
}

#endif /* service_h */
