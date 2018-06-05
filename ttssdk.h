#ifndef TTSSDK_H
#define TTSSDK_H
#include <iostream>
#include <memory>
#include <speech/tts.h>
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "common.h"
#include "player/pcmplayer.h"
#include "httpplayer.h"

typedef void (*callback_tts_func)(rokid::speech::TtsResult &Result,void *data,void *data2);
#define RETRY_TIME 10
enum vh_status{
    vh_start,
    vh_finish,
    vh_processing,
    vh_err
};

using namespace rokid;
class TtsSdk:public Pcmplayer,public Poco::Runnable
{
public:
    TtsSdk();
    ~TtsSdk();
    int init(speech::PrepareOptions &popts );
    vh_status speek(std::string strings);
    void addplayer(HttpPlayer &player);

    void start_speak();
    void stop_speak();
    void filldata(char *buf,int size);
    void set_vh_status(vh_status vhs);
    vh_status get_vh_status();

protected:
    virtual void run();
    void reinit();
    void Handle_tts_result(speech::TtsResult &Result);
private:
    std::shared_ptr<speech::Tts> tts;
    speech::PrepareOptions popts;
    callback_tts_func handleresult;
    HttpPlayer *httpplayer;
    Poco::Thread ptr;
    void * data;
    vh_status voicehandle;
    int tts_id;

};

#endif // TTSSDK_H
