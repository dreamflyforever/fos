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

#include "include/var_define.h"

MUTEX mut_block_queue;

void mut_block_queue_init()
{
    list_init(&mut_block_queue.list);
}

void mut_block_queue_insert(MUTEX *mutex)
{
    list_insert_behind(&mut_block_queue.list, &mutex->list);
}

void mut_block_queue_delete(MUTEX *mutex)
{
    list_delete(&mutex->list);
}

void mut_init(MUTEX *mutex, const U8 *name)
{
    if (mutex == NULL)
        return ; 
   
    mutex->name = name;
    mutex->flag = TRUE;
}

void mut_get(MUTEX *mutex)
{
    if (mutex == NULL)
        return ;

    U32 cpu_sr =  interrupt_disable();

    if (mutex->flag == TRUE)
    {
        mutex->flag = FALSE;
        mutex->copy_prio = new_task->prio;
        task_prio_change(new_task, PRIO_MUTEX);

        interrupt_enable(cpu_sr);
        return ;
    }

    mutex->tcb = new_task;
    mutex->tcb->state = 0;   
    prio_ready_queue_delete(mutex->tcb);
    mut_block_queue_insert(mutex);

    interrupt_enable(cpu_sr);
}

void mut_put(MUTEX *mutex)
{
    if (mutex == NULL)
        return ;
   
    mutex->flag = TRUE;

    U32 cpu_sr =  interrupt_disable();

    task_prio_change(new_task, mutex->copy_prio);
    
    MUTEX *mut_tmp;
    LIST *tmp = &mut_block_queue.list;
    while ( !is_list_last(tmp)){
        mut_tmp = list_entry(tmp->next, MUTEX, list);
        tmp = tmp->next;

        if (!(strcmp(mut_tmp->name, mutex->name)))
        {
            mutex->tcb->state = 1;   
            mut_block_queue_delete(mut_tmp);
            prio_ready_queue_insert_head(mut_tmp->tcb);
            schedule();

            interrupt_enable(cpu_sr);
            return ;
        }
    }

    interrupt_enable(cpu_sr);
}
