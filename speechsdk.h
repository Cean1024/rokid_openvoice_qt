#ifndef SPEECHSDK_H
#define SPEECHSDK_H
#include <iostream>
#include <memory>
#include "speech.h"


#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "common.h"
#include "filehandler.h"

using namespace rokid;
typedef void (*callback_speech_func)(rokid::speech::SpeechResult &Result,void *data);

class SpeechSdk: public Poco::Runnable
{
public:
    SpeechSdk();
    ~SpeechSdk();
    int init(rokid::speech::PrepareOptions &popts, callback_speech_func func, void *data);
    int speek(std::string strings);

    virtual void run();
    void speech_run();
protected:

    int speech_init(speech::PrepareOptions &popts);

private:
    Poco::Thread thread;

    std::shared_ptr<speech::Speech> speech = speech::Speech::new_instance();

    callback_speech_func handleresult;
    void * data;
    //
};

#endif // SPEECHSDK_H
