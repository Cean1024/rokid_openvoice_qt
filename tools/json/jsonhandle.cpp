#include "jsonhandle.h"

JsonHandle::JsonHandle()
{
}

JsonHandle::~JsonHandle()
{

}



r_status JsonHandle::analyses(std::string &data)
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
            action_voice(item);
        }
    }else if( type == "media") {
        std::string action = arrydata->get("action").toString();
        JSON::Object::Ptr item = arrydata->getObject("item");
        if(action == "PLAY") action_play(item);
        else action_stop(item);

    }else if (type == "pickup") {
        /*TO DO*/
    }

    return SUCCESS;
}
void JsonHandle::handleresponse()
{

        while(data.empty())usleep(100000);
        LOGOUT("JsonHandle : analys data");
        parse.reset();

        json = parse.parse( data );

        pObj = json.extract<JSON::Object::Ptr>();

        response = pObj->getObject("response");

        action = response->getObject("action");

        pArry = action->getArray( "directives" );

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

        LOGOUT("JsonHandle : finish, clean data");

        data.clear();

}
