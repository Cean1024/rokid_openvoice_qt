#ifndef HTTPDL_H
#define HTTPDL_H
#include "common.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"

class Httpdl
{
public:
    Httpdl();
    ~Httpdl();
    int HttpGetFile(const std::string& uri, const std::string& fileName, std::string& mime);

};

#endif // HTTPDL_H
