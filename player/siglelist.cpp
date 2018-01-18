#include "siglelist.h"

siglelist::siglelist()
{
    global_pram_http_dl_status = httpdl_stop;
}

siglelist::~siglelist()
{

}
void siglelist::sethttpdlstatus(httpdlstatus status)
{
    global_pram_http_dl_status = status;
}
httpdlstatus siglelist::gethttpdlstatus()
{

    return global_pram_http_dl_status;
}
