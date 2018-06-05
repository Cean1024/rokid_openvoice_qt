#include "netudpbase.h"

NetUdpBase::NetUdpBase()
{
    sa = new Poco::Net::SocketAddress(ECHO_UDP_IP,ECHO_UDP_PORT);
    dgs = new Poco::Net::DatagramSocket(Poco::Net::IPAddress::IPv4);

    dgs->connect(*sa);

}
NetUdpBase::~NetUdpBase()
{
    delete sa;
    delete dgs;
}
void NetUdpBase::senddata(char *buf,int len)
{
    try{
        dgs->sendBytes(buf,len);
    }catch (std::exception &e){
        LOGOUT("%s",e.what());
    }
}
void NetUdpBase::senddata(udppackage &package)
{
    try{
        dgs->sendBytes((char *)&package,sizeof (udppackage));
    }catch (std::exception &e) {
        LOGOUT("%s",e.what());
    }
}
