#include "filehandler.h"

filehandler::filehandler()
{
    audiofile_tts.audiofilepath = "/home/samba/work/rokid_openvoice_sdk/rokid_openvoice_qt/rokid_openvoice_qt/test/test_tts";
    audiofile_speech.audiofilepath = "/home/samba/work/rokid_openvoice_sdk/rokid_openvoice_qt/rokid_openvoice_qt/test/test_speech";
    audiofile_speech.file_status =-1;
    audiofile_tts.file_status =-1;
}

filehandler::~filehandler()
{

}

r_status filehandler::Getconfigfile(speech::PrepareOptions &popts)
{

    using namespace Poco;
    std::string tmp =CONFIGFILEPATH;
    tmp += CONFIGFILENAME;
    char buf[256];
    std::ifstream  fileop(tmp);
    std::string data;
    if(fileop.is_open()) {
        while (!fileop.eof()) {
            fileop.getline(buf,256);
            tmp =buf;
            data +=tmp;
        }
    }

    JSON::Parser parse;
    Dynamic::Var json = parse.parse( data );
    std::cout << json.toString() << std::endl;
    JSON::Object::Ptr pObj = json.extract<JSON::Object::Ptr>();
    JSON::Object::Ptr speech_config = pObj->getObject("speech_config");

    json = speech_config->get("host");
    popts.host =json.toString();
    //std::cout << popts.host << std::endl;


    json = speech_config->get("port");
    popts.port = json.convert<int>();
    //std::cout << popts.port << std::endl;

    json = speech_config->get("branch");
    popts.branch =json.toString();
    //std::cout << popts.branch << std::endl;

    json = speech_config->get("key");
    popts.key =json.toString();
    //std::cout << popts.key << std::endl;

    json = speech_config->get("device_type_id");
    popts.device_type_id =json.toString();
    //std::cout << popts.device_type_id << std::endl;

    json = speech_config->get("secret");
    popts.secret =json.toString();
    //std::cout << popts.secret << std::endl;

    json = speech_config->get("device_id");
    popts.device_id =json.toString();
    //std::cout << popts.device_id << std::endl;

    return SUCCESS;

}


void filehandler::Handle_speech_result(speech::SpeechResult &Result)
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
    case speech::SPEECH_RES_INTER:{
        printf("Handler ------speech------> get voice\n");
        f_speech<<"[SPEECH_RES_INTER]"<<std::endl;
        f_speech<<"nlp:"<<Result.nlp<<std::endl;
        f_speech<<"asr:"<<Result.asr<<std::endl;
        f_speech<<"action:"<<Result.action<<std::endl;
        f_speech<<"extra:"<<Result.extra<<std::endl;

    };break;
    case speech::SPEECH_RES_START:{
        printf("Handler ------speech------> voice start\n");
        f_speech.open(audiofile_speech.audiofilepath.c_str(),std::ios_base::out |std::ios_base::trunc);
        if(!f_speech){
            printf("file open failed\n");
            audiofile_speech.file_status = -1;
        } else{
            audiofile_speech.file_status = 1;
            f_speech<<"[SPEECH_RES_START]"<<std::endl;
            f_speech<<"nlp:"<<Result.nlp<<std::endl;
            f_speech<<"asr:"<<Result.asr<<std::endl;
            f_speech<<"action:"<<Result.action<<std::endl;
            f_speech<<"extra:"<<Result.extra<<std::endl;
        }
    };break;
    case speech::SPEECH_RES_CANCELLED:
    case speech::SPEECH_RES_END:{
        printf("Handler ------speech------> voice end\n");
        f_speech<<"[SPEECH_RES_END]"<<std::endl;
        f_speech<<"nlp:"<<Result.nlp<<std::endl;
        f_speech<<"asr:"<<Result.asr<<std::endl;
        f_speech<<"action:"<<Result.action<<std::endl;
        f_speech<<"extra:"<<Result.extra<<std::endl;
        if(audiofile_speech.file_status > 0 )
            f_speech.close();
    };break;

    case speech::SPEECH_RES_ASR_FINISH:{
        f_speech<<"action:"<<Result.action<<std::endl;
    };break;
    }


}

void filehandler::Handle_tts_result(speech::TtsResult &Result)
{

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
        f_tts.open(audiofile_tts.audiofilepath.c_str(),std::ios_base::out |std::ios_base::trunc);
        if(!f_tts) {
            printf("file open failed\n");
            audiofile_tts.file_status = -1;
        } else {
            audiofile_tts.file_status = 1;
            f_tts<<Result.voice;
        }
    };break;
    case speech::TTS_RES_CANCELLED:
    case speech::TTS_RES_END: {
        printf("Handler ------tts------> voice end\n");
        f_tts<<Result.voice;
        if(audiofile_tts.file_status > 0 )
            f_tts.close();
    };break;
    }
}
