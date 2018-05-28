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
    readpointer = nullptr;
    return SUCCESS;
}
listnode_d * LinkList::CreateNode()
{
    listnode_d *  node = nullptr;
    node = new listnode_d;
    if(node) {
        try{
        node->buf = reinterpret_cast <char *>(pool->get());
        }catch (std::exception &e) {
            LOGOUT("pool->get:%s",e.what());
            delete node;
            return nullptr;
        }
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
    while ( head != nullptr ) {
        node = head;
        head = head->next;
        Release(node);
    }
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
    if( head == nullptr ) return ERROR;

        (*data) = head;
        head = head->next;
        if ( head == nullptr) tail = nullptr;

    return SUCCESS;
}
r_status LinkList::read(listnode_d **data,rl_status rls)
{
    if(data == nullptr) return FAILED;
    if (rls = read_list) {

        if(readpointer == nullptr) readpointer = head;
        *data = readpointer;
        readpointer=readpointer->next;
        if(readpointer == nullptr)
            return  TILL_THE_END;

    } else if ( rls = read_head )  *data = head;
     else if(rls == read_tail)  *data = tail;
    return SUCCESS;
}
