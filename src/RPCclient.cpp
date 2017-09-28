#include "RPCclient.h"
#include <iomanip>
#include <sstream>

RPCclient::RPCclient(char host[], int port, int timeout, int max) : distanceDiscoveryClient(host, port), midpointDiscoveryClient(host,port)
{
    maxRetry = max;
    transactionId = 1;
    this->timeout = timeout;
    this->DiscoverDistanceServer();
    this->DiscoverMidpointServer();
}

int RPCclient::getTransId()
{
    return transactionId++;
}

int RPCclient::call_remote_cartesian_distance(Point* p1, Point* p2, float *result)
{
    SendMes message;
    message.transId = transactionId;
    message.code = dist;
    message.p1.x = p1->x;
    message.p1.y = p1->y;
    message.p2.x = p2->x;
    message.p2.y = p2->y;

    char buf[BUFSIZE];
    marshal_message(message, buf);
    udp_distance.sendto(buf, sizeof(message));
    char buffer[BUFSIZE];
    bool isRecvSuccess = false;
    int retryCount = 0;
    while(retryCount < this->maxRetry + 1){
        if(udp_distance.recvResult(buffer, sizeof(buffer))){
            isRecvSuccess = true;
            break;
        }
        else{
            retryCount++;
            if(retryCount< this->maxRetry+1){
                cout<<"retry " <<retryCount<< " times" << endl;
                
                if(this->DiscoverDistanceServer())
                {
                    cout<<"server recovered, send message again." <<endl;
                    udp_distance.sendto(buf, sizeof(message));
                }
            }
        }
    }
    if(isRecvSuccess){
        DistRes reply;
        unmarshal_dist_result(buffer, reply);
        *result = reply.result;
        return 0;
    }
    else
    {
        cout<<"after retry " << this->maxRetry << " times, recv is failed" << endl;
        return -1;
    }
}

int RPCclient::call_remote_midpoint(Point* p1, Point* p2, Point* presult){
    SendMes message;
    message.transId = transactionId;
    message.code = midPoint;
    message.p1.x = p1->x;
    message.p1.y = p1->y;
    message.p2.x = p2->x;
    message.p2.y = p2->y;

    char buf[BUFSIZE];
    marshal_message(message, buf);
    udp_midpoint.sendto(buf, sizeof(message));
    char buffer[BUFSIZE];
    bool isRecvSuccess = false;
    
    // retry logic
    int retryCount = 0;
    while(retryCount < this->maxRetry+1)
    {
        if(udp_midpoint.recvResult(buffer, sizeof(buffer)))
        {
            isRecvSuccess =true;
            break;
        }
        else
        {
            retryCount++;
            if(retryCount< this->maxRetry+1){
                cout<<"retry " <<retryCount<< " times" << endl;
                if(this->DiscoverMidpointServer())
                {
                    cout<<"server recovered, send message again." <<endl;
                    udp_midpoint.sendto(buf, sizeof(message));
                }
            }
        }
    }
    
    if(isRecvSuccess)
    {
        MidpointRes reply;
        unmarshal_midpoint_result(buffer, reply);
        presult->x = reply.p1.x;
        presult->y = reply.p1.y;
        return 0;
    }
    else
    {
        cout<<"after retry " << this->maxRetry << " times, recv is failed" << endl;
        return -1;
    }
}

void RPCclient::print(Point p1, Point p2, float result, Point p3){
    cout.width(20); cout << left << this->PointToString(p1);
    cout.width(20); cout << left << this->PointToString(p2);
    cout.width(20); cout << left << result;
    cout.width(20); cout << left << this->PointToString(p3);
    cout<<endl;
}

string RPCclient::PointToString(Point point)
{
    std::stringstream stream;
    
    stream<< "(" << point.x << "," << point.y << ")";
    
    return stream.str();
}

bool RPCclient::DiscoverDistanceServer()
{
    sockaddr_in src_addr;
    socklen_t src_addrlen = sizeof(src_addr);
    int servicePort = 0;
    int serviceType = dist;
    if(distanceDiscoveryClient.DiscoverServer(&src_addr, &src_addrlen, servicePort, serviceType))
    {
       char* serverHost = inet_ntoa(src_addr.sin_addr);
    
       udp_distance.SetupClient(serverHost, servicePort, this->timeout);
       udp_distance.getClientSocket();
        
        return true;
    }
    
    return false;
}

bool RPCclient::DiscoverMidpointServer()
{
    sockaddr_in src_addr;
    socklen_t src_addrlen = sizeof(src_addr);
    int servicePort = 0;
    int serviceType = midPoint;
    if(midpointDiscoveryClient.DiscoverServer(&src_addr, &src_addrlen, servicePort, serviceType))
    {
        char* serverHost = inet_ntoa(src_addr.sin_addr);
        
        udp_midpoint.SetupClient(serverHost, servicePort, this->timeout);
        udp_midpoint.getClientSocket();
        
        return true;
    }
    
    return false;
}