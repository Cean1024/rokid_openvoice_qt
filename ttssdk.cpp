#include "ttssdk.h"

TtsSdk::TtsSdk()
{
    handleresult=NULL;
}

TtsSdk::~TtsSdk()
{

}
int TtsSdk::speeck(std::string strings)
{
     return tts->speak(strings.c_str());
}

int TtsSdk::init(speech::PrepareOptions &popts, callback_tts_func func)
{

    handleresult = func;
    tts->prepare(popts);

    // 在prepare后任意时刻，都可以调用config修改配置
    // 默认配置codec = PCM, declaimer = ZH
    // 下面的代码将codec修改为OPU2，declaimer保持原状不变
/*
    std::shared_ptr<speech::TtsOptions> topts = speech::TtsOptions::new_instance();
    topts->set_codec(speech::Codec::OPU);
    tts->config(topts);
*/
    // 使用tts
    //int32_t id = tts->speak("我是会说话的机器人,我最爱吃的食物是机油,最喜欢的运动是聊天");
    //int32_t id = tts->speak("机器人");

    // 获取tts结果。api阻塞式，应考虑在独立线程中运行。
}

void TtsSdk::run()
{
    speech::TtsResult result;

    while (true) {
        if (!tts->poll(result))
            break;
        // 处理result
        handleresult(result);
    }
}
