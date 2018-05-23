#ifndef HANDLECALLBACKS
#define HANDLECALLBACKS
#include "speechsdk.h"
#include "ttssdk.h"
#include "player/player.h"
#include "tools/http/httpdl.h"
#include "tools/json/jsonhandle.h"
#include "player/pcmplayer.h"
#include "netserver.h"
#include "wavfile.h"


void Handle_speech_result(speech::SpeechResult &Result,void *data);
void Handle_tts_result(speech::TtsResult &Result, void *data, void *data2);
r_status handlhttpdl(char *buf,void *param);
void action_play(JSON::Object::Ptr &item,void *data);
void action_stop(JSON::Object::Ptr &item,void *data);
void action_voice(JSON::Object::Ptr &item,void *data);
void handlenext(void *data);
void openvoicerunnable(void *data);
struct noded {
    void *data1;
    void *data2;
};

#endif // HANDLECALLBACKS

