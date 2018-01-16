
#include "speechsdk.h"
#include "ttssdk.h"
#include "player/player.h"
#include "tools/http/httpdl.h"

using namespace std;


void Handle_speech_result(speech::SpeechResult &Result);
void Handle_tts_result(speech::TtsResult &Result);
void playhandler(Player &player);
void runhandler(void *data);


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
    SpeechSdk speech_sdk;
    TtsSdk tts_sdk;
    //filehandler filehdler;
    speech_sdk.init(popts , Handle_speech_result);
    //tts_sdk.init(popts , Handle_tts_result);

    std::string input;
    while(1) {
        DEBUG("waitting\n");
        std::cin >> input;
        speech_sdk.speek(input);

    };
*/
    Player player;

    Poco::Thread thr;

    //playhandler(player);
    //thr.start(runhandler,(void *)&player);
    //thr.join();

    Httpdl hdl;
    string url,rname,lname;
    url="http://rokid.oss-cn-qingdao.aliyuncs.com/thirdparty/news/res/common_bg3.mp3";
    rname="/home/cean/test.mp3";


    int filesize;
    try {
         filesize = hdl.HttpGetFile(url,rname,lname);
     }
     catch (Poco::Exception& e) {
         std::cerr << "Catch a poco exception: " << e.displayText() << std::endl;
         return -1;
     }
     catch (std::exception& e) {
         std::cerr << "Catch a std exception: " << e.what() << std::endl;
         return -1;
     }
     catch (...) {
         std::cerr << "Unknown Exception!!!" << std::endl;
         return -1;
     }

    std::cout << "Get " << lname << " " << filesize/1024.0/1024.0 << " MB!!!" << std::endl;

    while(1)sleep(10);
    return 0;
}
void runhandler(void *data)
{
    std::cout << "runhandler \n";
    Player *player = (Player * )data;
    playhandler(*player);
}
void playhandler(Player &player)
{
    std::string input;
    //std::cout << "please input words:";
    //std::cin >> input;
    input = "/home/samba/music/2.mp3";

    std::ifstream  f_audio(input);

    int count =0;
    if(!f_audio.is_open()) {
        DEBUG("Erro opening file:%s",input.c_str());
        return;
    }

    player.start();

    while(1)
    {
        listnode_d * node = player.list.CreateNode();
        f_audio.read(node->buf,node->size);
        player.list.Insert(node);
        DEBUG("insert data %5d  size:%d\n",count++,node->size);
        if( f_audio.eof())
            break;
        usleep(20000);
    }
    f_audio.close();

    while(1) {
        DEBUG("waitting\n");
        sleep(10);

    };
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

std::ofstream f_tts;

void Handle_tts_result(speech::TtsResult &Result)
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
