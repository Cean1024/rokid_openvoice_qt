#include "httpdl.h"

Httpdl::Httpdl()
{
    dl_sta = dl_stop;
}

Httpdl::~Httpdl()
{

}
void Httpdl::start()
{

   if(dl_sta == dl_stop) {
        LOGOUT("in Httpdl::start");
        thr.start(*this);
        dl_sta = dl_start;
   }
}
void Httpdl::stop()
{

    if( dl_sta == dl_start ) {
        LOGOUT("in Httpdl::stop");
        if(thr.isRunning());
        {
            pthread_t t_id = thr.tid();
            pthread_cancel(t_id);
            //thr.join();
        }


        dl_sta = dl_stop;
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
    LOGOUT("in");
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
        LOGPROCESS("dl proccess :%f%%",size*100.0 /ss);
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
        LOGPROCESS("dl proccess :%f%%",size*100.0 /ss);
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
    LOGOUT("start downloading");
    siglelist::getInstance()->sethttpdlstatus(httpdl_start);
    HttpGetFile(url,mime);
    siglelist::getInstance()->sethttpdlstatus(httpdl_finish);
    LOGOUT("\ndownloading finish mime:%s",mime.c_str());
}
