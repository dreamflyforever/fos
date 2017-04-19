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

#include <var_define.h>

void msg_block_queue_init(QUEUE * msg_block_queue_msg_head)
{
	list_init(&msg_block_queue_msg_head->list);
}

void msg_block_queue_insert(QUEUE * msg_queue)
{
	list_insert_behind(&msg_queue->list, &new_task->list);
}

void msg_block_queue_delete(TCB * tcb)
{
	list_delete(&tcb->list);
}

U8 msg_queue_create(QUEUE * entry, U32 length, U8 * name)
{
	if ((entry == NULL) || (length <= 0)) {
		os_printf("Message create error\n");
		return FALSE;
	}

	U32 cpu_sr = interrupt_disable();

	/*Init message block queue */
	msg_block_queue_init(entry);

	/*Build message queue */
	list_init(&entry->msg_head);
	entry->length = length;
	entry->count = 0;
	entry->name = name;

	interrupt_enable(cpu_sr);
	return TRUE;
}

/*
 * method: FIFO or LIFO;
 */
U8 msg_put(QUEUE * entry, MSG * msg, U8 method)
{
	if ((entry == NULL)
		|| (msg == NULL)
		|| (entry->count > entry->length)) {
		os_printf("Message put in queue error\n");
		return FALSE;
	}

	TCB *tcb_tmp;
	U8 retval = TRUE;

	U32 cpu_sr = interrupt_disable();

	/*Check tasks block on the message queue list */
	if (!is_list_last(&entry->list)) {
		tcb_tmp = list_entry(entry->list.next, TCB, list);
		msg_block_queue_delete(tcb_tmp);
		prio_ready_queue_insert_head(tcb_tmp);
		os_printf("release block task\n");
	}

	if (entry->count == entry->length) {
		/*XXX Todo: if queue is full, block the task */
		// block_queue(new_task);
		os_printf("Message queue overflow, leak the message\n");
		retval = FULL;
	} else {
		os_printf("[name:msg] [%s:%s]\n", entry->name, msg->buff);
		entry->count++;
		if (method == FIFO)
			list_insert_behind(&entry->msg_head, &msg->list);
		else
			/*Last come first server. */
			list_insert_spec(&entry->msg_head, &msg->list);
	}
	interrupt_enable(cpu_sr);
	/*XXX:Maybe don't schedule? */
	//schedule();
	return retval;
}

U8 msg_get(QUEUE * entry, void *buffer)
{
	if ((entry == NULL) || (buffer == NULL)) {
		os_printf("Message get in queue error\n"); return FALSE; }
	U32 cpu_sr = interrupt_disable();
	U8 retval = TRUE;
	/*If no message in the queue, then blcok the task */
	if (is_list_last(&entry->msg_head)) {
		prio_ready_queue_delete(new_task);
		msg_block_queue_insert(entry);
		retval = EMPTY;
		os_printf("block task\n");
	} else {
		/*Fetch message */
		MSG *msg = list_entry(entry->msg_head.next, MSG, list);
		memcpy(buffer, msg->buff, 5);
		/*
		 * Delete message XXX: but the message do not be included more
		 * than two queue list, because system is not ready to select
		 * the queue list to delete message.
		 */
		list_delete((&(entry->msg_head))->next);
		if (entry->count > 0)
			entry->count--;
	}

	interrupt_enable(cpu_sr);
	/*Maybe don't schedule? */
	if (retval == EMPTY)
		schedule();
	return retval;
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
	U8 *buffer = (U8 *) malloc(10);
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
