#ifndef JSONHANDLE_H
#define JSONHANDLE_H
#include "common.h"

#include "Poco/JSON/Parser.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/StreamCopier.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/JSON/Query.h"
#include "Poco/JSON/PrintHandler.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"

using namespace Poco;

typedef void( *cb_action)(JSON::Object::Ptr &item,void *data);

enum action_type {
  action_type_voice,
  action_type_media,
  action_type_stop
};


class JsonHandle:public Poco::Runnable
{
public:
    JsonHandle();
    ~JsonHandle();
    r_status handle(std::string &data);
    r_status handleaction(JSON::Object::Ptr &arrydata);
    r_status cb_registe(cb_action func,void *data,action_type type);
    void start()
    {
        thread.start(*this);
    }
protected:
    void run();

private:
    void *voicedata;
    void *mediadata;
    void *stopdata;
    cb_action voice_func;
    cb_action media_func;
    cb_action stop_func;

    Poco::Thread thread;

    std::string data;

    JSON::Parser parse;
    Dynamic::Var json;
    JSON::Object::Ptr pObj;

    JSON::Object::Ptr response;

    JSON::Object::Ptr action;

    JSON::Array::Ptr pArry;

    JSON::Array::ConstIterator it;

};

#endif // JSONHANDLE_H
