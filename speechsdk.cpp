#include "speechsdk.h"

SpeechSdk::SpeechSdk()
{
    handleresult=NULL;
    voice_id =-1;
    sh_status = handle_end;
    speech = nullptr;
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
    switch(flag) {
    case voice_data:speech->put_voice(voice_id,(uint8_t *)buf,len);break;

    case voice_start: {
        if(voice_id > 0) {
            LOGOUT("voice_id is not empty, please end or cancal it first");
            speech->cancel(voice_id);
            //return FAILED;
        }
        //voiceOptions = new speech::VoiceOptions;
        //voice_id = speech->start_voice(voiceOptions);
        voice_id = speech->start_voice();
        sh_status = handle_start;
    };break;
    case voice_cancal:
        speech->cancel(voice_id);
    case voice_end: {
        speech->end_voice(voice_id);
        voice_id =-1;
        //delete voiceOptions;

    };break;
    }
    return SUCCESS;
}


int SpeechSdk::speech_init()
{
    if(speech) speech->release();
    sleep(1);
    speech = speech::Speech::new_instance();
    std::shared_ptr<speech::SpeechOptions> opts = speech::SpeechOptions::new_instance();
    opts->set_codec(speech::Codec::PCM);
    opts->set_lang(speech::Lang::ZH);
    opts->set_vad_mode(speech::VadMode::LOCAL);
    opts->set_no_intermediate_asr(true);

    speech->prepare(popts);
    speech->config(opts);
    //speech->put_text("若琪，来首音乐");

}
int SpeechSdk::init (rokid::speech::PrepareOptions &popts, ResponseHandle *jhandle)
{
    this->j_hd = jhandle;
    this->popts = popts;
    speech_init();

    thread.start(*this);
    //thread.join();//等待该线程技术
    return 0;
}
void SpeechSdk::Handle_speech_result(speech::SpeechResult &Result)
{
    switch (Result.type) {
    case speech::SPEECH_RES_ERROR: {
        LOGOUT("Handler speech--> err:%d",Result.err);
    };break;
    case speech::SPEECH_RES_INTER: {
        LOGOUT("Handler speech-->  SPEECH_RES_INTER");

    };break;
    case speech::SPEECH_RES_START: {
        LOGOUT("Handler speech-->  SPEECH_RES_START");

    };break;
    case speech::SPEECH_RES_CANCELLED:
    case speech::SPEECH_RES_END: {
        LOGOUT("Handler speech-->  SPEECH_RES_END");
        LOGOUT("action:%s",Result.action.c_str());
        LOGOUT("nlp:%s",Result.nlp.c_str());
        j_hd->analyses(Result.action);
    };break;

    case speech::SPEECH_RES_ASR_FINISH: {
        LOGOUT("Handler speech-->  SPEECH_RES_ASR_FINISH");
        LOGOUT("ASR:%s",Result.asr.c_str());
        LOGOUT("nlp:%s",Result.nlp.c_str());
    };break;
    }
}

void SpeechSdk::speech_run()
{
    speech::SpeechResult result;
    while (true) {
        if (!speech->poll(result)) {
            speech_init();
            LOGOUT("speech->poll failed, reinit speech");
        }
        // 处理result
        if(result.type == speech::SPEECH_RES_END ) sh_status = handle_processing;
        Handle_speech_result(result);
        if(result.type == speech::SPEECH_RES_END ) sh_status = handle_end;
        else if( result.type == speech::SPEECH_RES_ERROR ) sh_status = handle_err;
    }

}

void SpeechSdk::run()
{
    speech_run();
}

