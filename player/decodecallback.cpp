#include <mad.h>
#include "mp3decode.h"
#include "alsahandle.h"
#include "player.h"
#include "siglelist.h"

static char streamtmpbuf[MEMPOOLBUFSIZED];

enum mad_flow input(void *data,
                    struct mad_stream *stream)
{

    struct audiodata *data_d = (struct audiodata *)data;
    listnode_d *gnode;

    while(1) {
        if ( data_d->playflag == start_play || \
             data_d->playflag == resume_play ) {

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
            }
            if(siglelist::getInstance()->gethttpdlstatus() \
                    == httpdl_finish) break;
            usleep(100000);
        } else if(data_d->playflag == pause_play) usleep(100000);
        else if(data_d->playflag == stop_play) break;
    }

    return MAD_FLOW_STOP;
}

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


//time_t timetick;
static char * pbuf2 = NULL;
static char databuf2[4608];
static audioparam params2;
static unsigned int count2 = 0;
enum mad_flow output(void *data,
                     struct mad_header const *header,
                     struct mad_pcm *pcm)
{
    unsigned int nchannels, nsamples;
    mad_fixed_t const *left_ch, *right_ch;

    struct audiodata *data_d = (struct audiodata *)data;
    /* pcm->samplerate contains the sampling frequency */




    nchannels = pcm->channels;
    nsamples  = pcm->length;
    left_ch   = pcm->samples[0];
    right_ch  = pcm->samples[1];
    int size = nsamples * nchannels * 2;

    pbuf2=databuf2;

    if(pcm->channels != params2.CHANNELS  || \
            header->samplerate != params2.RATE ) {
        count2++;
        if( count2 > 10 ) {
            count2 = 0;
            DEBUG("count :%u  channels :%d\n",count2,pcm->channels);
            params2.CHANNELS = pcm->channels;
            params2.RATE = header->samplerate ;
            //data_d->audio_addr->init(params2);
        }
    }

    unsigned short samples;

    switch(nchannels) {
    case 2:
        while (nsamples--) {
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
    data_d->audio_addr->writei(databuf2,1152);

    return MAD_FLOW_CONTINUE;
}
