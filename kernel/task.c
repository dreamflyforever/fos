#include "./include/var_define.h"
void task_creat(TCB *tcb,  TASK_ENTRY fun, STACK *stack, U8 prio, BOOL state)
{
    tcb->stack_ptr = stack_init(stack, 4*100, fun);
    tcb->prio      = prio;
    tcb->state     = state;
    bit_set(task_prio_map, prio);
    prio_ready_queue_insert(tcb);
}

void task_delect();

void task_resume();

void task_prio_change();

