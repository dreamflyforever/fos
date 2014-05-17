#include "include/queue.h"

void queue_create(QUEUE *entry, U32 length, U8 *name, U32 id)
{
    list_init(&entry->head);
    entry->length = length;
    entry->name   = name;
    entry->id     = id;
}

void msg_put(QUEUE *entry, MSG *msg, U8 mathod)
{
    if (mathod == FIFO)
        list_insert_behind(&entry->head, &msg->list);
    else
         list_insert_spec(&entry->head, &msg->list);
}

void msg_get(QUEUE *entry, void *buffer)
{
    //list_entry(entry->head.next, MSG, );
    MSG *msg = list_entry(entry->head.next, MSG, list);  
    //buffer   = msg->buff;
    memcpy(buffer, msg->buff, 5);
    list_delete(entry->head.next);
}

/*
 * Test queue example.
 **/
#if 0
QUEUE my_queue;
void main()
{
    queue_create(&my_queue, 1000, "first", 0);
    MSG msg1;
    MSG msg2;
    MSG msg3;
    msg1.buff = "abcd";
    msg2.buff = "abce";
    msg3.buff = "abcf";
    printf("test read = %s\n", msg1.buff);
    U8 *buffer = (U8 *)malloc(10);
    memset(buffer, 0, 10);
    msg_put(&my_queue, &msg1, 0); 
    msg_put(&my_queue, &msg2, 0); 
    msg_put(&my_queue, &msg3, 0);

    msg_get(&my_queue, buffer);
    printf("first read = %s\n", buffer);
    msg_get(&my_queue, buffer); 
    printf("second read = %s\n", buffer); 
    msg_get(&my_queue, buffer); 
    printf("three read = %s\n", buffer); 

}
#endif
