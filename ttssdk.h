#ifndef TTSSDK_H
#define TTSSDK_H
#include <iostream>
#include <memory>
#include "tts.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "common.h"
typedef void (*callback_tts_func)(rokid::speech::TtsResult &Result);

using namespace rokid;
class TtsSdk: public Poco::Runnable
{
public:
    TtsSdk();
    ~TtsSdk();
    int init(speech::PrepareOptions &popts ,callback_tts_func func);
    int speeck(std::__cxx11::string strings);

    virtual void run();
private:
    std::shared_ptr<speech::Tts> tts = speech::Tts::new_instance();
    callback_tts_func handleresult;

};

#endif // TTSSDK_H
