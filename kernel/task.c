#include "./include/var_define.h"

void task_create(TCB *tcb,  TASK_ENTRY fun, STACK *stack, U8 prio, BOOL state)
{
    tcb->stack_ptr = stack_init(stack, 4*100, fun);
    tcb->prio      = prio;
    tcb->state     = state;
    prio_ready_queue_insert(tcb);
}

void task_prio_change(TCB *tcb, U32 prio)
{
    prio_ready_queue_delete(tcb);
    bit_clear(task_prio_map, tcb->prio);
    tcb->prio = prio;
    prio_ready_queue_insert_head(tcb);
    bit_set(task_prio_map, tcb->prio);
}

void task_delete();

void task_stop();

void task_resume();
