#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <fstream>
#include <unistd.h>


#define S_NAME "openvice"
#define S_VERSION "openvice"
enum sdktype {
    sdktype_tts=0,
    sdktype_speech
};

typedef int r_status;

enum returnstatus {

    ERROR = -2,
    FAILED = -1,
    SUCCESS=0,
    PSUCCESS,
    PFAILED,
    FSUCCES,
    FFAILD,
    ISUCCESS,
    IFAILED,
    IGENOR,
    NEEDUPDATA,
    NOTNEEDUPDATA,
    EXIST,
    NOTEXIST,
    WAITING,
    YES,
    NO,
    PLAYSTART,
    PLAYSTOP,
    PLAYRUNING,
    PLAYWAITING,
    TILL_THE_END
};

#define DEBUG(format,args...) printf(format,##args)
#define LOGOUT(format,args...) printf("%s %s " format "\n",S_NAME,__func__,##args)



/*Mempool config*/
#define MEMPOOLBUFSIZE 5120
#define MEMPOOLBUFSIZED (MEMPOOLBUFSIZE + 2000)
#define MEMPOOLBUFCOUNT 200
#define MEMPOOLDEFCOUNT 100


/*device info*/
#define DEVICENAME "keantestaudio"
#define CONFIGFILEPATH \
    "/home/samba/work/rokid_openvoice_sdk/rokid_openvoice_qt/rokid_openvoice_qt/"
#define CONFIGFILENAME "openvoiceconfig.json"


/*play config*/
#define PLAYWAITINGTIME 10

/*loop rev audio set */
#define REV_SAMPLERATE 16000
#define REV_CH 1
#define REV_BITS 16
#define REV_FRAMESIZE (REV_SAMPLERATE/100)
#define REV_AFRAMEBUFSIZE (REV_FRAMESIZE * REV_CH *REV_BITS /8)

#endif // COMMON_H

