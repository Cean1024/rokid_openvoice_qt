#include "linklist.h"

LinkList::LinkList()
{
    if( !pool ) {
        //pool = new MemoryPool(BUFSIZE,MEMPOOLDEFCOUNT,MEMPOOLBUFCOUNT);
        pool = new MemoryPool(MEMPOOLBUFSIZE);
        CreateList();
    }
}

LinkList::LinkList(int size)
{
    if( !pool ) {
        pool = new MemoryPool(size,MEMPOOLDEFCOUNT,MEMPOOLBUFCOUNT);
        CreateList();
    }
}

LinkList::~LinkList()
{
    if(pool)
        delete pool;
}

r_status LinkList::CreateList()
{
    head.next = &tail;
    tail.next=NULL;
    return SUCCESS;
}
listnode_d * LinkList::CreateNode()
{
    listnode_d *  node = NULL;
    node = new listnode_d;
    if(node) {

        node->buf = reinterpret_cast <char*>(pool->get());
        node->size = pool->blockSize();

    }

    return node;
}
void  LinkList::Release(listnode_d *data)
{
    if(data){
        pool->release(data->buf);
        delete data;
    }
    data = NULL;
}

r_status LinkList::Insert(listnode_d *data)
{
    if(!data) return ERROR;
    if(tail.next == NULL) {
        head.next = data;
        tail.next = data;
        data->next =NULL;
    } else {
        tail.next->next = data;
        tail.next =data;
        data->next =NULL;
    }
    return SUCCESS;
}
r_status LinkList::get(listnode_d **data)
{
    if( head.next == &tail) return ERROR;
    if(head.next->next != NULL ) {
        (*data)= head.next;
        head.next = head.next->next;
        (*data)->next =NULL;
    } else {
        (*data) = head.next;
        head.next = &tail;
        tail.next = NULL;
    }
    return SUCCESS;
}
