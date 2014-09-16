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


void mut_init(MUTEX *mutex, U8 *name)
{
    if (mutex == NULL) return ; 
    mutex->name = name;
    mutex->flag = TRUE;
}

void mut_get(MUTEX *mutex)
{
    if (mutex == NULL) return ;

    if (mutex->flag == TRUE)
    {
        mutex->flag = FALSE;
        mutex->copy_prio = new_task->prio;
        new_task->prio = PRIO_MUTEX;
        return ;
    }
    
    mutex->tcb = new_task;
    mutex->tcb->state = 0;   
    prio_ready_queue_delete(mutex->tcb);
    mut_block_queue_insert(mutex);    
}

void mut_put(MUTEX *mutex)
{
    if (mutex == NULL) return ;
    mutex->flag = TRUE;

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
                return ;
            }
        }
        
}
