#ifndef SIGLELIST_H
#define SIGLELIST_H

#include "common.h"
#include "tools/linklist/linklist.h"


class siglelist
{
public:
    siglelist();
    ~siglelist();
    static siglelist *getInstance()
    {
        if(lisInstance == NULL)
            lisInstance = new siglelist;
    }

private:
    static siglelist * lisInstance;

};

#endif // SIGLELIST_H
