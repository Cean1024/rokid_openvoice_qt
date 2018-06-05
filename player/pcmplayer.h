#ifndef PCMPLAYER_H
#define PCMPLAYER_H
#include "alsahandle.h"
#include "siglelist.h"
#include "net/netudpbase.h"

#define PCMPLAYERFRAMSIZE 4096
#define HALFSIZE 2048
#define HALFOFHALF 1024
#define PCMPLAYSAMPLE 24000
#define PCMPLAYCH 2
enum Pcmplayer_status{
    Pcmplayer_start=0,
    Pcmplayer_stop,
    Pcmplayer_finish,
    Pcmplayer_resume,
    Pcmplayer_pause,
    Pcmplayer_waitfinish
};


class Pcmplayer
{
public:
    Pcmplayer();
    ~Pcmplayer();
    r_status fillaudiodata(char *buf,int size);
    r_status init_pcmplayer();
    r_status finish_pcmplayer();
    r_status resume_pcmplayer();
    r_status pause_pcmplayer();
    Pcmplayer_status returnstatus();
    void fillfinish() {
        fillflag = false;
    }


protected:
    void play();

private:
    LinkList *list;
    AlsaHandle audio;
    Pcmplayer_status playflag;
    bool fillflag;
    NetUdpBase udpobj;
};

#endif // PCMPLAYER_H
