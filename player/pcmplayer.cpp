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
r_status Pcmplayer::start()
{
    if(playflag == Pcmplayer_stop) {
        audio.init(12000,2,16);
        if(!thread.isRunning())thread.join();
        thread.start(*this);
        playflag = Pcmplayer_start;
    }
}

r_status Pcmplayer::finish()
{
    if(playflag == Pcmplayer_start) {
        playflag = Pcmplayer_finish;
        while ( playflag != Pcmplayer_stop ) usleep(10000);
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
    while( size >= PCMPLAYERFRAMSIZE ) {
        node = list->CreateNode();
        if( node ) {
            memcpy(node->buf,buf,PCMPLAYERFRAMSIZE);
            node->size = PCMPLAYERFRAMSIZE;
            list->Insert(node);
        }
        buf+=PCMPLAYERFRAMSIZE;
        size -=PCMPLAYERFRAMSIZE;
    }
    if(size>0) {
        node = list->CreateNode();
        if(node==nullptr)return FAILED;
        memcpy(node->buf,buf,size);
        node->size = size;
        list->Insert(node);
    }

    return SUCCESS;
}


void Pcmplayer::run()
{
    listnode_d * node;
    int ret;

    DEBUG("in Pcmplayer %s\n",__func__);


    while(1) {

        ret =list->get(&node);
        if(ret == SUCCESS )
            audio.writei(node->buf,node->size/4);
        else if( playflag == Pcmplayer_start) usleep(100000);
        else if( playflag == Pcmplayer_finish) break;
    }
    sleep(1);
    playflag = Pcmplayer_stop;

    DEBUG("out Pcmplayer %s\n",__func__);
    audio.stop();
}
