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

    r_status analyses(std::string &data);

protected:
    void handleresponse();

    r_status handleaction(JSON::Object::Ptr &arrydata);
    virtual void action_play(JSON::Object::Ptr &item)=0;
    virtual void action_stop(JSON::Object::Ptr &item)=0;
    virtual void action_voice(JSON::Object::Ptr &item)=0;

private:
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
