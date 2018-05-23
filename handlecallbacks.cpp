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
    sleep(2);
    std::cout <<tts<< std::endl;
}

void Handle_speech_result(speech::SpeechResult &Result, void *data)
{
    JsonHandle * handl = (JsonHandle *)data;

    switch (Result.type) {
    case speech::SPEECH_RES_ERROR: {
        printf("Handler ------speech------> err:%d\n",Result.err);
    };break;
    case speech::SPEECH_RES_INTER: {
        printf("Handler ------speech------> SPEECH_RES_INTER\n");
        std::cout<<"[SPEECH_RES_INTER]"<<std::endl;
        std::cout<<"nlp:"<<Result.nlp<<std::endl;
        std::cout<<"asr:"<<Result.asr<<std::endl;
        std::cout<<"action:"<<Result.action<<std::endl;
        std::cout<<"extra:"<<Result.extra<<std::endl;

    };break;
    case speech::SPEECH_RES_START: {
        printf("Handler ------speech------> SPEECH_RES_START\n");

    };break;
    case speech::SPEECH_RES_CANCELLED:
    case speech::SPEECH_RES_END: {
        printf("Handler ------speech------> SPEECH_RES_END\n");
        handl->handle(Result.action);
    };break;

    case speech::SPEECH_RES_ASR_FINISH: {
        std::cout<<"action:"<<Result.action<<std::endl;
    };break;
    }
}

void Handle_tts_result(speech::TtsResult &Result, void *data)
{
    struct noded *pdata = (struct noded *)data;
    Player * player = (Player *)pdata->data1;
    Pcmplayer * pcmplayer = (Pcmplayer *)pdata->data2;

    switch (Result.type) {
    case speech::TTS_RES_ERROR:{
        printf("Handler ------tts------> err:%d\n",Result.err);
    };break;
    case speech::TTS_RES_VOICE:{

        pcmplayer->fillaudiodata(\
                    (char *)Result.voice->c_str(),\
                    Result.voice->size());
        printf("Handler ------tts------> get voice  size:%d\n",Result.voice->size());

    };break;
    case speech::TTS_RES_START: {

        player->pause();
        char buf[PCMPLAYERFRAMSIZE];
        memset(buf,0,PCMPLAYERFRAMSIZE);
        pcmplayer->start();
        pcmplayer->fillaudiodata(buf,PCMPLAYERFRAMSIZE);
        printf("Handler ------tts------> voice start \n");
    };break;
    case speech::TTS_RES_CANCELLED:
    case speech::TTS_RES_END:{

        //printf("Handler ------tts------> voice end 1\n");
        pcmplayer->finish();
        //printf("Handler ------tts------> voice end 2\n");
        while(
              pcmplayer->returnstatus() \
              != Pcmplayer_stop )
            usleep(100000);
        //printf("Handler ------tts------> voice end 3\n");
        player->resume();
        printf("Handler ------tts------> voice end 4\n");
    };break;
    }
}
void openvoicerunnable(void *data)
{

    speech::PrepareOptions popts;
    filehandler fhdl;
    TtsSdk tts_sdk;
    SpeechSdk speech_sdk;
    Player player(handlenext,(void *)&speech_sdk);
    Pcmplayer pcmplayer;
    JsonHandle Jhandl;
    Httpdl hdl;
    struct noded Jsonhandlerdata,ttshandledata;

    Jsonhandlerdata.data1 = &player;
    Jsonhandlerdata.data2 = &hdl;
    ttshandledata.data1 = &player;
    ttshandledata.data2 = &pcmplayer;

    fhdl.Getconfigfile(popts);

    Jhandl.cb_registe(action_play,(void *)&Jsonhandlerdata,action_type_media);
    Jhandl.cb_registe(action_stop,(void *)&Jsonhandlerdata,action_type_stop);
    Jhandl.cb_registe(action_voice,(void *)&tts_sdk,action_type_voice);

    speech_sdk.init(popts , Handle_speech_result,(void *)&Jhandl);
    tts_sdk.init(popts ,Handle_tts_result,(void *)&ttshandledata);
    //std::string input;
    netserver server("192.168.199.245",9009);
    char buf[4096];
    int ret;
    int count=0;
    createWavHead  head_c;
    while(1) {
#if 0
        //int fd=open("/home/samba/来首音乐.wav",O_RDONLY);

        int fd=open("/home/samba/audio_2",O_RDONLY);
        ret = read(fd,buf,44);
        speech_sdk.speek_voice(nullptr,0,voice_start);
        memset(buf,0,1024);
        while ( ( ret = read(fd,buf,1024)) > 0 ) {

            speech_sdk.speek_voice(buf,ret,voice_data);
            memset(buf,0,1024);
        }
        speech_sdk.speek_voice(buf,ret,voice_end);
        close(fd);
#endif

#if 0
        DEBUG("waitting\n");
        sleep(100);
        //std::cin >> input;
        //speech_sdk.speek(input);
#endif

#if 0
        server.listenandaccept();

        memset(buf,0,4096);
        {
            std::string filename = "/home/samba/audio_";
            int sum=0;
            filename += std::to_string(count++);
            int fd = open(filename.c_str() ,   O_WRONLY | O_CREAT,0777);
            lseek(fd,sizeof (wav_hdr),SEEK_CUR);
            while ( (ret = server.getdata( buf , 4096)) >0){
                write(fd,buf,ret);
                sum +=ret;
            };
            server.closeclient();
            lseek( fd , 0 , SEEK_SET );
            wav_hdr *hd = head_c.CHead( 16000 , 1 , 16 , sizeof (wav_hdr) + sum );
            write(fd , hd , sizeof (wav_hdr) );

            close(fd);
            head_c.deletehead(hd);
        }
#endif

#if 1
        server.listenandaccept();
        speech_sdk.speek_voice(nullptr,0,voice_start);
        do{
            ret = server.getdata(buf,4096);
            if (ret > 0) speech_sdk.speek_voice(buf,ret,voice_data);
            else break;
        }
        while(true);
        speech_sdk.speek_voice(buf,ret,voice_end);
#endif

    };
}
