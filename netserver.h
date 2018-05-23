#ifndef NETSERVER_H
#define NETSERVER_H
#include "common.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 9999
#define LOCALIP "127.0.0.1"

class netserver
{
public:
    netserver();
    netserver(std::string ip,int port);
    ~netserver();

    r_status listenandaccept();
    r_status listenandaccept(sockaddr_storage &client);
    r_status getdata(char *buf,int len);
    r_status senddata(char *buf,int len);
    void closeclient();

protected:
    r_status initserver(std::string ip,int port);

private:
    int welcomeSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
};

#endif // NETSERVER_H
