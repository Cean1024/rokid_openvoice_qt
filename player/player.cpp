#include "player.h"
#include "decodecallback.cpp"
Player::Player()
{
    flag = true;
    next_func = NULL;
    data_d.playflag = stop_play;
}

Player::Player(next_cb func,void *data)
{
    flag = true;

    data_d.playflag = stop_play;

    next_func = func;
    this->data = data;

}
Player::~Player()
{

}

r_status Player::start()
{

    if(flag) {
        DEBUG("in Player %s\n",__func__);
        data_d.audio_addr = &audio;
        data_d.list_addr = &list;
        audio.init(44100,2,16);
        mp3.registe_callback(input,output);
        data_d.playflag = start_play;
        thread.start( *this );
       flag = !flag;
   }
    return SUCCESS;
}
r_status Player::stop()
{

   if(!flag) {
        DEBUG("in Player %s\n",__func__);
        data_d.playflag = stop_play;
        thread.join();
        audio.stop();
        list.clean();

       flag =!flag;
    }
    return SUCCESS;
}


r_status Player::pause()
{
    if( data_d.playflag == start_play \
            || data_d.playflag == resume_play ) {
        data_d.playflag = pause_play;
        usleep(100000);
        //audio.stop();
    }
}
r_status Player::resume()
{
    if( data_d.playflag == pause_play ) {
        //audio.init(44100,2,16);
        data_d.playflag = resume_play;
    }
}
r_status Player::fillaudiodata(char *buf,int size)
{
    listnode_d * node = list.CreateNode();
    memcpy(node->buf,buf,node->size);
    list.Insert(node);
    return SUCCESS;
}

void Player::run()
{
    DEBUG("play thread run\n");

    int ret = mp3.decode((void *)&data_d);

    //DEBUG("play thread stop ,ret:%d\n",ret);
    int count = PLAYWAITINGTIME;
    while (data_d.playflag == start_play && count >0 ){
        sleep(1);
        count--;
    }
    if(data_d.playflag == start_play || data_d.playflag == resume_play)
        if(next_func) next_func(data);

}
