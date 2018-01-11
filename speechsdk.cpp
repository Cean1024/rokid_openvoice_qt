#include "speechsdk.h"

SpeechSdk::SpeechSdk()
{
    handleresult=NULL;
}

SpeechSdk::~SpeechSdk()
{

}

int SpeechSdk::speek(std::string strings)
{
    //"若琪你好"

        return speech->put_text(strings.c_str());
}



int SpeechSdk::speech_init(speech::PrepareOptions &popts)
{

    std::shared_ptr<speech::SpeechOptions> opts = speech::SpeechOptions::new_instance();
    opts->set_codec(speech::Codec::OPU);
    opts->set_lang(speech::Lang::ZH);
    speech->config(opts);
    speech->prepare(popts);
    speech->put_text("若琪,我要听科技新闻");

}
int SpeechSdk::init(rokid::speech::PrepareOptions &popts, callback_speech_func func)
{
    handleresult = func;

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
        handleresult(result);
    }
}

void SpeechSdk::run()
{
    speech_run();
}

