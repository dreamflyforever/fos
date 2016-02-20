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

TICK tick_queue;
ULONG fos_tick;

void tick_queue_init(void)
{
	list_init(&tick_queue.list);
}

void tick_queue_insert(TICK * tick)
{
	list_insert_behind(&tick_queue.list, &tick->list);
}

void tick_queue_delete(TICK * tick)
{
	list_delete(&tick->list);
}

/*Get system tick*/
ULONG tick_get(void)
{
	return fos_tick;
}

/*System timer*/
void hardware_timer(void)
{
	hw_timer_clear_interrupt();

	U32 cpu = interrupt_disable();

	TICK *tick_tmp;
	LIST *tmp = &tick_queue.list;

	/*System tick plus one */
	fos_tick++;

	/*Traversing the tick list to search which one that is timeout */
	while (!is_list_last(tmp)) {

		tick_tmp = list_entry(tmp->next, TICK, list);
		tmp = tmp->next;

		if (tick_tmp->timeout == TIMEOUT) {

			switch (tick_tmp->style) {
				/*Callback timeout */
			case SOFTWARE_TIMER:
				tick_tmp->func(tick_tmp->func_arg);

				if (tick_tmp->period == CYCLE) {
					tick_tmp->timeout =
					    tick_tmp->timeout_copy;
				} else
					tick_queue_delete(tick_tmp);
				break;

				/*Sleep timeout */
			case DELAY:
				/*Put the delay task to ready queue head */
				tick_queue_delete(tick_tmp);
				prio_ready_queue_insert_tail(tick_tmp->tcb);

				break;
			default:
				OS_LOG("Timeout status error\n");
			}

		} else
			(tick_tmp->timeout)--;
	}

	interrupt_enable(cpu);

	schedule();
}

void timer_req(TICK * timer, FUNC_PTR func, U32 timeout, BOOL period, void *arg)
{
	U32 cpu = interrupt_disable();
	timer->func = func;
	timer->timeout = timeout;
	timer->period = period;
	timer->timeout_copy = timeout;
	timer->style = SOFTWARE_TIMER;
	timer->func_arg = arg;
	tick_queue_insert(timer);
	interrupt_enable(cpu);
}

void os_delay(U32 timeslice)
{
	U32 cpu = interrupt_disable();
	TICK *timer_delay = &new_task->delay;
	timer_delay->tcb = new_task;
	timer_delay->timeout = timeslice;
	timer_delay->timeout_copy = timeslice;
	timer_delay->style = DELAY;

	/*Delete the delay task to ready queue and add to tick-queue */
	prio_ready_queue_delete(timer_delay->tcb);
	tick_queue_insert(timer_delay);
	interrupt_enable(cpu);

	schedule();
}

/*
 * Test softtimer for tick-queue function
 *
 * */
#if 0
void my_timer(void *arg)
{
	os_printf("my_timer runing\n");
}

TICK my_timer_str;

void test_tick(void *arg)
{
	timer_req(&my_timer_str, my_timer, 1, CYCLE, arg);
}
#endif
