#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <iostream>
#include <fstream>
#include <cstdio>
#include "common.h"
#include "tts.h"
#include "speech.h"

#include "Poco/JSON/Parser.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/StreamCopier.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/JSON/Query.h"
#include "Poco/JSON/PrintHandler.h"

struct file_status{
    std::string audiofilepath;
    int file_status; // 0 not open , 1 open ,-1 err;
};

using namespace rokid;
class filehandler
{
public:
    filehandler();
    ~filehandler();
    r_status Getconfigfile(speech::PrepareOptions &popts);
    void Handle_tts_result(speech::TtsResult &Result);
    void Handle_speech_result(speech::SpeechResult &Result);

private:
    std::ofstream f_tts;
    std::ofstream f_speech;
    file_status audiofile_tts;
    file_status audiofile_speech;
};

#endif // FILEHANDLER_H
