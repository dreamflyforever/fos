#include "include/block.h"


void block_queue_init()
{
    list_init(&block_queue.list);
}

void block_queue_insert(BLOCK *block)
{
    list_insert_behind(&block_queue.list, &block->list);         
}

void block_queue_delete(BLOCK *block)
{
    list_delete(&block->list);    
}


