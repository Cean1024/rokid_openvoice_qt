#ifndef SPEECHSDK_H
#define SPEECHSDK_H
#include <iostream>
#include <memory>
#include <speech/speech.h>


#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "common.h"
#include "filehandler.h"
#include "responsehandle.h"

enum voice_status {
    voice_start,
    voice_data,
    voice_end,
    voice_cancal
};
enum speech_handle_status {
    handle_start,
    handle_processing,
    handle_end,
    handle_err
};

using namespace rokid;
typedef void (*callback_speech_func)(rokid::speech::SpeechResult &Result,void *data);

class SpeechSdk: public Poco::Runnable
{
public:
    SpeechSdk();
    ~SpeechSdk();
    void reinit() {
        speech_init();
    }
    int init(rokid::speech::PrepareOptions &popts, ResponseHandle * jhandle);
    int speek(std::string strings);
    int speek_voice(char *buf,int len,voice_status flag);
    speech_handle_status get_handle_status() {
        return sh_status;
    }


protected:
    virtual void run();
    void speech_run();
    int speech_init();
    void Handle_speech_result(speech::SpeechResult &Result);

private:
    Poco::Thread thread;

    std::shared_ptr<speech::Speech> speech ;
    rokid::speech::PrepareOptions popts;
    callback_speech_func handleresult;
    ResponseHandle * j_hd;
    int voice_id;
    speech_handle_status sh_status;
    speech::VoiceOptions *voiceOptions;
    //
};

#endif // SPEECHSDK_H
