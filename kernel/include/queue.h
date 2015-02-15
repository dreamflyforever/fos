#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <list.h>

/*Last In, First Out*/
#define LIFO 0
/*First In, First Out*/
#define FIFO 1

typedef struct QUEUE_STR{
    /*For block task*/
    LIST list;
    /*For message list*/
    LIST msg_head;
    U32  length;
    /*Judge message max number*/
    U32  count;
    U8   *name;
}QUEUE;

typedef struct MSG_STR{
    LIST list;
    U8   type;
    U32  length;
    U8   *buff;
}MSG;

void msg_queue_create(QUEUE *entry, U32 length, U8 *name);

void msg_put(QUEUE *entry, MSG *msg, U8 method);

void msg_get(QUEUE *entry, void *buffer);

#endif
