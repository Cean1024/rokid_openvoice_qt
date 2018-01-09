#include "speechsdk.h"

SpeechSdk::SpeechSdk()
{
    type=sdktype_tts;
}

SpeechSdk::~SpeechSdk()
{

}

int SpeechSdk::speek(std::string strings)
{
    //"若琪你好"

    if(type == sdktype_tts)
        return tts->speak(strings.c_str());
    else
        return speech->put_text(strings.c_str());
}



int SpeechSdk::tts_init(speech::PrepareOptions &popts)
{
    tts->prepare(popts);

    // 在prepare后任意时刻，都可以调用config修改配置
    // 默认配置codec = PCM, declaimer = ZH
    // 下面的代码将codec修改为OPU2，declaimer保持原状不变

    std::shared_ptr<speech::TtsOptions> topts = speech::TtsOptions::new_instance();
    topts->set_codec(speech::Codec::OPU);
    tts->config(topts);

    // 使用tts
    //int32_t id = tts->speak("我是会说话的机器人,我最爱吃的食物是机油,最喜欢的运动是聊天");
    int32_t id = tts->speak("机器人");

    // 获取tts结果。api阻塞式，应考虑在独立线程中运行。
}

int SpeechSdk::speech_init(speech::PrepareOptions &popts)
{

    std::shared_ptr<speech::SpeechOptions> opts = speech::SpeechOptions::new_instance();
    opts->set_codec(speech::Codec::OPU);
    opts->set_lang(speech::Lang::ZH);
    speech->config(opts);
    speech->prepare(popts);
    speech->put_text("若琪,你好");

}
int SpeechSdk::init(rokid::speech::PrepareOptions &popts,sdktype type)
{
    this->type = type;

    if(type == sdktype_tts) tts_init(popts);
    else speech_init(popts);

    thread.start(*this);
    //thread.join();//等待该线程技术
    return 0;
}

void SpeechSdk::tts_run()
{
    speech::TtsResult result;

    while (true) {
        if (!tts->poll(result))
            break;
        // 处理result
        filehdler.Handle_tts_result(result);
    }
}

void SpeechSdk::speech_run()
{
    speech::SpeechResult result;
    while (true) {
        if (!speech->poll(result))
            break;
        // 处理result
        filehdler.Handle_speech_result(result);
    }
}

void SpeechSdk::run()
{
    if(type == sdktype_tts) tts_run();
    else speech_run();
}

