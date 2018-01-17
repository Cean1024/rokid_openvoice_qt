#include "jsonhandle.h"

JsonHandle::JsonHandle()
{    voicedata = NULL;
     mediadata  = NULL;
     voice_func = NULL;
     media_func = NULL;

}

JsonHandle::~JsonHandle()
{

}
r_status JsonHandle::cb_registe(cb_action func,void *data,action_type type)
{
    switch(type){
    case action_type_voice:{this->voice_func = func;this->voicedata = data;};break;
    case action_type_media:{this->media_func = func;this->mediadata = data;};break;
    case action_type_stop:{this->stop_func = func;this->stopdata = data;};break;
    default:return ERROR;
    }
    return SUCCESS;
}

r_status JsonHandle::handle(std::string &data)
{
    JSON::Parser parse;
    Dynamic::Var json = parse.parse( data );
    JSON::Object::Ptr pObj = json.extract<JSON::Object::Ptr>();

    JSON::Object::Ptr response = pObj->getObject("response");

    JSON::Object::Ptr action = response->getObject("action");

    JSON::Array::Ptr pArry = action->getArray( "directives" );

    JSON::Array::ConstIterator it = pArry->begin();

    for ( ; it != pArry->end(); it++ )
    {  
        //std::cout << it->toString() << std::endl;
        parse.reset();
        json =  parse.parse(it->toString());
        JSON::Object::Ptr arrydata = json.extract<JSON::Object::Ptr>();
        handleaction(arrydata);
    }
     return SUCCESS;
}
r_status JsonHandle::handleaction(JSON::Object::Ptr &arrydata)
{
    Dynamic::Var tmp = arrydata->get("action");
    JSON::Object::Ptr item = arrydata->getObject("item");
    std::string tmpp = tmp.toString();
    if( tmpp == "PLAY" ) {
        tmp =arrydata->get("type");
        tmpp = tmp.toString();
        if( tmpp == "voice") {
            DEBUG("voice handler action\n");
            if(voice_func) voice_func(item,voicedata);

        }else if(tmpp == "media") {
            DEBUG("media handler action\n");
            if(media_func) media_func(item,mediadata);

        }

    } else if ( tmpp == "STOP" ) {
        DEBUG("STOP handler action\n");
        if(stop_func) stop_func(item,stopdata);

    }
    return SUCCESS;
}
