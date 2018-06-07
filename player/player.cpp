#include "player.h"

Player::Player()
{

    next_func = NULL;
    data_d.playflag = stop_play;
    data_d.f_sta = data_fill_init;
    mp3 = new mp3decode;
}

Player::Player(next_cb func,void *data)
{

    data_d.playflag = stop_play;
    data_d.f_sta = data_fill_init;
    mp3 = new mp3decode;
    next_func = func;
    this->data = data;

}
Player::~Player()
{
    delete mp3;
}

r_status Player::init_player()
{

    if( data_d.playflag == stop_play  ) {
        LOGOUT("in Player \n");
        data_d.audio_addr = &audio;
        data_d.list_addr = &list;
        data_d.udpobj = &udpobj;
        audio.init(44100,2,16);

        data_d.playflag = start_play;
        data_d.f_sta = data_filling;

        mp3->registe_callback(inputHandle,outputHandle,(void *)&data_d);
        decodeflag =true;
        data_d.disinfo=true;
   }
   return SUCCESS;
}
r_status Player::finish_player()
{

    if( data_d.playflag != stop_play ) {
        LOGOUT("in Player \n");
        data_d.playflag = stop_play;
        data_d.f_sta = data_fill_finish;
        while(decodeflag)usleep(10000);
        audio.stop();
        list.clean();


    }
    return SUCCESS;
}


r_status Player::pause_player()
{
    if( data_d.playflag == start_play  ) {
        LOGOUT("in Player \n");
        data_d.playflag = pause_play;
        usleep(100000);
        audio.stop();
    }
}
r_status Player::resume_player()
{
    if ( data_d.playflag == pause_play ) {
        LOGOUT("in Player \n");
        audio.init(44100,2,16);
        data_d.playflag = start_play;
    }
}

r_status Player::fillaudiodata(char *buf,int size)
{
    listnode_d * node = list.CreateNode();
    if( node == nullptr ) return FAILED;
    if(size > node->size)size = node->size;
    memcpy(node->buf,buf,size);
    list.Insert(node);
    return SUCCESS;
}

enum mad_flow Player::inputHandle(void *data,
                    struct mad_stream *stream)
{
    struct audiodata *data_d = (struct audiodata *)data;
    listnode_d *gnode;
    static char streamtmpbuf[MEMPOOLBUFSIZED];

    while(1) {
        if ( data_d->playflag == start_play ) {

            int ret = data_d->list_addr->get(&gnode);
            if(ret != ERROR ) {

                if( stream->error == MAD_ERROR_BUFLEN ) {
                    int remainbytes = stream->bufend - stream->next_frame;
                    //DEBUG("有不完整 PCM 数据帧,%d \n",remainbytes);
                    memcpy(streamtmpbuf,stream->next_frame,remainbytes);
                    memcpy(streamtmpbuf+remainbytes,gnode->buf,gnode->size);
                    gnode->size += remainbytes;
                    mad_stream_buffer(stream, (unsigned char *)streamtmpbuf, gnode->size);
                } else mad_stream_buffer(stream, (unsigned char *)gnode->buf, gnode->size);

                data_d->list_addr->Release(gnode);

                return MAD_FLOW_CONTINUE;
            } else if(data_d->f_sta == data_fill_finish) break;
            usleep(100000);
        } else if(data_d->playflag == pause_play) usleep(100000);
        else if(data_d->playflag == stop_play) break;
    }

    return MAD_FLOW_STOP;
}
enum mad_flow Player::outputHandle(void *data,
                     struct mad_header const *header,
                     struct mad_pcm *pcm)
{

    static char databuf2[4608];
    static udppackage package;
    char * pbuf2 = databuf2;

    unsigned int nchannels, nsamples;
    mad_fixed_t const *left_ch, *right_ch;

    static struct audiodata *data_d = (struct audiodata *)data;


    /* pcm->samplerate contains the sampling frequency */
    nchannels = pcm->channels;
    nsamples  = pcm->length;
    left_ch   = pcm->samples[0];
    right_ch  = pcm->samples[1];
    int size = nsamples * nchannels * 2;
    if( data_d->disinfo ) {
        LOGOUT("samplerate:%d , ch:%d",pcm->samplerate,pcm->channels)
        data_d->disinfo =false;
        package.samplerate = pcm->samplerate;
        package.ch=pcm->channels;
        package.size = size;
        data_d->audio_addr->init(pcm->samplerate,pcm->channels,16);
    }
    unsigned short samples;

    switch(nchannels) {
    case 2:
        while (nsamples--) {
/*
            samples= scale(*left_ch);
            *pbuf1++ = samples & 0xff;
            *pbuf1++ = (samples >>8) &0xff;
*/
            samples= scale(*left_ch++);
            *pbuf2++ = samples & 0xff;
            *pbuf2++ = (samples >> 8) &0xff;
            samples= scale(*right_ch++);
            *pbuf2++ = samples & 0xff;
            *pbuf2++ = (samples >>8) &0xff;
        }
        break;
    case 1:
        while (nsamples--) {
            samples= scale(*left_ch++);
            *pbuf2++ = samples & 0xff;
            *pbuf2++ = (samples >>8) &0xff;
        }
        break;
    default:
        break;
    }

    while (data_d->playflag == pause_play ) {

            usleep(100000);

    }
    memcpy(package.buf,databuf2,size);
    data_d->udpobj->senddata(package);
    data_d->audio_addr->writei(databuf2,1152);

    return MAD_FLOW_CONTINUE;
}
r_status Player::play()
{
    LOGOUT("play run\n");

    int ret = mp3->decode();

    LOGOUT("decode ret:%d",ret);
    decodeflag = false;
    //DEBUG("play thread stop ,ret:%d\n",ret);
    /*
    int count = PLAYWAITINGTIME;
    while ( data_d.playflag == start_play && count >0 ) {
        sleep(1);
        count--;
    }
    if(data_d.playflag == start_play || data_d.playflag == resume_play )
        if(next_func) next_func(data);
    */
    return ret;
}
