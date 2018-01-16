#include "player.h"
#include "decodecallback.cpp"
Player::Player()
{

}

Player::~Player()
{

}

r_status Player::start()
{
    data_d.audio_addr = &audio;
    data_d.list_addr = &list;
    audio.init(44100,2,16);
    mp3.registe_callback(input,output);
    thread.start( *this );

}
r_status Player::stop()
{
    data_d.playflag = stop_play;
    thread.join();
    audio.stop();
}

void Player::run()
{
    DEBUG("play thread run\n");
    data_d.playflag = start_play;
    usleep(100000);
    int ret = mp3.decode((void *)&data_d);
    DEBUG("play thread stop ,ret:%d\n",ret);
}
