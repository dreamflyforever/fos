#include "include/var_define.h"

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

void sem_init(SEM *semaphore, U8 *name, U32 num)
{
    semaphore->count = num;
    semaphore->name  = name;
}

void sem_get(SEM *semaphore)
{
    schedule_lock();
    semaphore->count--;
    schedule_unlock();

    if(semaphore->count < 1){
        semaphore->tcb = new_task;
        semaphore->tcb->state = 0;
        prio_ready_queue_delete(semaphore->tcb);
        sem_block_queue_insert(semaphore);    
        schedule();
    }
    return;
}

void sem_put(SEM *semaphore)
{
    SEM *sem_tmp;
    LIST *tmp = &sem_block_queue.list;
    while ( !is_list_last(tmp)){
        sem_tmp = list_entry(tmp->next, SEM, list);
        tmp = tmp->next;

        if (!(strcmp(sem_tmp->name, semaphore->name)))
        {
            schedule_lock();
            sem_tmp->count++;
            schedule_unlock();
            if (sem_tmp->count > 0)
            {
                semaphore->tcb->state = 1;   
                sem_block_queue_delete(sem_tmp);
                prio_ready_queue_insert_head(sem_tmp->tcb);
                schedule();
                return ;
            }
        }
        
    }
    schedule_lock();
    semaphore->count++;
    schedule_unlock();    
}
