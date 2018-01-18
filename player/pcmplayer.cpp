#include "pcmplayer.h"

Pcmplayer::Pcmplayer()
{
    list = new LinkList(PCMPLAYERFRAMSIZE);
}

Pcmplayer::~Pcmplayer()
{
    delete list;
}
r_status Pcmplayer::start()
{
     playflag = Pcmplayer_start;
     audio.init(12000,2,16);
     thread.start(*this);
}

r_status Pcmplayer::finish()
{
    playflag = Pcmplayer_finish;
}

r_status Pcmplayer::fillaudiodata(char *buf,int size)
{
    listnode_d * node ;
    while( size >= PCMPLAYERFRAMSIZE ) {
        node = list->CreateNode();
        memcpy(node->buf,buf,PCMPLAYERFRAMSIZE);
        node->size = PCMPLAYERFRAMSIZE;
        list->Insert(node);
        buf+=PCMPLAYERFRAMSIZE;
        size -=PCMPLAYERFRAMSIZE;
    }
    if(size>0) {
        node = list->CreateNode();
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
    while(1) {

        ret =list->get(&node);
        if(ret == SUCCESS )
            audio.writei(node->buf,node->size/4);
        else if( playflag == Pcmplayer_start) usleep(100000);
        else if( playflag == Pcmplayer_finish) break;
    }
}
