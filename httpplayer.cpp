#include "httpplayer.h"

HttpPlayer::HttpPlayer()
{

}
HttpPlayer::~HttpPlayer()
{

}
void Download(void *data);

void HttpPlayer::start(std::string url)
{

    setUrl(url);
    init_player();
    thrdl.start(Download,this);
    thr.start(*this);
}

void HttpPlayer::stop()
{
    if(thrdl.isRunning())thrdl.join();
    finish_player();
    if(thr.isRunning())thr.join();
}

r_status HttpPlayer::handlehttpdl(char *dldata, int len)
{
    fillaudiodata(dldata,len);
    return SUCCESS;
}
void Download(void *data)
{
    HttpPlayer *hplayer = (HttpPlayer *)data;
    hplayer->httpdownload();
    hplayer->fillfinish();
}
void HttpPlayer::run()
{
    play();
    finish_player();
}
