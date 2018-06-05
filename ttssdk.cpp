#include "ttssdk.h"

TtsSdk::TtsSdk()
{
    handleresult=NULL;
    voicehandle = vh_finish;
    tts = speech::Tts::new_instance();
    tts_id =0;
    httpplayer =nullptr;;
}

TtsSdk::~TtsSdk()
{
    if(tts)tts->release();
}
vh_status TtsSdk::speek(std::string strings)
{
    vh_status vhs;
    int times =  RETRY_TIME;
    //speek_start:

    LOGOUT("start tts speak");
    //if(tts_id >0 ) {tts->cancel(tts_id); tts_id =0;sleep(1);}
    set_vh_status(vh_start);
    tts_id = tts->speak(strings.c_str());
    LOGOUT("finish tts speak");

    return vhs;

}

void TtsSdk::reinit()
{
    if(tts)tts->release();
    tts = speech::Tts::new_instance();
    tts->prepare(popts);
    // 在prepare后任意时刻，都可以调用config修改配置
    std::shared_ptr<speech::TtsOptions> topts = speech::TtsOptions::new_instance();
    topts->set_codec(speech::Codec::PCM);
    //topts->set_samplerate(96000);
    tts->config(topts);
}

int TtsSdk::init( speech::PrepareOptions &popts)
{
    //handleresult = func;
    this->popts = popts;
    this->data = data;

    reinit();

    ptr.start(*this);
}

void TtsSdk::addplayer(HttpPlayer &player)
{
    this->httpplayer = &player;
}

void  TtsSdk::start_speak()
{
    httpplayer->pause_player();
    usleep(10000);
    init_pcmplayer();

}
void TtsSdk::stop_speak()
{
    play();
    sleep(1);
    finish_pcmplayer();
    httpplayer->resume_player();
}
void TtsSdk::filldata(char *buf,int size) {
    fillaudiodata(buf,size);
}

void TtsSdk::set_vh_status(vh_status vhs)
{
    voicehandle=vhs;
}
vh_status TtsSdk::get_vh_status()
{
    return voicehandle;
}

void TtsSdk::Handle_tts_result(speech::TtsResult &Result)
{

    switch (Result.type) {
    case speech::TTS_RES_ERROR: {
        LOGOUT("Handler tts--> err:%d",Result.err);

        this->set_vh_status(vh_err);

    };break;
    case speech::TTS_RES_VOICE: {
        LOGOUT("Handler tts--> voice fill");

        this->filldata(\
                    (char *)Result.voice->c_str(),\
                    Result.voice->size());

    };break;
    case speech::TTS_RES_START: {
        LOGOUT("Handler tts--> voice start");
        this->set_vh_status(vh_processing);


    };break;
    case speech::TTS_RES_CANCELLED:
    case speech::TTS_RES_END: {
        LOGOUT("Handler tts--> voice end");

        this->set_vh_status(vh_finish);
        fillfinish();

    };break;
    }
}
void TtsSdk::run()
{
    speech::TtsResult result;

    while (true)    {
        if (!tts->poll(result)) {
            reinit();
            LOGOUT("tts->poll failed ,reinit tts!!");
        }

        // 处理result
        Handle_tts_result(result);
    }
}
