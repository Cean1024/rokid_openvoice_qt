#ifndef PLAYER_H
#define PLAYER_H
#include "alsahandle.h"
#include "mp3decode.h"
#include "siglelist.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"


enum playstatus {
    stop_play =0,
    start_play,
    pause_play,
    resume_play
};

struct audiodata {
    LinkList *list_addr;
    AlsaHandle *audio_addr;
    playstatus playflag; /*0 stop 1 play*/
};

typedef void (*next_cb)(void *data);

class Player:public Poco::Runnable
{
public:
    Player();
    Player(next_cb func,void *data);
    ~Player();


    r_status start();
    r_status stop();
    r_status pause();
    r_status resume();

    r_status fillaudiodata(char *buf,int size);



protected:
    void virtual run();
    Poco::Thread thread;

private:
    struct audiodata data_d;
    AlsaHandle audio;
    mp3decode *mp3;
    LinkList list;

    bool flag;
    void *data;
    next_cb next_func;

};

#endif // PLAYER_H
