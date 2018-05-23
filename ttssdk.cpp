#include "ttssdk.h"

TtsSdk::TtsSdk()
{
    handleresult=NULL;
    voicehandle = vh_finish;
}

TtsSdk::~TtsSdk()
{

}
int TtsSdk::speek(std::string strings)
{
     int ret;
     vh_status vhs;
     int times =  RETRY_TIME;
speek_start:

     set_vh_status(vh_start);
     ret = tts->speak(strings.c_str());
     while( true ) {
         vhs = get_vh_status();
         if( vhs == vh_finish) break;
         else if( vhs == vh_err  ) {
             reinit();

             if( times > 0)
                 times--;
             else break;

             goto speek_start;
         }
         usleep(100000);
     }
     return ret;

}

void TtsSdk::reinit()
{
    tts->prepare(popts);
}

int TtsSdk::init( speech::PrepareOptions &popts, callback_tts_func func, void *data )
{
    handleresult = func;
    tts->prepare(popts);
    this->popts = popts;
    this->data = data;

    // 在prepare后任意时刻，都可以调用config修改配置
    // 默认配置codec = PCM, declaimer = ZH
    // 下面的代码将codec修改为OPU2，declaimer保持原状不变

    std::shared_ptr<speech::TtsOptions> topts = speech::TtsOptions::new_instance();
    topts->set_codec(speech::Codec::PCM);
    tts->config(topts);

    // 使用tts
    //int32_t id = tts->speak("我是会说话的机器人,我最爱吃的食物是机油,最喜欢的运动是聊天");
    //int32_t id = tts->speak("机器人");

    // 获取tts结果。api阻塞式，应考虑在独立线程中运行。

    ptr.start(*this);
}

void TtsSdk::run()
{
    speech::TtsResult result;

    while (true) {
        if (!tts->poll(result))
            break;
        // 处理result
        handleresult(result,data,this);
        switch(result.type) {
        case speech::TTS_RES_ERROR: {
            set_vh_status(vh_err);
        };break;
        case speech::TTS_RES_CANCELLED:
        case speech::TTS_RES_END:{
            set_vh_status(vh_finish);
        };break;

        }
    }
}
