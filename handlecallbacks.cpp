#include "handlecallbacks.h"

void handlenext(void *data)
{
    SpeechSdk  * sbody = (SpeechSdk *)data;
    sbody->speek("下一首");
}
r_status handlhttpdl(char *buf,void *param)
{
    Player *player=(Player *)param;
    return player->fillaudiodata(buf,MEMPOOLBUFSIZE);
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

    LOGOUT("tts:%s",tts.c_str());
    tts_sdk->speek(tts);

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

void Handle_tts_result(speech::TtsResult &Result, void *data,void *data2)
{
    struct noded *pdata = (struct noded *)data;
    Player * player = (Player *)pdata->data1;
    Pcmplayer * pcmplayer = (Pcmplayer *)pdata->data2;

    switch (Result.type) {
    case speech::TTS_RES_ERROR: {
        printf("Handler ------tts------> err:%d\n",Result.err);
        pcmplayer->finish();
        player->resume();

    };break;
    case speech::TTS_RES_VOICE: {

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
    case speech::TTS_RES_END: {

        pcmplayer->waitfinish();
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
    netserver server("192.168.199.122",9999);
    char buf[4096];
    int ret;
    //int count=0;

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
            createWavHead  head_c;
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
        {
            int playerflag=0;
            int pcmflag=0;
            playstatus p_sta = player.getPlayStatus();
            Pcmplayer_status pcm_sta = pcmplayer.returnstatus();
            if(p_sta == start_play || p_sta == resume_play){
                player.pause();
                playerflag =1;
            }
            if(pcm_sta == Pcmplayer_start || pcm_sta == Pcmplayer_resume ) {
                pcmplayer.pause();
                pcmflag =1;
            }

            LinkList revdata(REV_AFRAMEBUFSIZE);
            listnode_d *node;
            do{
                ret = server.getdata(buf,4096);
                if (ret > 0) {
                    node = revdata.CreateNode();
                    memcpy(node->buf,buf,ret);
                    node->size = ret;
                    revdata.Insert(node);
                } else break;
            }
            while( true );
            player.resume();
            int retryTime = 0;
entry_of_speech:
            speech_sdk.speek_voice (nullptr,0,voice_start);
            while(true) {
                node = nullptr;
                ret = revdata.read ( &node ,read_list);
                if(ret == SUCCESS ) {
                    speech_sdk.speek_voice (node->buf,node->size,voice_data);
                } else break;
            }
            speech_sdk.speek_voice(buf,ret,voice_end);
            int count=5;
            speech_handle_status shs;
            while( count-- > 0 && (shs = speech_sdk.get_handle_status()) == handle_start ) sleep(1);
            if( (shs == handle_start || shs == handle_err) && retryTime-- >0 ) {  // wait timeout or err occoured
                speech_sdk.reinit();
                goto entry_of_speech;
            }
            revdata.clean();
            if(playerflag)player.resume();
            if(pcmflag)pcmplayer.resume();

        }
#endif

    };
}
