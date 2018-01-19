
#include "handlecallbacks.h"

int main()
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
    std::string input;
    while(1) {
        DEBUG("waitting\n");
        std::cin >> input;
        speech_sdk.speek(input);
    };
    return 0;
}
