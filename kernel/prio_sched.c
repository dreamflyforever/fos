#include "include/var_define.h"

TCB* bit_first_one_search(U32 num)
{
    U8 i; 
    LIST *prio_list_head;
    TCB *tmp;

    i = 0;
    while(i < 32)
    {
        if ((num >> i) & 0x01) 
        {   
            prio_list_head = &task_prio_queue[i].list;
            tmp            = list_entry( prio_list_head->next, TCB, list);
            
            if (tmp->state == TRUE)
                return tmp;
        }

        i++;
    }
    
    if (old_task->state == 0)
        return idle_task;
    
    return old_task;
}


void prio_ready_queue_init()
{
    U8 i = 0;
    while(i<32){

        list_init(&task_prio_queue[i].list);
        i++;
    }
}

void prio_ready_queue_insert(TCB *tcb)
{
    list_insert_behind(&task_prio_queue[tcb->prio].list, &tcb->list);
}

void prio_ready_queue_insert_head(TCB *tcb)
{
    list_insert_spec(&task_prio_queue[tcb->prio].list, &tcb->list);
}

void prio_ready_queue_delete(TCB *tcb)
{
    list_delete(&tcb->list);   
}

U8 prio_ready_queue_fetch()
{
    TCB *tmp =  bit_first_one_search(task_prio_map);

    /* task have two state
     * state == 0, if the hight priority task equal current task,
     * switch to old task, else return, not schedule.
     * state == 1, see the flow.
     * return value: 0 schedule
     *               1 not schedule
     **/
    if (!tmp->state)
    {
        if (new_task->prio == tmp->prio){
#if DEBUG    
            os_printf("-----------%0x\n", old_task);
#endif
            if (old_task == NULL)
                old_task = &idle_tcb;
            new_task = old_task;
            old_task = tmp;
            return 0;
        }
        return 1;
    }
   
    if (new_task->prio == tmp->prio){
#if DEBUG
        os_printf("old_task->prio == %d--------\n", tmp->prio);
#endif
        if (is_list_last(&task_prio_queue[new_task->prio].list))
            return 1;
    }

    old_task = new_task;
    new_task = tmp;
#if DEBUG
    os_printf("old_task->prio == %d\n", old_task->prio);
    os_printf("new_task->prio == %d\n", new_task->prio);
#endif
    return 0;
}

void schedule()
{
    U8 judge;

    if (schedule_is_lock == TURE)
        return ;
   
    judge = prio_ready_queue_fetch();
    if (judge){
        return ;
    }

    __asm__ ("l.sys 0");
    __asm__ ("l.nop");
}

void start_which_task(TCB *first_task)
{
    new_task = first_task;
    extern void start_schedule();
    start_schedule();
}

void schedule_lock()
{
    U32 cpu_sr;
    cpu_sr = interrupt_disable();
    schedule_is_lock = TURE;
    interrupt_enable(cpu_sr);    
}

void schedule_unlock()
{
    U32 cpu_sr;
    cpu_sr = interrupt_disable();
    schedule_is_lock = FLASE;
    interrupt_enable(cpu_sr); 
}
