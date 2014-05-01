#ifndef __STACK_READY__
#define __STACK_READY__

//#include "../../kernel/include/var_define.h"

typedef void (*TASK_ENTRY_COPY)(void *p_arg);
#define STACK_COPY unsigned int
#define U32 unsigned int
void *stack_init( STACK_COPY *p_stk_base, U32 stk_size, TASK_ENTRY_COPY p_task);

#endif
