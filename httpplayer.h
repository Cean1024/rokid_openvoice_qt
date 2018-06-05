#ifndef HTTPPLAYER_H
#define HTTPPLAYER_H
#include "common.h"
#include "tools/http/httpdl.h"
#include "player/player.h"

class HttpPlayer:public Httpdl,public Poco::Runnable,public Player
{
public:
    HttpPlayer();
    ~HttpPlayer();
    void start(std::string url);
    void stop();

protected:
    void run();
    r_status handlehttpdl(char *dldata,int len);


private:
    Poco::Thread thr;
    Poco::Thread thrdl;
};

#endif // HTTPPLAYER_H
