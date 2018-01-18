
#include "speechsdk.h"
#include "ttssdk.h"
#include "player/player.h"
#include "tools/http/httpdl.h"
#include "tools/json/jsonhandle.h"


using namespace std;


void Handle_speech_result(speech::SpeechResult &Result,void *data);
void Handle_tts_result(speech::TtsResult &Result,void *data);
void handlhttpdl(char *buf,void *param);
void action_play(JSON::Object::Ptr &item,void *data);
void action_stop(JSON::Object::Ptr &item,void *data);
void action_voice(JSON::Object::Ptr &item,void *data);
void handlenext(void *data);
struct noded{
    void *data1;
    void *data2;
};


int main()
{


    using namespace rokid;

    speech::PrepareOptions popts;
    popts.host = "apigwws.open.rokid.com";
    popts.port = 443;
    popts.branch = "/api";
    // 认证信息，需要申请
    popts.key = "17E0ECB86CC446BFB62FF0A6A3641B0B";
    popts.device_type_id = "89A9AD627E124316BD8392D749B5C73B";
    popts.secret = "1ECCFBCB2298416693A5790304BDF9C9";
    // 设备名称，类似昵称，可自由选择，不影响认证结果
    popts.device_id = "keantestaudio";
/*
    //SpeechSdk speech_sdk;
    //TtsSdk tts_sdk;
    //filehandler filehdler;
    //speech_sdk.init(popts , Handle_speech_result);
    //tts_sdk.init(popts , Handle_tts_result);


    std::string input;
    while(1) {
        DEBUG("waitting\n");
        std::cin >> input;
        speech_sdk.speek(input);

    };*/

    SpeechSdk speech_sdk;
    Player player(handlenext,(void *)&speech_sdk);
    //Player player;
    JsonHandle Jhandl;

    Httpdl hdl;
    struct noded nodes;
    nodes.data1 = &player;
    nodes.data2 = &hdl;


    Jhandl.cb_registe(action_play,(void *)&nodes,action_type_media);
    Jhandl.cb_registe(action_stop,(void *)&nodes,action_type_stop);
    Jhandl.cb_registe(action_voice,NULL,action_type_voice);


    speech_sdk.init(popts , Handle_speech_result,(void *)&Jhandl);

    std::string input;
    while(1) {
        DEBUG("waitting\n");
        std::cin >> input;
        speech_sdk.speek(input);

    };
    return 0;
}
void handlenext(void *data)
{
    SpeechSdk  * sbody = (SpeechSdk *)data;
    sbody->speek("下一首");
}
void handlhttpdl(char *buf,void *param)
{
    Player *player=(Player *)param;
    listnode_d * node = player->list.CreateNode();
    memcpy(node->buf,buf,node->size);
    player->list.Insert(node);

}

void action_play(JSON::Object::Ptr &item,void *data)
{
    struct noded *nodes = (struct noded *)data;
    Player *player = (Player *)nodes->data1;
    Httpdl *hdl = (Httpdl *)nodes->data2;
    Dynamic::Var tmp = item->get("url");
    std::string url = tmp.toString();
    std::cout <<url<< std::endl;
    hdl->stop();
    player->stop();

    player->start();
    hdl->RegistCb( handlhttpdl, nodes->data1);
    hdl->setUrl(url);
    hdl->start();
}

void action_stop(JSON::Object::Ptr &item,void *data)
{
    struct noded *nodes = (struct noded *)data;
    Player *player = (Player *)nodes->data1;
    Httpdl *hdl = (Httpdl *)nodes->data2;
    hdl->stop();
    player->stop();
}
void action_voice(JSON::Object::Ptr &item,void *data)
{
    Dynamic::Var tmp = item->get("tts");
    std::string url = tmp.toString();
    std::cout <<url<< std::endl;
}


void Handle_speech_result(speech::SpeechResult &Result, void *data)
{
    JsonHandle * handl = (JsonHandle *)data;
/*
    SPEECH_RES_INTER = 0,
    SPEECH_RES_START,
    SPEECH_RES_ASR_FINISH,
    SPEECH_RES_END,
    SPEECH_RES_CANCELLED,
    SPEECH_RES_ERROR
*/
    switch (Result.type) {
    case speech::SPEECH_RES_ERROR:{
        printf("Handler ------speech------> err:%d\n",Result.err);
       };break;
    case speech::SPEECH_RES_INTER: {
        printf("Handler ------speech------> get voice\n");
        std::cout<<"[SPEECH_RES_INTER]"<<std::endl;
        std::cout<<"nlp:"<<Result.nlp<<std::endl;
        std::cout<<"asr:"<<Result.asr<<std::endl;
        std::cout<<"action:"<<Result.action<<std::endl;
        std::cout<<"extra:"<<Result.extra<<std::endl;

       };break;
    case speech::SPEECH_RES_START:{
        printf("Handler ------speech------> voice start\n");

       };break;
    case speech::SPEECH_RES_CANCELLED:
    case speech::SPEECH_RES_END:{
        printf("Handler ------speech------> voice end\n");
        handl->handle(Result.action);
       };break;

    case speech::SPEECH_RES_ASR_FINISH: {
        std::cout<<"action:"<<Result.action<<std::endl;
    };break;
    }
}

std::ofstream f_tts;

void Handle_tts_result(speech::TtsResult &Result, void *data)
{

#if 0
    switch (Result.type) {
    case speech::TTS_RES_ERROR:{
        printf("Handler ------tts------> err:%d\n",Result.err);
       };break;
    case speech::TTS_RES_VOICE:{
        printf("Handler ------tts------> get voice\n");
       };break;
    case speech::TTS_RES_START:{
        printf("Handler ------tts------> voice start\n");

       };break;
    case speech::TTS_RES_CANCELLED:
    case speech::TTS_RES_END:{
        printf("Handler ------tts------> voice end\n");
        std::cout<<Result.voice;
       };break;
    }
#else
    switch (Result.type) {
    case speech::TTS_RES_ERROR:{
        printf("Handler ------tts------> err:%d\n",Result.err);
       };break;
    case speech::TTS_RES_VOICE:{
        printf("Handler ------tts------> get voice\n");
        f_tts<<Result.voice;
       };break;
    case speech::TTS_RES_START:{
        printf("Handler ------tts------> voice start\n");
        f_tts.open("/home/samba/work/rokid_openvoice_sdk/rokid_openvoice_qt/rokid_openvoice_qt/test/test_tts",std::ios_base::out |std::ios_base::trunc);
        if(!f_tts){
            printf("file open failed\n");

        } else {

            f_tts<<Result.voice;
        }
       };break;
    case speech::TTS_RES_CANCELLED:
    case speech::TTS_RES_END: {
        printf("Handler ------tts------> voice end\n");
        f_tts<<Result.voice;

            f_tts.close();
       };break;
    }
#endif
}
