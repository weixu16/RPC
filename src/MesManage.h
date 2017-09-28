
#ifndef message_h
#define message_h
#include <stdint.h>

#define BUFSIZE 64

typedef struct s_Point{
    float x;
    float y;
} Point;


enum opCode {midPoint, dist, both};

typedef struct send_message {
    int transId;
    int code;
    Point p1;
    Point p2;
}SendMes;

typedef struct midPoint_result{
    int transId;
    int resCode;
    Point p1;
}MidpointRes;

typedef struct dist_result {
    int transId;
    int resCode;    
    float result;
}DistRes;

typedef struct Discovery_message{
    int serviceType;
} DiscoveryMes;

typedef struct serverDiscovery_result{
    int servicePort;
    int serviceType;
} DiscoveryRes;

    char* marshal_int(char* buffer, int value);
    long int unpacki32(unsigned char *buf);
    uint32_t htonf(float f);
    float ntohf(uint32_t p);
    unsigned long int unpacku32(unsigned char *buf);
    char* unmarshal_int(char* buffer, int *value);
    char* marshal_float(char* buffer, float value);
    char* unmarshal_float(char* buffer, float *value);
    void marshal_message(SendMes message, char *buffer);
    void unmarshal_message(char* buffer, SendMes &message);
    void marshal_midpoint_result(MidpointRes result, char* buffer);
    void unmarshal_midpoint_result(char* buffer, MidpointRes &result);
    void marshal_dist_result(DistRes result, char* buffer);
    void unmarshal_dist_result(char* buffer, DistRes &result);


    void marshal_discovery_message(DiscoveryMes result, char* buffer);
    void unmarshal_discovery_message(char* buffer, DiscoveryMes &result);
    void marshal_discovery_result(DiscoveryRes result, char* buffer);
    void unmarshal_discovery_result(char* buffer, DiscoveryRes &result);


#endif
