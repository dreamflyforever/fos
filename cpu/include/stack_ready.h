/*
 * Copyright (c) 2013, Shanjin Yang.<sjyangv0@gmail.com>
 * All rights reserved.
 *
 * This file is part of the FOS.
 *
 * The latest version of FOS download by <https://github.com/yangshanjin/YSJ_OS>
 *
 */

#ifndef __STACK_READY__
#define __STACK_READY__

typedef void (*TASK_ENTRY)(void *p_arg);
typedef void (*TASK_EXIT)(void *p_arg);
#define STACK_COPY unsigned int
#define U32 unsigned int
void *stack_init( STACK_COPY *p_stk_base, U32 stk_size, TASK_ENTRY p_task, TASK_EXIT task_exit);

#endif
