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

#ifndef __PORT__H__
#define __PORT__H__

#include <var_define.h>
#include <ucontext.h>

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define printf_to_uart printf
#define STACK unsigned int

typedef void (*_TASK_ENTRY)(void);

STACK * stack_init(STACK* stack_ptr, U32 stack_size, _TASK_ENTRY p_task, void *task_exit);

//void port_schedule();
//void start_schedule(TCB * tcb);

void	hw_timer_init();
void	hw_interrupt_init();
void	uart_init();
void	interrupt_enable(int sr_cpu);
int	interrupt_disable();
void	hw_timer_clear_interrupt();

#endif
