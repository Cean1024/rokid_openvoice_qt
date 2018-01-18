#include "mp3decode.h"


mp3decode::mp3decode()
{

}

mp3decode::~mp3decode()
{

}

/*
 * This is the function called by main() above to perform all the decoding.
 * It instantiates a decoder object and configures it with the input,
 * output, and error callback functions above. A single call to
 * mad_decoder_run() continues until a callback function returns
 * MAD_FLOW_STOP (to stop decoding) or MAD_FLOW_BREAK (to stop decoding and
 * signal an error).
 */

r_status mp3decode::decode(void *data)
{

    int result;

    /* configure input, output, and error functions */

    mad_decoder_init( &decoder, data , inputfunc , 0 /* header */, 0 /* filter */,
                     outputfunc, 0, 0 /* message */);

    /* start decoding */
    result = mad_decoder_run (&decoder, MAD_DECODER_MODE_SYNC);

    mad_decoder_finish(&decoder);
    return result;
}
void mp3decode::release()
{
    /* release the decoder */

}

r_status mp3decode::registe_callback(callback_input_func inputf,callback_out_func outputf)
{
    if( !outputfunc || ! inputfunc )
        return ERROR;
    this->outputfunc = outputf;
    this->inputfunc = inputf;
    return SUCCESS;
}


