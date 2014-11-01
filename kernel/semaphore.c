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

SEM sem_block_queue;

void sem_block_queue_init()
{
    list_init(&sem_block_queue.list);
}

void sem_block_queue_insert(SEM *semphore)
{
    list_insert_behind(&sem_block_queue.list, &semphore->list);
}

void sem_block_queue_delete(SEM *semphore)
{
    list_delete(&semphore->list);
}

void sem_init(SEM *semaphore, const U8 *name, U32 num)
{
    if (semaphore == NULL)
        return;

    semaphore->count = num;
    semaphore->name  = name;
}

void sem_get(SEM *semaphore)
{
    if (semaphore == NULL)
        return;

    U32 cpu_sr =  interrupt_disable();
    
    semaphore->count--;

    if (semaphore->count < 1){
        semaphore->tcb = new_task;
        semaphore->tcb->state = NON_RUNNING_STATE;
        prio_ready_queue_delete(semaphore->tcb);
        sem_block_queue_insert(semaphore);
        schedule();
    }

    interrupt_enable(cpu_sr);
    return;
}

void sem_put(SEM *semaphore)
{
    if (semaphore == NULL)
        return;

    SEM *sem_tmp;
    LIST *tmp = &sem_block_queue.list;

    U32 cpu_sr =  interrupt_disable();

    while ( !is_list_last(tmp) ){
        sem_tmp = list_entry(tmp->next, SEM, list);
        tmp = tmp->next;

        if (!(strcmp((const char *)sem_tmp->name, (const char *)semaphore->name)))
        {
            sem_tmp->count++;
            if (sem_tmp->count > 0)
            {
                semaphore->tcb->state = CAN_RUNNING_STATE;   
                sem_block_queue_delete(sem_tmp);
                prio_ready_queue_insert_head(sem_tmp->tcb);
                schedule();

                interrupt_enable(cpu_sr);
                return ;
            }
        }
    }
    semaphore->count++;
    
    interrupt_enable(cpu_sr);
}
