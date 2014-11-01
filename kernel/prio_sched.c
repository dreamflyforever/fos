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

TCB* bit_first_one_search(U32 num)
{
    LIST *prio_list_head;
    TCB *ret;
   
    U8 i = 0; 
    
    U32 cpu_sr =  interrupt_disable();
   
    for (; i < SYSTEM_WORD; i++)
    { 
        /* The first task of each priority queue must be representative, if it
         * can not run, then all task of the queue behind the task can not run
         */
        if ((num >> i) & 0x01)  
        {   
            prio_list_head = &task_prio_queue[i].list;
            ret            = list_entry( prio_list_head->next, TCB, list);
               
            if (ret->state == TRUE) 
                return ret;
        }
    }
    interrupt_enable(cpu_sr);
   
    if (old_task->state == NON_RUNNING_STATE)
        return &idle_tcb;
    
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

void prio_ready_queue_insert_tail(TCB *tcb)
{
    U32 cpu_sr =  interrupt_disable();

    bit_set(task_prio_map, tcb->prio);
    list_insert_behind(&task_prio_queue[tcb->prio].list, &tcb->list);
    
    interrupt_enable(cpu_sr);
}

void prio_ready_queue_insert_head(TCB *tcb)
{
    U32 cpu_sr =  interrupt_disable();
    
    bit_set(task_prio_map, tcb->prio);
    list_insert_spec(&task_prio_queue[tcb->prio].list, &tcb->list);

    interrupt_enable(cpu_sr);
}

void prio_ready_queue_delete(TCB *tcb)
{
    U32 cpu_sr =  interrupt_disable();
    
    list_delete(&tcb->list);

    /* If the task ready queue have no task, clear the corresponding task_prio_map*/
    if (is_list_last(&task_prio_queue[tcb->prio].list)){
        bit_clear(task_prio_map, tcb->prio);
        return ;
    }
#ifdef mutex_debug
    LIST *tmp = &task_prio_queue[tcb->prio].list;
    TCB *tcb_tmp = &task_prio_queue[tcb->prio];
    int i = 0;
    while (!is_list_last(tmp)){
        tmp = task_prio_queue[tcb->prio].list.next;
        i++;
        os_printf("i = %d\n", i);
        os_printf("prio = %d\n", tcb->prio);
        tcb_tmp = list_entry(tmp->next, TCB, list);
        os_printf("tmp rio = %d\n", tcb_tmp->prio);
    }
#endif
    interrupt_enable(cpu_sr);
}

U8 prio_ready_queue_fetch()
{
    U32 cpu_sr =  interrupt_disable();
    
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
            
            return FALSE;
        }
        return TRUE;
    }
   
    if (new_task->prio == tmp->prio){
#if DEBUG
        os_printf("old_task->prio == %d--------\n", tmp->prio);
#endif
        /* Why this operate, I forget */
        if (is_list_last(&task_prio_queue[new_task->prio].list))
            return TRUE;
    }

    interrupt_enable(cpu_sr);
    
    old_task = new_task;
    new_task = tmp;

#if DEBUG
    os_printf("old_task->prio == %d\n", old_task->prio);
    os_printf("new_task->prio == %d\n", new_task->prio);
#endif
    return FLASE;
}

void schedule()
{

    if (schedule_is_lock == TURE)
        return ;
   
    if (prio_ready_queue_fetch()){
        return ;
    }

    __asm__ ("l.sys 0");
    __asm__ ("l.nop");
}

BOOL start_which_task(TCB *first_task)
{
    if (first_task == NULL)
    {
        os_printf("error: First task is NULL\n");
        return FALSE;
    }

    new_task = first_task;
    
    U32 cpu_sr = interrupt_disable();
    
    start_schedule();
    
    interrupt_enable(cpu_sr);

    return TRUE;
}

void schedule_lock()
{
    schedule_is_lock = TURE;
}

void schedule_unlock()
{
    schedule_is_lock = FLASE;
}
