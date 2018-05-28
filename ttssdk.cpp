#include "ttssdk.h"

TtsSdk::TtsSdk()
{
    handleresult=NULL;
    voicehandle = vh_finish;
    tts = speech::Tts::new_instance();
}

TtsSdk::~TtsSdk()
{
    if(tts)tts->release();
}
vh_status TtsSdk::speek(std::string strings)
{
     int ret;
     vh_status vhs;
     int times =  RETRY_TIME;
speek_start:

     LOGOUT("in");
     set_vh_status(vh_start);
     ret = tts->speak(strings.c_str());
     LOGOUT("in");
     while( true ) {
         vhs = get_vh_status();

         if( vhs == vh_finish) break;
         else if( vhs == vh_err  ) {
             tts->cancel(ret);
             reinit();
             goto speek_start;
         } else if(vhs == vh_start) {

             if( times > 0)
                 times--;
             else {
                 tts->cancel(ret);
                 break;
             }
         }
         sleep(1);
     }
     LOGOUT("OUT");
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
