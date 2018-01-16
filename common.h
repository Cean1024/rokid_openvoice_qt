#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <fstream>
#include <unistd.h>

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
    PLAYWAITING
};

#define DEBUG(format,args...) printf(format,##args)




/*Mempool config*/
#define MEMPOOLBUFSIZE 5120
#define MEMPOOLBUFSIZED (MEMPOOLBUFSIZE + 2000)
#define MEMPOOLBUFCOUNT 200
#define MEMPOOLDEFCOUNT 100

#endif // COMMON_H

