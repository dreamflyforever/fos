#include "list.h"

#define FIFO 1

typedef struct QUEUE_STR{
    LIST head;
    U32  length;
    U8   *name;
    U32  id;
}QUEUE;

typedef struct MSG_STR{
    LIST list;
    U8   type;
    U32  length;
    U8   *buff;

}MSG;

void queue_create(QUEUE *entry, U32 length, U8 *name, U32 id);

void msg_put(QUEUE *entry, MSG *msg, U8 mathod);

void msg_get(QUEUE *entry, void *buffer);

