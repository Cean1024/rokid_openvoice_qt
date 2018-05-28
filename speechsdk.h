#ifndef SPEECHSDK_H
#define SPEECHSDK_H
#include <iostream>
#include <memory>
#include <speech/speech.h>


#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "common.h"
#include "filehandler.h"

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
    int init(rokid::speech::PrepareOptions &popts, callback_speech_func func, void *data);
    int speek(std::string strings);
    int speek_voice(char *buf,int len,voice_status flag);
    speech_handle_status get_handle_status() {
        return sh_status;
    }


protected:
    virtual void run();
    void speech_run();
    int speech_init();

private:
    Poco::Thread thread;

    std::shared_ptr<speech::Speech> speech ;
    rokid::speech::PrepareOptions popts;
    callback_speech_func handleresult;
    void * data;
    int voice_id;
    speech_handle_status sh_status;
    //
};

#endif // SPEECHSDK_H
