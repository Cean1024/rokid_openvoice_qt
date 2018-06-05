#include "netserver.h"

netserver::netserver()
{
    welcomeSocket =-1;
    newSocket =-1;
    initserver(LOCALIP,PORT);
}

netserver::netserver(std::__cxx11::string ip, int port)
{
    welcomeSocket =-1;
    newSocket =-1;
    initserver(ip,port);
}

netserver::~netserver()
{

}

r_status netserver::initserver(std::string ip,int port) {

  int ret;


  welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);
  if(welcomeSocket<0){perror("socket");return FAILED;}


  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;

  serverAddr.sin_port = htons(port);

  serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());

  memset(serverAddr.sin_zero, '0', sizeof serverAddr.sin_zero);

  /*---- Bind the address struct to the socket ----*/
  ret = bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
  if(ret<0){perror("bind");return FAILED;}\


  return SUCCESS;
}

r_status netserver::listenandaccept()
{
    if(welcomeSocket<0){return FAILED;}

    /*---- Listen on the socket, with 5 max connection requests queued ----*/
    if(listen(welcomeSocket,5) ==0 ) {LOGOUT("Listening\n");}
    else { printf("Error\n"); return FAILED;}

    /*---- Accept call creates a new socket for the incoming connection ----*/
    addr_size = sizeof serverStorage;
    newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
    if(newSocket <0)return FAILED;
    LOGOUT("client comming");

    return SUCCESS;
}

r_status netserver::listenandaccept(sockaddr_storage &client)
{
    if(welcomeSocket<0) {return FAILED;}
    /*---- Listen on the socket, with 5 max connection requests queued ----*/
    if(listen(welcomeSocket,5) ==0 ){ LOGOUT("Listening");}
    else{ printf("Error\n"); return FAILED;}

    /*---- Accept call creates a new socket for the incoming connection ----*/
    socklen_t addr_size = sizeof client;
    if(newSocket>0)close(newSocket);
    newSocket = accept(welcomeSocket, (struct sockaddr *) &client, &addr_size);
    if(newSocket <0)return FAILED;
    LOGOUT("client comming");

    return SUCCESS;
}

void netserver::closeclient()
{
    if(newSocket>0) close(newSocket);
    newSocket = -1;
}

r_status netserver::getdata(char *buf,int len)
{
    if(newSocket<0){return FAILED;}
    return recv(newSocket,buf,len,0);
}

r_status netserver::senddata(char *buf,int len)
{
    if(newSocket<0){return FAILED;}
    /*---- Send message to the socket of the incoming connection ----*/
    return send(newSocket,buf,len,0);
}
