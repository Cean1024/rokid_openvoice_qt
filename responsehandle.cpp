#include "responsehandle.h"

ResponseHandle::ResponseHandle()
{

}
r_status ResponseHandle::cb_registe(void *data,action_type type)
{
    switch(type){
    case action_type_voice:{this->voicedata = data;};break;
    case action_type_media:{this->mediadata = data;};break;
    case action_type_stop:{;this->stopdata = data;};break;
    default:return ERROR;
    }
    return SUCCESS;
}
void ResponseHandle::action_play(JSON::Object::Ptr &item)
{
    HttpPlayer *hplayer=(HttpPlayer *)mediadata;
    Dynamic::Var tmp = item->get("url");
    std::string url = tmp.toString();
    hplayer->stop();
    hplayer->start(url);
}

void ResponseHandle::action_stop(JSON::Object::Ptr &item)
{
    HttpPlayer *hplayer= (HttpPlayer *)stopdata;

    hplayer->stop();
}
void ResponseHandle::action_voice(JSON::Object::Ptr &item)
{
    TtsSdk *tts_sdk = (TtsSdk *)voicedata;
    Dynamic::Var tmp = item->get("tts");
    std::string tts = tmp.toString();
    int index =tts.find("若琪",0);
    if( index !=std::string::npos) tts.replace(tts.find("若琪",0),6,"小桑");

    LOGOUT("tts:%s",tts.c_str());
    //return ;
    tts_sdk->set_vh_status(vh_start);
    //vh_status vhs = tts_sdk->speek(tts);
    tts_sdk->speek(tts);
    tts_sdk->start_speak();
    while ( tts_sdk->get_vh_status() == vh_start ) usleep(10000);
    tts_sdk->stop_speak();
}
void ResponseHandle::run()
{
    while(true) {
        handleresponse();
    }
}
