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

/*When task return, the function wii guarantee the system steady operation, but
 * it's incomplete. XXX*/
void task_exit()
{
	os_printf("Task return\n");
	while (1)
		os_delay(100);
}

U8 task_create(TCB * tcb, U8 * name, TASK_ENTRY fun, void *arg, STACK * stack,
	       U32 stack_size, U8 prio, BOOL state)
{
	if (tcb == NULL || fun == NULL || stack == NULL) {
		OS_LOG("Task error")
		    return NO_TCB;
	}

	memset(stack, 0, stack_size);
	tcb->stack_ptr = stack_init(stack, stack_size, fun, arg, task_exit);
	tcb->name = name;
	tcb->prio = prio;
	tcb->state = state;

	prio_ready_queue_insert_tail(tcb);

	return TRUE;
}

/*Task only change it's priority by itself XXX*/
U8 task_prio_change(TCB * tcb, U32 prio)
{
	if ((tcb == NULL) || (prio < 1) || (prio > SYSTEM_WORD)) {
		OS_LOG("Task error")
		    return NO_TCB;
	}

	OS_ASSERT(new_task == tcb);

	prio_ready_queue_delete(tcb);
	tcb->prio = prio;
	prio_ready_queue_insert_head(tcb);

	return TRUE;
}

void task_delete();

void task_stop();

void task_resume();
