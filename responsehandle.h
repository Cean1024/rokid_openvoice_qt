#ifndef RESPONSEHANDLE_H
#define RESPONSEHANDLE_H
#include "tools/json/jsonhandle.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "common.h"
#include "ttssdk.h"
#include "tools/http/httpdl.h"
#include "httpplayer.h"

class ResponseHandle:public Poco::Runnable,public JsonHandle
{
public:
    ResponseHandle();
    void start()
    {
        thread.start(*this);
    }

    r_status cb_registe(void *data,action_type type);

protected:
    void run();
    void action_play(JSON::Object::Ptr &item);
    void action_stop(JSON::Object::Ptr &item);
    void action_voice(JSON::Object::Ptr &item);

private:
    void *voicedata;
    void *mediadata;
    void *stopdata;
    Poco::Thread thread;

};

#endif // RESPONSEHANDLE_H
