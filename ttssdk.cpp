#include "ttssdk.h"

TtsSdk::TtsSdk()
{
    handleresult=NULL;
    voicehandle = vh_finish;
    tts = speech::Tts::new_instance();
    tts_id =0;
    player =nullptr;
    mp3player =nullptr;;
}

TtsSdk::~TtsSdk()
{
    if(tts)tts->release();
}
vh_status TtsSdk::speek(std::string strings)
{
     vh_status vhs;
     int times =  RETRY_TIME;
speek_start:

     LOGOUT("start tts speak");
     if(tts_id >0 ) {tts->cancel(tts_id); tts_id =0;sleep(1);}
     set_vh_status(vh_start);
     tts_id = tts->speak(strings.c_str());
     LOGOUT("finish tts speak");
     while( true ) {
         vhs = get_vh_status();

         if( vhs == vh_finish) break;
         else if( vhs == vh_err  ) {
             tts->cancel(tts_id);
             tts_id =0;
             reinit();
             goto speek_start;
         } else if(vhs == vh_start ) {

             if( times > 0)
                 times--;
             else {
                 tts->cancel(tts_id);
                 tts_id = 0;
                 //reinit();
                 break;
             }
         }
         sleep(1);
     }
     LOGOUT("OUT TtsSdk");
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
    tts->config(topts);
}

int TtsSdk::init( speech::PrepareOptions &popts, callback_tts_func func, void *data )
{
    handleresult = func;
    this->popts = popts;
    this->data = data;

    reinit();

    ptr.start(*this);
}

void TtsSdk::addplayer(Pcmplayer &player )
{

    this->player = &player;

}
void TtsSdk::addplayer(Player &player)
{
    this->mp3player = &player;

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
        handleresult(result,data,this);

        }

}
