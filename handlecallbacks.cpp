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




void openvoicerunnable(void *data)
{
    speech::PrepareOptions popts;
    filehandler fhdl;
    TtsSdk tts_sdk;
    SpeechSdk speech_sdk;
    HttpPlayer hplayer;
    ResponseHandle Jhandl;


    fhdl.Getconfigfile(popts);

    Jhandl.cb_registe((void *)&hplayer,action_type_media);
    Jhandl.cb_registe((void *)&hplayer,action_type_stop);
    Jhandl.cb_registe((void *)&tts_sdk,action_type_voice);

    Jhandl.start();

    tts_sdk.addplayer(hplayer);

    speech_sdk.init(popts ,&Jhandl);
    tts_sdk.init(popts);

    //std::string input;
    netserver server("192.168.199.122",9999);
    char buf[4096];
    int ret;
    //int count=0;

    //LinkList revdata(REV_AFRAMEBUFSIZE);

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
            int ttsflag=0;
            playstatus p_sta = hplayer.getPlayStatus();
            Pcmplayer_status tts_sta = tts_sdk.returnstatus();
            if(p_sta == start_play ){
                hplayer.pause_player();
                playerflag =1;
            }
            if(tts_sta == Pcmplayer_start ){
                tts_sdk.pause_pcmplayer();
                ttsflag =1;
            }

            speech_sdk.speek_voice (nullptr,0,voice_start);
            do{
                ret = server.getdata(buf,4096);
                if (ret > 0) {
                speech_sdk.speek_voice (buf,ret,voice_data);
                } else break;
            }
            while( true );
            speech_sdk.speek_voice(buf,ret,voice_end);
#if 0

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
            hplayer.resume();
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
            int count=3;
            speech_handle_status shs;
            while( count-- > 0 && (shs = speech_sdk.get_handle_status()) == handle_start ) sleep(1);
            if( (shs == handle_start || shs == handle_err) && retryTime-- >0 ) {  // wait timeout or err occoured
                speech_sdk.speek_voice(nullptr,0,voice_cancal);
                //speech_sdk.reinit();
                goto entry_of_speech;
            }
            revdata.clean();
#endif
            if(playerflag)hplayer.resume_player();
            if(ttsflag)tts_sdk.resume_pcmplayer();

        }
#endif

    };
}
