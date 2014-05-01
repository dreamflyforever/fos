#include "include/var_define.h"

void sem_init(SEM *semaphore, U32 num)
{
    //semaphore->tcb = new_task;
    semaphore->count = num;
}
void sem_get(SEM *semaphore)
{
    semaphore->tcb = new_task;
    if (semaphore->count > 0)
    {
        semaphore->count--;
        return;
    }
    else
    {
        semaphore->tcb->state = 0;
        schedule();
    }

    return;
}

void sem_put(SEM *semaphore)
{
    semaphore->count++;
    semaphore->tcb->state = 1;   
}
