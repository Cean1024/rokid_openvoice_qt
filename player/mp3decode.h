#ifndef MP3DECOD_H
#define MP3DECOD_H
#include "include/mad.h"
#include "common.h"

typedef enum mad_flow ( * callback_out_func )(void *data,struct mad_header const *header,struct mad_pcm *pcm);
typedef enum mad_flow (*callback_input_func)(void *data, struct mad_stream *stream);
struct buffer {
    unsigned char const *start;
    unsigned long length;
};


class mp3decode
{
public:
    mp3decode();
    ~mp3decode();
    r_status registe_callback(callback_input_func inputf,callback_out_func outputf);
    r_status decode(void *data);


private:
    callback_out_func outputfunc;
    callback_input_func inputfunc;
    struct mad_decoder decoder;
    void release();

};

#endif // MP3DECOD_H
