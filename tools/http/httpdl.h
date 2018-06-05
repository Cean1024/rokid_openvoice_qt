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

class Httpdl
{
public:
    Httpdl();
    ~Httpdl();

    r_status httpdownload();
    r_status setUrl(std::string &url);

protected:
    virtual r_status handlehttpdl(char *dldata,int len) =0;
    r_status HttpGetFile(std::string &url, std::__cxx11::string &mime);
    r_status HttpGetFile(const std::string& uri, const std::string& fileName, std::string& mime);

private:
    std::string url;
    httpdl_status dl_sta;

};

#endif // HTTPDL_H
