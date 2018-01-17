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

using namespace Poco;

typedef void( *cb_action)(JSON::Object::Ptr &item,void *data);

enum action_type {
  action_type_voice,
  action_type_media,
  action_type_stop
};


class JsonHandle
{
public:
    JsonHandle();
    ~JsonHandle();
    r_status handle(std::string &data);
    r_status handleaction(JSON::Object::Ptr &arrydata);
    r_status cb_registe(cb_action func,void *data,action_type type);
private:
    void *voicedata;
    void *mediadata;
    void *stopdata;
    cb_action voice_func;
    cb_action media_func;
    cb_action stop_func;
};

#endif // JSONHANDLE_H
