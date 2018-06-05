#ifndef NETUDPBASE_H
#define NETUDPBASE_H
#include "common.h"
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include <exception>

struct udppackage {
    int samplerate;
    int ch;
    int size;
    char buf[5760];
};

class NetUdpBase
{
public:
    NetUdpBase();
    ~NetUdpBase();
    void senddata(char *buf,int len);
    void senddata(udppackage &package);

protected:

private:
    Poco::Net::SocketAddress *sa;
    Poco::Net::DatagramSocket *dgs;


};

#endif // NETUDPBASE_H
