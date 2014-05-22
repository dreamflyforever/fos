#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "var_define.h"

typedef struct BLOCK_STR{
    LIST list;
    U8   style; //queue, mailbox, semphpore
    TCB  *tcb;
}BLOCK;

extern BLOCK block_queue;

void block_queue_init();

void block_queue_insert(BLOCK *block);

void block_queue_delete(BLOCK *block);

#endif
