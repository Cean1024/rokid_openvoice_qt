#ifndef HTTPDL_H
#define HTTPDL_H
#include "common.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/Runnable.h"
#include "player/siglelist.h"

typedef r_status (*httpdlcb)(char *dldata,void *outdata);

enum httpdl_status{
    dl_start,
    dl_stop
};

class Httpdl:public Poco::Runnable
{
public:
    Httpdl();
    ~Httpdl();
    r_status HttpGetFile(const std::string& uri, const std::string& fileName, std::string& mime);
    r_status RegistCb(httpdlcb func_cb,void *outdata);
    r_status HttpGetFile(std::string &url, std::__cxx11::string &mime);
    r_status setUrl(std::string &url);
    void start();
    void stop();
protected:
    void virtual run();

private:
    void *outdata;
    httpdlcb handledl;
    std::string url;
    Poco::Thread thr;
    httpdl_status dl_sta;

};

#endif // HTTPDL_H
