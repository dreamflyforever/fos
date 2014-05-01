#include "include/var_define.h"

TCB idle_tcb;
U32 idle_stack[4*100];

void idle_task(void *arg)
{
    while (1)
    {
        os_printf("Enter idle task\n");
        schedule();
    }
}
