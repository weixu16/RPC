#include "MesManage.h"

char* marshal_int(char* buffer, int value)
{
    buffer[0] = (value >> 24) ;
    buffer[1] = (value >> 16) ;
    buffer[2] = (value >> 8) ;
    buffer[3] = value ;
    return buffer+4;
}

/*
 copy from book: Beej's Guide to Network Programming
 http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
 */
long int unpacki32(unsigned char *buf)
{
    unsigned long int i2 = ((unsigned long int)buf[0]<<24) |
    ((unsigned long int)buf[1]<<16) |
    ((unsigned long int)buf[2]<<8)  |
    buf[3];
    long int i;
    
    // change unsigned numbers to signed
    if (i2 <= 0x7fffffffu) { i = i2; }
    else { i = -1 - (long int)(0xffffffffu - i2); }
    
    return i;
}

/*
 copy from book: Beej's Guide to Network Programming
 http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
 */
uint32_t htonf(float f)
{
    uint32_t p;
    uint32_t sign;
    
    if (f < 0) { sign = 1; f = -f; }
    else { sign = 0; }
    
    p = ((((uint32_t)f)&0x7fff)<<16) | (sign<<31); // whole part and sign
    p |= (uint32_t)(((f - (int)f) * 65536.0f))&0xffff; // fraction
    
    return p;
}

/*
 copy from book: Beej's Guide to Network Programming
 http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
 */
float ntohf(uint32_t p)
{
    float f = ((p>>16)&0x7fff); // whole part
    f += (p&0xffff) / 65536.0f; // fraction
    
    if (((p>>31)&0x1) == 0x1) { f = -f; } // sign bit set
    
    return f;
}

/*
 copy from book: Beej's Guide to Network Programming
 http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
 */
unsigned long int unpacku32(unsigned char *buf)
{
    return ((unsigned long int)buf[0]<<24) |
    ((unsigned long int)buf[1]<<16) |
    ((unsigned long int)buf[2]<<8)  |
    buf[3];
}

char* unmarshal_int(char* buffer, int *value)
{
    *value = (int)unpacki32((unsigned char *)buffer);
    
    return buffer+4;
}


char* marshal_float(char* buffer, float value)
{
    uint32_t p = htonf(value);
    
    buffer[0] = p >> 24;
    buffer[1] = p >> 16;
    buffer[2] = p >> 8;
    buffer[3] = p;
    return buffer+4;
}

char* unmarshal_float(char* buffer, float *value)
{
    unsigned int a = (unsigned int)unpacku32((unsigned char*)buffer);
    *value = ntohf(a);
    
    return buffer+4;
}

void marshal_message(SendMes message, char *buffer)
{
    buffer = marshal_int(buffer, message.transId);
    buffer = marshal_int(buffer, message.code);
    buffer = marshal_float(buffer, message.p1.x);
    buffer = marshal_float(buffer, message.p1.y);
    buffer = marshal_float(buffer, message.p2.x);
    buffer = marshal_float(buffer, message.p2.y);
}

void unmarshal_message(char* buffer, SendMes &message)
{
    buffer = unmarshal_int(buffer, &message.transId);
    buffer = unmarshal_int(buffer, &message.code);
    buffer = unmarshal_float(buffer, &message.p1.x);
    buffer = unmarshal_float(buffer, &message.p1.y);
    buffer = unmarshal_float(buffer, &message.p2.x);
    buffer = unmarshal_float(buffer, &message.p2.y);
}

void marshal_midpoint_result(MidpointRes result, char* buffer)
{
    buffer = marshal_int(buffer, result.transId);
    buffer = marshal_int(buffer, result.resCode);
    buffer = marshal_float(buffer, result.p1.x);
    buffer = marshal_float(buffer, result.p1.y);
}

void unmarshal_midpoint_result(char* buffer, MidpointRes &result)
{
    buffer = unmarshal_int(buffer, &result.transId);
    buffer = unmarshal_int(buffer, &result.resCode);
    buffer = unmarshal_float(buffer, &result.p1.x);
    buffer = unmarshal_float(buffer, &result.p1.y);
}

void marshal_dist_result(DistRes result, char* buffer)
{
    buffer = marshal_int(buffer, result.transId);
    buffer = marshal_int(buffer, result.resCode);
    buffer = marshal_float(buffer, result.result);
}

void unmarshal_dist_result(char* buffer, DistRes &result)
{
    buffer = unmarshal_int(buffer, &result.transId);
    buffer = unmarshal_int(buffer, &result.resCode);
    buffer = unmarshal_float(buffer, &result.result);
}

void marshal_discovery_message(DiscoveryMes result, char* buffer)
{
    buffer = marshal_int(buffer, result.serviceType);
}

void unmarshal_discovery_message(char* buffer, DiscoveryMes &result)
{
    buffer = unmarshal_int(buffer, &result.serviceType);
}


void marshal_discovery_result(DiscoveryRes result, char* buffer)
{
    buffer = marshal_int(buffer, result.servicePort);
    buffer = marshal_int(buffer, result.serviceType);
}

void unmarshal_discovery_result(char* buffer, DiscoveryRes &result)
{
    buffer = unmarshal_int(buffer, &result.servicePort);
    buffer = unmarshal_int(buffer, &result.serviceType);
}
