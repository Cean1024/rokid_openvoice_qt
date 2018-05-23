#include "speechsdk.h"

SpeechSdk::SpeechSdk()
{
    handleresult=NULL;
    voice_id =-1;
    sh_status = handle_end;
}

SpeechSdk::~SpeechSdk()
{

}

int SpeechSdk::speek(std::string strings)
{
    //"若琪你好"
    return speech->put_text(strings.c_str());
}
int SpeechSdk::speek_voice(char *buf,int len,voice_status flag)
{
    switch(flag){
    case voice_data:speech->put_voice(voice_id,(uint8_t *)buf,len);break;
    case voice_end:{
        speech->end_voice(voice_id);
        voice_id =-1;

    };break;
    case voice_start: {
        if(voice_id > 0) {
            LOGOUT("voice_id is not empty, please end or cancal it first");
            speech->cancel(voice_id);
            //return FAILED;
        }
        voice_id = speech->start_voice();
        sh_status = handle_start;
    };break;
    case voice_cancal:
        speech->cancel(voice_id);
        voice_id =-1;
        break;

    }
    return SUCCESS;
}


int SpeechSdk::speech_init(speech::PrepareOptions &popts)
{

    std::shared_ptr<speech::SpeechOptions> opts = speech::SpeechOptions::new_instance();
    opts->set_codec(speech::Codec::PCM);
    opts->set_lang(speech::Lang::ZH);
    opts->set_vad_mode(speech::VadMode::LOCAL);
    speech->config(opts);
    speech->prepare(popts);
    //speech->put_text("若琪，来首音乐");

}
int SpeechSdk::init (rokid::speech::PrepareOptions &popts, callback_speech_func func,void * data)
{
    handleresult = func;
    this->data = data;
    this->popts = popts;
    speech_init(popts);

    thread.start(*this);
    //thread.join();//等待该线程技术
    return 0;
}

void SpeechSdk::speech_run()
{
    speech::SpeechResult result;
    while (true) {
        if (!speech->poll(result))
            break;
        // 处理result
        if(result.type == speech::SPEECH_RES_END ) sh_status = handle_processing;
        handleresult(result,data);
        if(result.type == speech::SPEECH_RES_END ) sh_status = handle_end;
        else if( result.type == speech::SPEECH_RES_ERROR ) sh_status = handle_err;
    }
}

void SpeechSdk::run()
{
    speech_run();
}

