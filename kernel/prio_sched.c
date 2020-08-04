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

#define MAPTABLE 1
/*drop the pin of fisrt bit one*/
static unsigned int  const  bitmap[256] = {
	0u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, /* 0x00 to 0x0F */
	4u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, /* 0x10 to 0x1F */
	5u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, /* 0x20 to 0x2F */
	4u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, /* 0x30 to 0x3F */
	6u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, /* 0x40 to 0x4F */
	4u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, /* 0x50 to 0x5F */
	5u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, /* 0x60 to 0x6F */
	4u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, /* 0x70 to 0x7F */
	7u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, /* 0x80 to 0x8F */
	4u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, /* 0x90 to 0x9F */
	5u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, /* 0xA0 to 0xAF */
	4u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, /* 0xB0 to 0xBF */
	6u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, /* 0xC0 to 0xCF */
	4u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, /* 0xD0 to 0xDF */
	5u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, /* 0xE0 to 0xEF */
	4u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u  /* 0xF0 to 0xFF */
};

static unsigned char y, c[8] = {0};

void bitmap_set(int prio)
{
	unsigned char a, b;
	a = prio & 0x7;
	b = prio >> 3;
	c[b] |= 1 << a;
	y = y | 1 << b;
}

int bitmap_get()
{
	unsigned char a, b, prio;
	b = bitmap[y];
	a = bitmap[c[b]];
	prio = b*8 + a;
	return prio;
}

void bitmap_clear(int prio)
{
	unsigned char a, b;
	a = prio & 0x7;
	b = prio >> 3;
	c[b] &= ~(1 << a);
	if (c[b] == 0)
		y = y & ~(1 << b);
}

/*Search the ready-high-priority task*/
TCB *bit_first_one_search(U32 num)
{
	LIST *prio_list_head;
	TCB *tcb_ret;

	U8 i;
#if MAPTABLE
	i = bitmap_get();
	prio_list_head = &task_prio_queue[i].list;
	tcb_ret = list_entry(prio_list_head->next, TCB, list);
	/*return the first found task */
	return tcb_ret;
#else
	for (i = 0; i < SYSTEM_WORD; i++) {
		/*The first task of each priority queue must be representative*/
		if ((num >> i) & 0x01) {
			prio_list_head = &task_prio_queue[i].list;
			tcb_ret = list_entry(prio_list_head->next, TCB, list);
			/*return the first found task */
			return tcb_ret;
		}
	}
#endif
	/*If not ready task, then return NULL */
	tcb_ret = NULL;
	return tcb_ret;
}

/*Build the ready queue*/
void prio_ready_queue_init()
{
	U8 i;
	for (i = 0; i < SYSTEM_WORD; i++)
		list_init(&task_prio_queue[i].list);
}

void prio_ready_queue_insert_tail(TCB * tcb)
{
	OS_ASSERT(tcb);
#if MAPTABLE
	bitmap_set(tcb->prio);
#else

	bit_set(task_prio_map, tcb->prio);
#endif
	list_insert_behind(&task_prio_queue[tcb->prio].list, &tcb->list);
}

void prio_ready_queue_insert_head(TCB * tcb)
{
	OS_ASSERT(tcb);
#if MAPTABLE
	bitmap_set(tcb->prio);
#else
	bit_set(task_prio_map, tcb->prio);
#endif
	list_insert_spec(&task_prio_queue[tcb->prio].list, &tcb->list);
}

void prio_ready_queue_delete(TCB * tcb)
{
	OS_ASSERT(tcb);
	list_delete(&tcb->list);

	/* If the task ready queue have no task, clear the corresponding task_prio_map */
	if (is_list_last(&task_prio_queue[tcb->prio].list)) {
#if MAPTABLE
		bitmap_clear(tcb->prio);
#else

		bit_clear(task_prio_map, tcb->prio);
#endif
	}
}

U8 prio_ready_queue_fetch()
{
	TCB *tmp = bit_first_one_search(task_prio_map);

	/*If not ready task, then assert */
	OS_ASSERT(tmp);

	if (!is_list_last(&tmp->list)) {
		prio_ready_queue_delete(tmp);
		prio_ready_queue_insert_tail(tmp);
	}

	/*if the fetch task is equal to current task, no sched */
	if (tmp == new_task) {
		return NO_SCHED;
	} else {
		/*Switch the task */
		old_task = new_task;
		new_task = tmp;
	}

	return SCHED;
}

/*schedule task*/
void schedule()
{
	if (schedule_is_lock == TRUE)
		return;

	U32 cpu_sr = interrupt_disable();
	if (NO_SCHED == prio_ready_queue_fetch()) {
		interrupt_enable(cpu_sr);
		return;
	}

	port_schedule();
	interrupt_enable(cpu_sr);
}

/*Start the first task*/
BOOL start_which_task(TCB * first_task)
{
	OS_ASSERT(first_task);
	if (first_task == NULL) {
		OS_LOG("First task is NULL\n");
		return FALSE;
	}

	new_task = first_task;
	start_schedule(new_task);

	return TRUE;
}

void schedule_lock()
{
	schedule_is_lock = TRUE;
}

void schedule_unlock()
{
	schedule_is_lock = FALSE;
}

void find_high_ready_task()
{
	U32 cpu_sr = interrupt_disable();
	prio_ready_queue_fetch();
	interrupt_enable(cpu_sr);
}
