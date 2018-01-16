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

class Player:public Poco::Runnable
{
public:
    Player();
    ~Player();

    r_status start();
    r_status stop();
    LinkList list;
    struct audiodata data_d;
    AlsaHandle audio;
    mp3decode mp3;
protected:
    void virtual run();

private:


    Poco::Thread thread;


};

#endif // PLAYER_H
