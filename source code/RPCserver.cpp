#include "RPCserver.h"
#include "MesManage.h"

RPCserver::RPCserver(int port, int serviceType) : udp(port){
    this->serviceType = (opCode)serviceType;
    udp.getServerSocket();
}

void RPCserver::replyRPCall(){
    
    char buf[BUFSIZE];
    int size = BUFSIZE;
    sockaddr_in src_addr;
    socklen_t src_addrlen = sizeof(src_addr);

    udp.recv(buf, size, &src_addr, &src_addrlen);
    
    SendMes mess;
    unmarshal_message(buf, mess);
    
    if(mess.code != this->serviceType && this->serviceType != both)
    {
        cout<<"service "<<mess.code << " is not supported" <<endl;
        return;
    }
    
    if(mess.p1.x < 0 || mess.p2.x < 0)
    {
        cerr << "point must be greater than 0" << endl;
        return ;
    }
    if(mess.code == dist)
    {
        float result;
        DistRes reply;
        
        reply.transId = mess.transId;
        if(this->cartesian_distance_server(&mess.p1, &mess.p2, &result) <0)
        {
            reply.resCode = -1;
            reply.result = -1;
        }
        else
        {
            reply.resCode = 0;
            reply.result = result;
        }
        char bufresult[BUFSIZE];
        marshal_dist_result(reply, bufresult);
        udp.sendBackto(bufresult, sizeof(reply), &src_addr, &src_addrlen);
    }
    if(mess.code == midPoint)
    {
        Point result;
        MidpointRes reply;
        reply.transId = mess.transId;
        if(this->midpoint_server(&mess.p1, &mess.p2, &result) <0)
        {
            reply.resCode = -1;
            reply.p1.x = -1;
            reply.p1.y = -1;
        }
        else{
            reply.resCode = 1;
            reply.p1.x = result.x;
            reply.p1.y = result.y;
        }

        char buf[BUFSIZE];
        marshal_midpoint_result(reply, buf);
        udp.sendBackto(buf, sizeof(reply), &src_addr, &src_addrlen);
    }
    return;
}

int RPCserver::cartesian_distance_server (Point *p1, Point* p2, float* result)
{
    
    if( (p1->x > 0 && p1->y < 0 ) || (p2->x < 0  && p2->y >0))
    {
        cerr << "p1 or p2 is in the lower right" <<endl;
        return -1;
    }
    
    *result = sqrt(pow((p1->x - p2->x),2) + pow((p1->y - p2->y),2));
    return 0;
}

int RPCserver::midpoint_server(Point *p1, Point* p2, Point* result)
{
    if( (p1->x > 0 && p1->y < 0 ) || (p2->x < 0  && p2->y >0))
    {
        cerr << "p1 or p2 is in the lower right" <<endl;
        return -1;
    }
    
    
    result->x= (p1->x + p2->x)/2;
    result->y= (p1->y + p2->y)/2;
    
    return 0;
}
