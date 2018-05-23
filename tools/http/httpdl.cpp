#include "httpdl.h"

Httpdl::Httpdl()
{
    flag =true;
}

Httpdl::~Httpdl()
{

}
void Httpdl::start()
{

   if(flag) {
        DEBUG("in Httpdl %s\n",__func__);
        thr.start(*this);
        flag = !flag;
   }
}
void Httpdl::stop()
{

    if( !flag ) {
        DEBUG("in Httpdl %s\n",__func__);
        while(thr.isRunning())usleep(100000);
        thr.join();
        flag = !flag;
    }
}

r_status Httpdl::RegistCb(httpdlcb func_cb,void *outdata)
{

    this->handledl = func_cb;
    this->outdata = outdata;
    return SUCCESS;
}

r_status Httpdl::HttpGetFile(std::string &url,std::string &mime)
{
    DEBUG("in Httpdl %s\n",__func__);
    Poco::URI objUri;
    Poco::Net::HTTPClientSession objHttpCliSess;
    Poco::Net::HTTPRequest objHttpReq;
    Poco::Net::HTTPResponse objHttpRes;
    int size = 0;

    objUri = url;

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
        throw Poco::ProtocolException(url);
    }

    char buf[MEMPOOLBUFSIZE];

    while (size < ss && !objHttpResIs.eof()) {

        objHttpResIs.read(buf,(ss-size > MEMPOOLBUFSIZE ) ? MEMPOOLBUFSIZE:(ss-size));
        if(this->handledl(buf,outdata) != SUCCESS)LOGOUT(" handledl err!!");
        size += MEMPOOLBUFSIZE;
        printf("dl proccess :%f%%\r",size*100.0 /ss);
    }
}

r_status Httpdl::HttpGetFile(const std::string& uri, const std::string& fileName, std::string& mime)
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
    char buf[1024];


    while (size < ss && !objHttpResIs.eof()) {

        objHttpResIs.read(buf,(ss-size >1024 )? 1024:(ss-size));
        objFileOs.write(buf,(ss-size >1024 )? 1024:(ss-size));
        size +=1024;
        printf("dl proccess :%f%%\r",size*100.0 /ss);
    }

    return size;
}

r_status Httpdl::setUrl(std::string &url)
{
    this->url = url;
    return SUCCESS;
}

void Httpdl::run()
{
    std::string mime;
    siglelist::getInstance()->sethttpdlstatus(httpdl_start);
    HttpGetFile(url,mime);
    siglelist::getInstance()->sethttpdlstatus(httpdl_finish);
}
