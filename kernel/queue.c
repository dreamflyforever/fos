/*
 * Copyright (c) 2013, Shanjin Yang.<sjyangv0@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Shanjin Yang.
 * 4. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the FOS.
 *
 * The latest version of FOS download by <https://github.com/yangshanjin/YSJ_OS>
 *
 */

#include <queue.h>
#include <block.h>
#include <var_define.h>

BLOCK block_queue;

void msg_queue_create(QUEUE *entry, U32 length, U8 *name, U32 id)
{

    U32 cpu_sr =  interrupt_disable();

    list_init(&entry->head);
    entry->length = length;
    entry->name   = name;
    entry->id     = id;

    interrupt_enable(cpu_sr);
}

void msg_put(QUEUE *entry, MSG *msg, U8 method)
{
    BLOCK *block;

    U32 cpu_sr =  interrupt_disable();

    if (!is_list_last(&block_queue.list))
    {
        block = list_entry(block_queue.list.next, BLOCK, list);
        block->tcb->state = 1;
        block_queue_delete(block);
    }
    if (method == FIFO)
        list_insert_behind(&entry->head, &msg->list);
    else
        list_insert_spec(&entry->head, &msg->list);
    //schedule();
    interrupt_enable(cpu_sr);
}


void msg_get(QUEUE *entry, void *buffer)
{
    BLOCK block_tmp;

    U32 cpu_sr =  interrupt_disable();

    if (is_list_last(&entry->head)){

        block_tmp.tcb = new_task;
        if ( block_tmp.tcb->state == 1){
            block_tmp.tcb->state = 0;
            block_queue_insert(&block_tmp);
        }
        schedule();
    }
    MSG *msg = list_entry(entry->head.next, MSG, list);
    memcpy(buffer, msg->buff, 5);
    list_delete(entry->head.next);

    interrupt_enable(cpu_sr);
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
