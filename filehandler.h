#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <iostream>
#include <fstream>
#include <cstdio>
#include "tts.h"
#include "speech.h"

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
    void Handle_tts_result(speech::TtsResult &Result);
    void Handle_speech_result(speech::SpeechResult &Result);

private:
    std::ofstream f_tts;
    std::ofstream f_speech;
    file_status audiofile_tts;
    file_status audiofile_speech;
};

#endif // FILEHANDLER_H
