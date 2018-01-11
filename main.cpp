#include <iostream>
#include <unistd.h>
#include "speechsdk.h"
#include "ttssdk.h"

using namespace std;


void Handle_speech_result(speech::SpeechResult &Result);
void Handle_tts_result(speech::TtsResult &Result);

int main()
{
    using namespace rokid;

    speech::PrepareOptions popts;
    popts.host = "apigwws.open.rokid.com";
    popts.port = 443;
    popts.branch = "/api";
    // 认证信息，需要申请

    // 设备名称，类似昵称，可自由选择，不影响认证结果
    popts.device_id = "keantestaudio";

    SpeechSdk speech_sdk;
    TtsSdk tts_sdk;
    filehandler filehdler;
    speech_sdk.init(popts , Handle_speech_result);
    tts_sdk.init(popts , Handle_tts_result);
    std::string input;
    while(1)
    {
        speech_sdk.speek(input);
        std::cout << "please input words:";
        std::cin >> input;
    }

    return 0;
}

void Handle_speech_result(speech::SpeechResult &Result)
{
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

            std::cout<<"[SPEECH_RES_START]"<<std::endl;
            std::cout<<"nlp:"<<Result.nlp<<std::endl;
            std::cout<<"asr:"<<Result.asr<<std::endl;
            std::cout<<"action:"<<Result.action<<std::endl;
            std::cout<<"extra:"<<Result.extra<<std::endl;

       };break;
    case speech::SPEECH_RES_CANCELLED:
    case speech::SPEECH_RES_END:{
        printf("Handler ------speech------> voice end\n");
        std::cout<<"[SPEECH_RES_END]"<<std::endl;
        std::cout<<"nlp:"<<Result.nlp<<std::endl;
        std::cout<<"asr:"<<Result.asr<<std::endl;
        std::cout<<"action:"<<Result.action<<std::endl;
        std::cout<<"extra:"<<Result.extra<<std::endl;

       };break;

    case speech::SPEECH_RES_ASR_FINISH: {
        std::cout<<"action:"<<Result.action<<std::endl;
    };break;
    }
}


void Handle_tts_result(speech::TtsResult &Result)
{
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
}
