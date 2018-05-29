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
    this->data = data;
    return SUCCESS;
}
r_status JsonHandle::handleaction(JSON::Object::Ptr &arrydata)
{
    std::string type =arrydata->get("type").toString();

    if(type == "voice") {
        std::string action = arrydata->get("action").toString();
        if( action == "PLAY" ) {
            JSON::Object::Ptr item = arrydata->getObject("item");
            if(voice_func) voice_func(item,voicedata);
        }
    }else if( type == "media") {
        std::string action = arrydata->get("action").toString();
        JSON::Object::Ptr item = arrydata->getObject("item");
        if(action == "PLAY") if(media_func) media_func(item,mediadata);
        else if(stop_func) stop_func(item,stopdata);

    }else if (type == "pickup") {
        /*TO DO*/
    }

    return SUCCESS;
}
void JsonHandle::run()
{
    LOGOUT("in JsonHandle");
    while(true) {
        while(data.empty())usleep(100000);
        LOGOUT("JsonHandle : analys data");
        parse.reset();

        json = parse.parse( data );

        pObj = json.extract<JSON::Object::Ptr>();
        //if( pObj != nullptr) {
        response = pObj->getObject("response");
        //if(response != nullptr) {
        action = response->getObject("action");
        //  if(action != nullptr) {
        pArry = action->getArray( "directives" );
        //    if(pArry !=nullptr) {
        if(pArry->size()>0) {
            it = pArry->begin();

            for ( ; it != pArry->end(); it++ )
            {
                std::cout << it->toString() << std::endl;
                parse.reset();
                json =  parse.parse(it->toString());
                JSON::Object::Ptr arrydata = json.extract<JSON::Object::Ptr>();
                handleaction(arrydata);
            }
        }
        //   }
        // }
        //}
        //}
        LOGOUT("JsonHandle : finish, clean data for next");

        data.clear();
    }
}
