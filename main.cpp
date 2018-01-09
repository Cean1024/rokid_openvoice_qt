#include <iostream>
#include <unistd.h>
#include "speechsdk.h"


using namespace std;

int main()
{
    using namespace rokid;

    speech::PrepareOptions popts;
    popts.host = "apigwws.open.rokid.com";
    popts.port = 443;
    popts.branch = "/api";
    // 认证信息，需要申请
    popts.key = "770AE250B1F8488E8EBD0E4E3EE01424";
    popts.device_type_id = "BE947B8C753043A4B2543605A5668B88";
    popts.secret = "E58FDBB7A4EF41209E09C022758F55BF";
    // 设备名称，类似昵称，可自由选择，不影响认证结果
    popts.device_id = "keantestaudio";

    SpeechSdk tts_sdk,speech_sdk;
    tts_sdk.init(popts,sdktype_tts);
    speech_sdk.init(popts,sdktype_speech);
    std::string input;
    while(1)
    {

        speech_sdk.speek(input);
        std::cout << "please input words:";
        std::cin >> input;
    }

    return 0;
}

