#ifndef LINKLIST_H
#define LINKLIST_H
#include "Poco/MemoryPool.h"
#include "common.h"



typedef struct listnode {
    char *buf;
    int size;
    struct listnode *next;
}listnode_d;

using Poco::MemoryPool;
class LinkList
{
public:
    LinkList();
    LinkList(int size);
    ~LinkList();
    r_status Insert(listnode_d *data);
    r_status get(listnode_d **data);
    listnode_d * CreateNode();
    void  Release(listnode_d *data);

protected:
    r_status CreateList();

private:
    MemoryPool *pool;
    listnode_d head;
    listnode_d tail;
};

#endif // LINKLIST_H
