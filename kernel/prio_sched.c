#include "include/var_define.h"


U32 task_prio_map;

TCB* bit_first_one_search(U32 num)
{
    U8 i   = 0;
    LIST *prio_list_head;
    TCB *tmp;
    while(i < 32)
    {
        if ((num >> i) & 0x01) 
        {   
            prio_list_head = &task_prio_queue[i].list;
            tmp    = list_entry( prio_list_head->next, TCB, list);

            if (tmp->state == 0)
            {
                ;
            }
            else{

                return tmp;
            }
        }
        i++;
    }
    //uart_print_str("No bit set\n");
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
    list_insert_spec(&task_prio_queue[tcb->prio].list, &tcb->list);
}

U8 prio_ready_queue_fetch()
{
    //U8 prio_num = bit_first_one_search(task_prio_map);

    //LIST *prio_list_head = &task_prio_queue[prio_num].list;
    //TCB *tmp    = list_entry( prio_list_head->next, TCB, list);
    TCB *tmp =  bit_first_one_search(task_prio_map);

    /* task have two state
     * one condition: state == 0, if the hight priority task equal now task 
     * , switch to old task, else return, not schedule.
     * the other conditon: state == 1, see the flow.
     * return value: 0 schedule
     *               1 not schedule
     **/
    if (!tmp->state)
    {
        if (new_task->prio == tmp->prio){
            //os_printf("-----------%0x\n", old_task);
            if (old_task == NULL)
                old_task = &idle_tcb;
            new_task = old_task;
            old_task = tmp;
            return 0;
        }
        return 1;
    }
   
    if (new_task->prio == tmp->prio){
        //os_printf("old_task->prio == %d--------\n", tmp->prio);

        return 1;
    }
    old_task = new_task;
    new_task = tmp;

    //os_printf("old_task->prio == %d\n", old_task->prio);
    //os_printf("new_task->prio == %d\n", new_task->prio);
    return 0;
}

void schedule()
{
    U8 judge = prio_ready_queue_fetch();
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
