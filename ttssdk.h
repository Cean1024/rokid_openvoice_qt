#ifndef TTSSDK_H
#define TTSSDK_H
#include <iostream>
#include <memory>
#include "tts.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "common.h"
typedef void (*callback_tts_func)(rokid::speech::TtsResult &Result,void *data,void *data2);
#define RETRY_TIME 5
enum vh_status{
    vh_start,
    vh_finish,
    vh_err
};

using namespace rokid;
class TtsSdk: public Poco::Runnable
{
public:
    TtsSdk();
    ~TtsSdk();
    int init(speech::PrepareOptions &popts ,callback_tts_func func,void *data);
    int speek(std::__cxx11::string strings);
    void set_vh_status(vh_status vhs)
    {
        voicehandle=vhs;
    }
    vh_status get_vh_status()
    {
        return voicehandle;
    }
protected:
    virtual void run();
    void reinit();
private:
    std::shared_ptr<speech::Tts> tts = speech::Tts::new_instance();
    speech::PrepareOptions popts;
    callback_tts_func handleresult;
    Poco::Thread ptr;
    void * data;
    vh_status voicehandle;

};

#endif // TTSSDK_H
