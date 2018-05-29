#include "pcmplayer.h"

Pcmplayer::Pcmplayer()
{
    list = new LinkList(PCMPLAYERFRAMSIZE);
    playflag = Pcmplayer_stop;
    thread.start(*this);
}

Pcmplayer::~Pcmplayer()
{
    delete list;
}
r_status Pcmplayer::start()
{
    if(playflag == Pcmplayer_stop) {
        LOGOUT("in Pcmplayer \n");
        audio.init(12000,2,16);

        playflag = Pcmplayer_start;
    }
}
r_status Pcmplayer::stop()
{
    if(playflag == Pcmplayer_start || playflag == Pcmplayer_finish) {
        LOGOUT("in Pcmplayer \n");
        playflag = Pcmplayer_stop;

        audio.stop();
    }
}
r_status Pcmplayer::finish()
{
    if(playflag == Pcmplayer_start  ) {
        LOGOUT("in Pcmplayer \n");
        playflag = Pcmplayer_finish;
        audio.stop();
    }
}

r_status Pcmplayer::waitfinish()
{
    if(playflag == Pcmplayer_start ) {
        LOGOUT("in Pcmplayer \n");
        playflag = Pcmplayer_waitfinish;
        while(playflag != Pcmplayer_stop)usleep(10000);
        audio.stop();
    }
}


r_status Pcmplayer::resume()
{
    if( playflag == Pcmplayer_pause) {
        LOGOUT("in Pcmplayer \n");
        audio.init(12000,2,16);
        playflag = resume_pause_storage;
    }
}

r_status Pcmplayer::pause()
{
    if( playflag !=  Pcmplayer_pause && playflag != Pcmplayer_stop ) {
        LOGOUT("in Pcmplayer \n");
        resume_pause_storage = playflag;
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

    LOGOUT("in Pcmplayer \n");

    while(1) {

        while ( playflag == Pcmplayer_stop) {usleep(100000);}
        switch(playflag) {
        case Pcmplayer_resume:
        case Pcmplayer_start:{
            ret = list->get(&node);
            if(ret == SUCCESS ) {
                audio.writei(node->buf,node->size/4);
                list->Release(node);
            }else  usleep(10000);

        };break;
        case Pcmplayer_pause:{
            usleep(10000);
        };break;
        case Pcmplayer_finish:{playflag = Pcmplayer_stop;list->clean();}break;
        case Pcmplayer_waitfinish: {
            while(1) {

                if( playflag == Pcmplayer_pause ) {usleep(10000);continue;}
                ret = list->get(&node);
                if(ret == SUCCESS ) {
                    audio.writei(node->buf,node->size/4);
                    list->Release(node);
                }
                else { sleep(1); playflag = Pcmplayer_stop;break;}

            }
        };break;
        }
    }

    LOGOUT("out Pcmplayer \n");

}
