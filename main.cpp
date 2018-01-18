
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
    struct noded nodes;

    nodes.data1 = &player;
    nodes.data2 = &hdl;

    fhdl.Getconfigfile(popts);

    Jhandl.cb_registe(action_play,(void *)&nodes,action_type_media);
    Jhandl.cb_registe(action_stop,(void *)&nodes,action_type_stop);
    Jhandl.cb_registe(action_voice,(void *)&tts_sdk,action_type_voice);


    speech_sdk.init(popts , Handle_speech_result,(void *)&Jhandl);
    tts_sdk.init(popts ,Handle_tts_result,(void *)&pcmplayer);
    std::string input;
    while(1) {
        DEBUG("waitting\n");
        std::cin >> input;
        speech_sdk.speek(input);
    };
    return 0;
}
