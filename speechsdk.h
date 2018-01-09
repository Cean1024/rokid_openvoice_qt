#ifndef SPEECHSDK_H
#define SPEECHSDK_H
#include <iostream>
#include <memory>
#include "speech.h"
#include "tts.h"

#include "Poco/Thread.h"
#include "Poco/Runnable.h"

#include "filehandler.h"

enum sdktype{
    sdktype_tts=0,
    sdktype_speech
};

using namespace rokid;
class SpeechSdk: public Poco::Runnable
{
public:
    SpeechSdk();
    ~SpeechSdk();
    int init(rokid::speech::PrepareOptions &popts,sdktype type);
    int speek(std::string strings);

    virtual void run();
    void tts_run();
    void speech_run();
protected:

    int tts_init(speech::PrepareOptions &popts);
    int speech_init(speech::PrepareOptions &popts);

private:
    Poco::Thread thread;
    std::shared_ptr<speech::Tts> tts = speech::Tts::new_instance();
    std::shared_ptr<speech::Speech> speech = speech::Speech::new_instance();
    sdktype type;
    filehandler filehdler;
};

#endif // SPEECHSDK_H
