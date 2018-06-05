#include "pcmplayer.h"

Pcmplayer::Pcmplayer()
{
    list = new LinkList(PCMPLAYERFRAMSIZE);
    playflag = Pcmplayer_stop;

}

Pcmplayer::~Pcmplayer()
{
    delete list;
}
r_status Pcmplayer::init_pcmplayer()
{
    if(playflag == Pcmplayer_stop) {
        LOGOUT("in Pcmplayer \n");
        audio.init(PCMPLAYSAMPLE,PCMPLAYCH,16);

        playflag = Pcmplayer_start;
        fillflag =true;
    }
}
r_status Pcmplayer::finish_pcmplayer()
{
    if(playflag == Pcmplayer_start) {
        LOGOUT("in Pcmplayer \n");
        playflag = Pcmplayer_stop;

        audio.stop();
    }
}

r_status Pcmplayer::resume_pcmplayer()
{
    if( playflag == Pcmplayer_pause) {
        LOGOUT("in Pcmplayer \n");
        audio.init(PCMPLAYSAMPLE,PCMPLAYCH,16);
        playflag = Pcmplayer_start;
    }
}

r_status Pcmplayer::pause_pcmplayer()
{
    if( playflag ==  Pcmplayer_start ) {
        LOGOUT("in Pcmplayer \n");

        playflag = Pcmplayer_pause;
        audio.stop();
    }
}

Pcmplayer_status Pcmplayer::returnstatus()
{
    return playflag;
}

r_status Pcmplayer::fillaudiodata(char *buf,int size)
{
    listnode_d * node ;
    static short buf_ch2[PCMPLAYERFRAMSIZE];
    short *p = (short *)buf_ch2;
    short *p_r= (short *)buf;
    while( size >= HALFSIZE ) {
        node = list->CreateNode();
        if( node ) {
            /*update 1ch to 2ch*/
            for(int i=0;i<HALFOFHALF;i++) {
                *p++ =*p_r;
                *p++=*p_r++;
            }
            memcpy(node->buf,buf_ch2,PCMPLAYERFRAMSIZE);
            node->size = PCMPLAYERFRAMSIZE;
            list->Insert(node);
        }
        size -=HALFSIZE;
        p = buf_ch2;
    }
    if(size>0) {
        node = list->CreateNode();
        if(node==nullptr)return FAILED;

        /*update 1ch to 2ch*/
        for(int i=0;i<size/2;i++) {
            *p++ =*p_r;
            *p++=*p_r++;
        }
        memcpy(node->buf,buf_ch2,size*2);
        node->size = size*2;
        list->Insert(node);
    }

    return SUCCESS;
}


void Pcmplayer::play()
{
    listnode_d * node;
    int ret;

    bool loop=true;
    LOGOUT("in Pcmplayer \n");
    udppackage package;
    package.samplerate=PCMPLAYSAMPLE;
    package.ch=PCMPLAYCH;
    while(loop) {
        switch(playflag) {
        case Pcmplayer_start:{
            ret = list->get(&node);
            if(ret == SUCCESS ) {
                memcpy(package.buf,node->buf,node->size);
                package.size=node->size;
                udpobj.senddata(package);
                audio.writei(node->buf,node->size/4);
                list->Release(node);
            } else if( fillflag ) usleep(10000);
            else loop=false;

        };break;
        case Pcmplayer_pause:{
            usleep(10000);
        };break;
        case Pcmplayer_stop:{
            loop=false;
        };
        }
    }

    LOGOUT("out Pcmplayer \n");

}
