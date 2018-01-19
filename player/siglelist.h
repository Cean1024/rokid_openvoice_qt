#ifndef SIGLELIST_H
#define SIGLELIST_H

#include "common.h"
#include "tools/linklist/linklist.h"


enum httpdlstatus {
    httpdl_start = 0,
    httpdl_finish,
    httpdl_stop
};


class siglelist
{

private:
    siglelist();
    ~siglelist();
    httpdlstatus global_pram_http_dl_status;

public:

    static siglelist *getInstance()
    {
        static siglelist instance;   //局部静态变量
         return &instance;
    }
    void sethttpdlstatus(httpdlstatus status);
    httpdlstatus gethttpdlstatus();


};

#endif // SIGLELIST_H
