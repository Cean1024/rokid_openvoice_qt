#include "pcmplayer.h"

Pcmplayer::Pcmplayer()
{
    list = new LinkList(PCMPLAYERFRAMSIZE);
    playflag = Pcmplayer_finish;
}

Pcmplayer::~Pcmplayer()
{
    delete list;
}
r_status Pcmplayer::start()
{
    if(playflag == Pcmplayer_finish) {
        audio.init(12000,2,16);
        if(!thread.isRunning())thread.join();
        thread.start(*this);
        playflag = Pcmplayer_start;
    }
}

r_status Pcmplayer::finish()
{
    if(playflag == Pcmplayer_start  ) {
        playflag = Pcmplayer_finish;
        thread.join();
        audio.stop();
    }
}
r_status Pcmplayer::waitfinish()
{
    if(playflag == Pcmplayer_start ) {
        playflag = Pcmplayer_waitfinish;
        thread.join();
        audio.stop();
    }
}


r_status Pcmplayer::resume()
{
    if( playflag == Pcmplayer_pause) {
        audio.init(12000,2,16);
        playflag = resume_pause_storage;
    }
}

r_status Pcmplayer::pause()
{
    if( playflag !=  Pcmplayer_pause && playflag != Pcmplayer_finish ) {
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

    DEBUG("in Pcmplayer %s\n",__func__);

    while(1) {

        if( playflag == Pcmplayer_finish) break;
        switch(playflag) {
        case Pcmplayer_start:{
            ret = list->get(&node);
            if(ret == SUCCESS ){
                audio.writei(node->buf,node->size/4);
                list->Release(node);
            }
            else usleep(10000);

        };break;
        case Pcmplayer_pause:{
            usleep(10000);
        };break;

        case Pcmplayer_waitfinish: {
            while(1) {

                if( playflag == Pcmplayer_pause ) {usleep(10000);continue;}
                ret = list->get(&node);
                if(ret == SUCCESS ) {
                    audio.writei(node->buf,node->size/4);
                    list->Release(node);
                }
                else { playflag = Pcmplayer_finish;break;}

            }
        };break;
        }
    }
    list->clean();
    sleep(1);
    DEBUG("out Pcmplayer %s\n",__func__);

}
