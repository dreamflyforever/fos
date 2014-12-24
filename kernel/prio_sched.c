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

/*Search the ready-high-priority task*/
TCB* bit_first_one_search(U32 num)
{
    LIST *prio_list_head;
    TCB *tcb_ret;

    U8 i; 

    for (i = 0; i < SYSTEM_WORD; i++)
    { 
        /* The first task of each priority queue must be representative*/
        if ((num >> i) & 0x01)
        {
            prio_list_head = &task_prio_queue[i].list;
            tcb_ret        = list_entry(prio_list_head->next, TCB, list);

            return tcb_ret;
        }
    }

    if (old_task->state == NON_RUNNING_STATE)
        return &idle_tcb;

    return old_task;
}

/*Build the ready queue*/
void prio_ready_queue_init()
{
    U8 i;
    for (i = 0; i < SYSTEM_WORD; i++)
        list_init(&task_prio_queue[i].list);
}

void prio_ready_queue_insert_tail(TCB *tcb)
{
    bit_set(task_prio_map, tcb->prio);
    list_insert_behind(&task_prio_queue[tcb->prio].list, &tcb->list);
}

void prio_ready_queue_insert_head(TCB *tcb)
{
    bit_set(task_prio_map, tcb->prio);
    list_insert_spec(&task_prio_queue[tcb->prio].list, &tcb->list);
}

void prio_ready_queue_delete(TCB *tcb)
{
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
        tmp = tmp->next;
        i++;
        os_printf("i = %d\n", i);
        os_printf("prio = %d\n", tcb->prio);
        tcb_tmp = list_entry(tmp->next, TCB, list);
        os_printf("task-name = %s\n", tcb_tmp->name);
    }
#endif
}

U8 prio_ready_queue_fetch()
{
    TCB *tmp =  bit_first_one_search(task_prio_map);

    if (!is_list_last(&tmp->list))
    {
        prio_ready_queue_delete(tmp);
        prio_ready_queue_insert_tail(tmp);
    }

    /*if the fetch task equel current task*/
    if (tmp == new_task)
        return NO_SCHED;

    old_task = new_task;
    new_task = tmp;

    return SCHED;
}

void schedule()
{
    if (schedule_is_lock == TURE)
        return ;

    U32 cpu_sr = interrupt_disable();
    if (NO_SCHED == prio_ready_queue_fetch())
    {
        interrupt_enable(cpu_sr);
        return ;
    }
    //os_printf("old_task->name == %s\n", old_task->name);
    //os_printf("new_task->name == %s\n", new_task->name);
 //os_printf("%d %s %d\n", __LINE__, __FUNCTION__, __FILE__);
    __asm__ ("l.sys 0");
    __asm__ ("l.nop");
    interrupt_enable(cpu_sr);
}

BOOL start_which_task(TCB *first_task)
{
    if (first_task == NULL)
    {
        os_printf("error: First task is NULL\n");
        return FALSE;
    }

    new_task = first_task;

    start_schedule();

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

void find_high_ready_task()
{
    U32 cpu_sr = interrupt_disable();
    prio_ready_queue_fetch();
    interrupt_enable(cpu_sr);
}
