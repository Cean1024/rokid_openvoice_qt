#ifndef PLAYER_H
#define PLAYER_H
#include "alsahandle.h"
#include "mp3decode.h"
#include "siglelist.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"


enum playstatus {
    stop_play =0,
    start_play
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

    LinkList list;

protected:
    void virtual run();
    Poco::Thread thread;

private:

    bool flag;
    void *data;
    next_cb next_func;
    struct audiodata data_d;
    AlsaHandle audio;
    mp3decode mp3;

};

#endif // PLAYER_H
