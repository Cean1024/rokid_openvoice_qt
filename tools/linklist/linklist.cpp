#include "linklist.h"

LinkList::LinkList()
{

        //pool = new MemoryPool(BUFSIZE,MEMPOOLDEFCOUNT,MEMPOOLBUFCOUNT);
        pool = new MemoryPool(MEMPOOLBUFSIZE);
        CreateList();

}

LinkList::LinkList(int size)
{

        pool = new MemoryPool(size);
        CreateList();

}

LinkList::~LinkList()
{
    if(pool)
        delete pool;
}

r_status LinkList::CreateList()
{
    head = nullptr;
    tail = nullptr;
    return SUCCESS;
}
listnode_d * LinkList::CreateNode()
{
    listnode_d *  node = nullptr;
    node = new listnode_d;
    if(node) {
        node->buf = reinterpret_cast <char*>(pool->get());
        node->size = pool->blockSize();
        node->next=nullptr;
    }
    return node;
}
void  LinkList::Release(listnode_d *data)
{
    if(data) {
        pool->release(data->buf);
        delete data;
    }
    data = nullptr;
}

r_status LinkList::clean()
{
    listnode_d *node;
    while( head != tail && tail != nullptr) {
        node = head;
        head = head->next;
        Release(node);
    }

    Release(head);
    head=nullptr;
    tail=nullptr;

    return SUCCESS;
}

r_status LinkList::Insert(listnode_d *data)
{

    if(!data) return ERROR;
    if(tail != nullptr ) {

        tail->next = data;
        tail=data;

    } else {

        tail = data;
        head =data;

    }

    tail->next=nullptr;

    return SUCCESS;
}
r_status LinkList::get(listnode_d **data)
{
    if(  head == nullptr ) return ERROR;

        (*data)= head;
        head = head->next;
        if(head==nullptr)tail=nullptr;

    return SUCCESS;
}
