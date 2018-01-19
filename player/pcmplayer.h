#ifndef PCMPLAYER_H
#define PCMPLAYER_H
#include "alsahandle.h"
#include "siglelist.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"

#define PCMPLAYERFRAMSIZE 4096

enum Pcmplayer_status{
    Pcmplayer_start=0,
    Pcmplayer_finish,
    Pcmplayer_begain,
    Pcmplayer_stop
};


class Pcmplayer:public Poco::Runnable
{
public:
    Pcmplayer();
    ~Pcmplayer();
    r_status fillaudiodata(char *buf,int size);
    r_status start();
    r_status finish();
    Pcmplayer_status returnstatus();


protected:
    void virtual run();
    Poco::Thread thread;

private:
    LinkList *list;
    AlsaHandle audio;

    Pcmplayer_status playflag;
};

#endif // PCMPLAYER_H
