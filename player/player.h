#ifndef PLAYER_H
#define PLAYER_H
#include "alsahandle.h"
#include "mp3decode.h"
#include "siglelist.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "net/netudpbase.h"

enum playstatus {
    stop_play =0,
    start_play,
    pause_play,
    resume_play
};

enum fillstatus {
    data_fill_init,
    data_filling,
    data_fill_finish
};

struct audiodata {
    LinkList *list_addr;
    AlsaHandle *audio_addr;
    NetUdpBase *udpobj;
    playstatus playflag; /*0 stop 1 play*/
    fillstatus f_sta;
    bool disinfo;
};

typedef void (*next_cb)(void *data);


inline signed int scale(mad_fixed_t sample)
{
    /* round */
    sample += (1L << (MAD_F_FRACBITS - 16));

    /* clip */
    if (sample >= MAD_F_ONE)
        sample = MAD_F_ONE - 1;
    else if (sample < -MAD_F_ONE)
        sample = -MAD_F_ONE;

    /* quantize */
    return sample >> (MAD_F_FRACBITS + 1 - 16);
}


class Player
{
public:
    Player();
    Player(next_cb func,void *data);
    ~Player();


    r_status init_player();
    r_status finish_player();
    r_status pause_player();
    r_status resume_player();

    r_status fillaudiodata(char *buf,int size);
    void fillfinish() {
        data_d.f_sta = data_fill_finish;
    }
    playstatus getPlayStatus() {
        return data_d.playflag;
    }
    r_status play();

protected:
    static enum mad_flow outputHandle(void *data,
                         struct mad_header const *header,
                         struct mad_pcm *pcm);
    static enum mad_flow inputHandle(void *data,
                        struct mad_stream *stream);

private:
    struct audiodata data_d;
    AlsaHandle audio;
    mp3decode *mp3;
    LinkList list;
    NetUdpBase udpobj;

    bool decodeflag;

    void *data;
    next_cb next_func;

};

#endif // PLAYER_H
