#include "httpdl.h"

Httpdl::Httpdl()
{

}

Httpdl::~Httpdl()
{

}

int Httpdl::HttpGetFile(const std::string& uri, const std::string& fileName, std::string& mime)
{
    Poco::URI objUri;
    Poco::Net::HTTPClientSession objHttpCliSess;
    Poco::Net::HTTPRequest objHttpReq;
    Poco::Net::HTTPResponse objHttpRes;
    int size = 0;

    objUri = uri;

    objHttpReq.setMethod(Poco::Net::HTTPRequest::HTTP_GET);
    objHttpReq.setURI(objUri.getPathAndQuery());
    objHttpReq.setHost(objUri.getHost());

    objHttpCliSess.setHost(objUri.getHost());
    objHttpCliSess.setPort(objUri.getPort());
    objHttpCliSess.sendRequest(objHttpReq);

    std::istream& objHttpResIs = objHttpCliSess.receiveResponse(objHttpRes);
    mime = objHttpRes.getContentType();
    std::streamsize ss = objHttpRes.getContentLength();
    if (ss == Poco::Net::HTTPMessage::UNKNOWN_CONTENT_LENGTH) {
        throw Poco::ProtocolException(uri);
    }

    std::ofstream objFileOs(fileName.c_str(), std::ios::binary);
    if (objFileOs.fail()) {
        throw Poco::CreateFileException(fileName);
    }

    while (ss-- > 0 && !objHttpResIs.eof()) {
        int dat = objHttpResIs.get();
        objFileOs.put(dat);
        size ++;
    }

    return size;
}
