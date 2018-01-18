#include "handlecallbacks.h"

void handlenext(void *data)
{
    SpeechSdk  * sbody = (SpeechSdk *)data;
    sbody->speek("下一首");
}
void handlhttpdl(char *buf,void *param)
{
    Player *player=(Player *)param;
    player->fillaudiodata(buf,MEMPOOLBUFSIZE);
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

    //return ;

    hdl->RegistCb( handlhttpdl, nodes->data1);
    hdl->setUrl(url);
    hdl->start();
    player->start();
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
    TtsSdk *tts_sdk = (TtsSdk *)data;
    Dynamic::Var tmp = item->get("tts");
    std::string tts = tmp.toString();
    tts_sdk->speek(tts);
    std::cout <<tts<< std::endl;
}

void Handle_speech_result(speech::SpeechResult &Result, void *data)
{
    JsonHandle * handl = (JsonHandle *)data;

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
//std::ofstream audioop;
void Handle_tts_result(speech::TtsResult &Result, void *data)
{
    Pcmplayer * pcmplayer = (Pcmplayer *)data;

    switch (Result.type) {
    case speech::TTS_RES_ERROR:{
        printf("Handler ------tts------> err:%d\n",Result.err);
       };break;
    case speech::TTS_RES_VOICE:{
        printf("Handler ------tts------> get voice  size:%d\n",Result.voice->size());
     //   audioop << *Result.voice;
        pcmplayer->fillaudiodata(\
                    (char *)Result.voice->c_str(),\
                    Result.voice->size());
       };break;
    case speech::TTS_RES_START:{
        printf("Handler ------tts------> voice start\n");
        pcmplayer->start();
      //  audioop.open("audio.pcm",std::ios::out\
       //              | std::ios::app |std::ios::binary);
       };break;
    case speech::TTS_RES_CANCELLED:
    case speech::TTS_RES_END:{
        printf("Handler ------tts------> voice end\n");
         pcmplayer->finish();
      //  audioop.close();
       };break;
    }

}
