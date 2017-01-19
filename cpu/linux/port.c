/*
 * Copyright (c) 2015 Shanjin Yang.  All rights reserved.
 *
 *  Shanjin Yang
 *  Shanghai, China
 *  <sjyangv0@qq.com>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at 
 * https://github.com/yangshanjin/fos/blob/master/LICENSE.
 */

#include <assert.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <hw_include.h>

STACK *stack_init(STACK * stack_ptr, U32 stack_size, TASK_ENTRY p_task,
		  void *arg, void *task_exit)
{
	if ((stack_ptr == NULL) || (stack_size <= 0) || (p_task == NULL)
	    || (task_exit == NULL))
		assert(0);

	ucontext_t *stack = (ucontext_t *) malloc(sizeof(ucontext_t));
	assert(stack);
	memset(stack, 0, sizeof(ucontext_t));

	if (getcontext(stack) == -1)
		handle_error("getcontext");

	stack->uc_link = NULL;
	stack->uc_stack.ss_sp = stack_ptr;
	stack->uc_stack.ss_size = stack_size;
	stack->uc_stack.ss_flags = 0;
	makecontext(stack, (void (*)(void))p_task, 1, arg);
	return (STACK *) stack;
}

void port_schedule()
{
	if (swapcontext
	    ((ucontext_t *) old_task->stack_ptr,
	     (ucontext_t *) new_task->stack_ptr) == -1) {
		handle_error("swapcontext");
	}
}

void start_schedule(TCB * tcb)
{
	if (setcontext((ucontext_t *) tcb->stack_ptr) == -1) {
		handle_error("swapcontext");
	}
}

void hw_timer_init()
{
	struct itimerval t;
	t.it_interval.tv_usec = TICKS_PER_SECOND * 100;
	t.it_interval.tv_sec = 0;
	t.it_value.tv_usec = 0;
	t.it_value.tv_sec = 1;

	if (setitimer(ITIMER_REAL, &t, NULL) < 0) {
		printf("error\n");
	}
	signal(SIGALRM, hardware_timer);
}

void hw_interrupt_init()
{
}

void uart_init()
{
}

void interrupt_enable(int sr_cpu)
{
}

int interrupt_disable()
{
	return 0;
}

void hw_timer_clear_interrupt()
{
}
