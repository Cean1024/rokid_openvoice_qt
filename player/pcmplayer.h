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
    Pcmplayer_resume,
    Pcmplayer_pause,
    Pcmplayer_waitfinish
};


class Pcmplayer:public Poco::Runnable
{
public:
    Pcmplayer();
    ~Pcmplayer();
    r_status fillaudiodata(char *buf,int size);
    r_status start();
    r_status finish();
    r_status waitfinish();
    r_status resume();
    r_status pause();
    Pcmplayer_status returnstatus();


protected:
    void virtual run();
    Poco::Thread thread;

private:
    LinkList *list;
    AlsaHandle audio;

    Pcmplayer_status playflag;
    Pcmplayer_status resume_pause_storage;
};

#endif // PCMPLAYER_H
