#include "RPCserver.h"
#include "RPCclient.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include "MesManage.h"
#include "service.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <pthread.h>
#include "ServerDiscoveryClient.h"

#define SIZE 1024

using namespace std;

class ThreadParam {
public:
    ThreadParam(int port, char* group, int servicePort, int serviceType) : port(port), servicePort(servicePort), serviceType(serviceType)
    {
        strncpy(this->group, group, BUFSIZE);
    };
    int port;  // port of discovery server
    int servicePort; // the port that supply service
    int serviceType; // sevice type
    char group[BUFSIZE];
};

vector<string> split(const string &s, char delim){
    vector<string> elems;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)){
        elems.push_back(item);
    }
    return elems;
}

void* RPCServer(void* arg)
{
    ThreadParam &param = *(ThreadParam *)arg;
    
    int servicePort = param.servicePort;
    int serviceType = param.serviceType;
    delete &param;
    RPCserver server(servicePort, serviceType);
    
    while ( true ) {
        server.replyRPCall();
    }
    
    return NULL;
}

void* DiscoveryServer(void* arg)
{
    char group[BUFSIZE];
    ThreadParam &param = *(ThreadParam *)arg;
    int port = param.port;
    int servicePort = param.servicePort;
    int serviceType = param.serviceType;
    strncpy(group, param.group, BUFSIZE);
    delete &param;
    
    UDPserver udpserver(port);
    
    udpserver.getMultiCasetServerSocket(group);
    while ( true ) {
        char buf[BUFSIZE];
        int size = BUFSIZE;
        sockaddr_in src_addr;
        socklen_t src_addrlen = sizeof(src_addr);
        
        if(udpserver.recv(buf, size, &src_addr, &src_addrlen))
        {
        
            DiscoveryMes discoveryMessage;
            unmarshal_discovery_message(buf, discoveryMessage);
            cout<<"receive message from :" << inet_ntoa(src_addr.sin_addr) <<endl;
            cout<<"recevie message requesttype: " << discoveryMessage.serviceType <<endl;
            
            if(discoveryMessage.serviceType == serviceType || serviceType == both)
            {
                cout<<"service type match, reply" <<endl;
                char resultBuf[BUFSIZE];
                DiscoveryRes discoveryResult;
                discoveryResult.servicePort = servicePort;
                discoveryResult.serviceType = serviceType;
                marshal_discovery_result(discoveryResult, resultBuf);
        
                udpserver.sendBackto(resultBuf, sizeof(discoveryResult), &src_addr, &src_addrlen);
            }
        }
    }
    
    return NULL;
}

int main(int argc, char * argv[]) {
    if (argc != 4) {
        cerr << "usage: port host serviceType|filename" << endl;
        cerr << "example:"<<endl;
        cerr << "5003 239.255.255.255 0 to boot up server that only caculate midpoint" <<endl;
        cerr << "5003 239.255.255.255 1 to boot up server that only caculate distance" <<endl;
        cerr << "5003 239.255.255.255 2 to boot up server that caculate both" <<endl;
        cerr << "5003 239.255.255.255 file.txt to boot up client" <<endl;
        return -1;
    }

    int port = atoi( argv[1] );

    char *host = argv[2];
    
    int serviceType=2;
    char* filename = NULL;
    
    if(strncmp(argv[3], "0", 1) == 0|| strncmp(argv[3], "1", 1) ==0 || strncmp(argv[3], "2", 1) ==0)
    {
        serviceType = atoi(argv[3]);
    }
    else
    {
        filename = argv[3];
    }
    
    if ( filename != NULL ) { // client
        // do caculation
        RPCclient client(host, port, 5, MAX_RETRIES);
        ifstream myFile;
        myFile.open(filename);
        string line;
        cout.width(20); cout << left << "P1";
        cout.width(20); cout << left << "P2";
        cout.width(20); cout << left << "distance";
        cout.width(20); cout << left << "midpoint";
        cout << endl;
        
        while(getline(myFile, line)){
            vector<string> numbers = split(line, ' ');
            Point p1;
            Point p2;
            p1.x = stof(numbers[0]);
            p1.y = stof(numbers[1]);
            p2.x = stof(numbers[2]);
            p2.y = stof(numbers[3]);
            float dis;
            Point p3;
            int cartresult = cartesian_distance(&client, &p1, &p2, &dis);
            int midpointresult = midpoint_server(&client, &p1, &p2, &p3);
            
            
            if(cartresult == -1)
            {
                dis = -100.0;
            }
               
            if(midpointresult == -1)
            {
                p3.x = -100.0;
                p3.y = -100.0;
            }
            
            client.print(p1, p2, dis, p3);
            sleep(5);
        }
        myFile.close();
         
    }
    else {                   // server
        
        int servicePort = port+1;
        pthread_t rpcServer;
        ThreadParam *param1 = new ThreadParam(port, host, servicePort, serviceType);
        
        pthread_create(&rpcServer, NULL, RPCServer, (void*)param1);
        
        ThreadParam *param2 = new ThreadParam(port, host, servicePort, serviceType);
        pthread_t discoverServer;
        pthread_create(&discoverServer, NULL, DiscoveryServer, (void*)param2);
        
        pthread_join(discoverServer, NULL);
        pthread_join(rpcServer, NULL);
        
    }
    
}
