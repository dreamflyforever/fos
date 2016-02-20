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

/*The priority ceiling algorithm*/
#include <var_define.h>

void mutex_block_queue_init(MUTEX * mutex_block_queue_head)
{
	list_init(&mutex_block_queue_head->list);
}

void mutex_block_queue_insert(MUTEX * mutex)
{
	list_insert_behind(&mutex->list, &mutex->tcb->list);
}

void mutex_block_queue_delete(TCB * tcb)
{
	list_delete(&tcb->list);
}

/*
 * Mutex init default is enable got.
 */
U8 mutex_init(MUTEX * mutex, const U8 * name)
{
	if (mutex == NULL) {
		OS_LOG("Mutex null\n");
		return NO_MUTEX;
	}

	/*Init mutex block queue */
	mutex_block_queue_init(mutex);

	mutex->name = name;
	mutex->enable = 1;
	return TRUE;
}

/*
 * If mutex is enable got, change the task priority as default set, else block
 * the task.
 */
U8 mutex_get(MUTEX * mutex)
{
	if (mutex == NULL) {
		OS_LOG("Mutex null\n");
		return NO_MUTEX;
	}

	U32 cpu_sr = interrupt_disable();

	/*If mutex disable, then block the task and switch task. */
	if (!mutex->enable) {
		mutex->tcb = new_task;
		prio_ready_queue_delete(new_task);
		mutex_block_queue_insert(mutex);
		schedule();
	} else {
		/*For recover task priority */
		mutex->copy_prio = new_task->prio;

		/*Changing the task priority, so the other task preempt to get the mutex will fail */
		prio_ready_queue_delete(new_task);
		/*Priority ceiling */
		new_task->prio = PRIO_MUTEX;
		prio_ready_queue_insert_head(new_task);
		mutex->enable = 0;
	}

	interrupt_enable(cpu_sr);
	return TRUE;
}

/*
 * Release the mutex, recover the task priority, the first task that block on the mutex
 * queue will be remove from the queue.
 */
U8 mutex_put(MUTEX * mutex)
{
	if (mutex == NULL) {
		OS_LOG("Mutex null\n");
		return NO_MUTEX;
	}

	U32 cpu_sr = interrupt_disable();

	/*Change the task priority, recover the copy-priority */
	prio_ready_queue_delete(new_task);
	new_task->prio = mutex->copy_prio;
	prio_ready_queue_insert_head(new_task);
	mutex->enable = 1;

	TCB *tcb_tmp;
	/*Check tasks block on the mutex queue */
	if (!is_list_last(&mutex->list)) {
		tcb_tmp = list_entry(mutex->list.next, TCB, list);
		mutex_block_queue_delete(tcb_tmp);
		prio_ready_queue_insert_head(tcb_tmp);
	}

	interrupt_enable(cpu_sr);
	schedule();
	return TRUE;
}

/*Todo: Judge the mutex belong to which mutex-block-queue, and delete it.XXX*/
U8 mutex_delete(MUTEX * mutex)
{
	return TRUE;
}
