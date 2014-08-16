#include "include/var_define.h"
	
TICK tick_queue;

void tick_queue_init()
{
    list_init(&tick_queue.list);
}

void tick_queue_insert(TICK *tick)
{
    list_insert_behind(&tick_queue.list, &tick->list);         
}

void tick_queue_delete(TICK *tick)
{
    list_delete(&tick->list);    
}

void hardware_timer()
{
    hw_timer_clear_interrupt();

    U32 cpu = interrupt_disable();
    TICK *tick_tmp; 
    LIST *tmp = &tick_queue.list;
    
    while (!is_list_last(tmp)) {       
        
        tick_tmp = list_entry(tmp->next, TICK, list);
        tmp = tmp->next;
       
        if(tick_tmp->timeout == 0)
        {
            if (tick_tmp->style == SOFTWARE_TIMER){
  
                tick_tmp->func(tick_tmp->func_arg);
                
                if (tick_tmp->period == CYCLE){
                    
                    tick_tmp->timeout = tick_tmp->timeout_copy;
                     
                    if(is_list_last(tmp)){

                        schedule();
                        interrupt_enable(cpu);
                        return;
                    }

                }
            }

            if (tick_tmp->style == DELAY)
            {
                tick_tmp->tcb->state = 1;
                
                prio_ready_queue_delete(tick_tmp->tcb);
                prio_ready_queue_insert_head(tick_tmp->tcb);

              
                if(is_list_last(tmp)){

                    tick_queue_delete(tick_tmp);
                    schedule();
                    interrupt_enable(cpu);
                    return;
                }
                
            }
            
            tick_queue_delete(tick_tmp);
        }
        (tick_tmp->timeout)--;
    } 

    schedule();
    interrupt_enable(cpu);
}

void timer_req(TICK *timer, FUNC_PTR func, U32 timeout, BOOL period, void *arg)
{
    timer->func    = func;
    timer->timeout = timeout;
    timer->period  = period;
    timer->timeout_copy = timeout;
    timer->style        = SOFTWARE_TIMER;
    timer->func_arg = arg;
    tick_queue_insert(timer);
}

TICK timer_delay_entry;

void os_delay(U32 timeslice)
{   
    U32 cpu = interrupt_disable();
    TICK *timer_delay       = &timer_delay_entry; 
    timer_delay->tcb        = new_task;
    timer_delay->tcb->state = 0;
    timer_delay->timeout    = timeslice;
    timer_delay->timeout_copy     = timeslice;
    timer_delay->style      = DELAY;
   
    prio_ready_queue_delete(timer_delay->tcb);
    prio_ready_queue_insert(timer_delay->tcb);

    tick_queue_insert(timer_delay);
    schedule();
    
    interrupt_enable(cpu);
}
TICK timer_delay_entry2;

void os_delay2(U32 timeslice)
{   
    U32 cpu = interrupt_disable();
    TICK *timer_delay       = &timer_delay_entry2; 
    timer_delay->tcb        = new_task;
    timer_delay->tcb->state = 0;
    timer_delay->timeout    = timeslice;
    timer_delay->timeout_copy     = timeslice;
    timer_delay->style      = DELAY;
    
    prio_ready_queue_delete(timer_delay->tcb);
    prio_ready_queue_insert(timer_delay->tcb);

    tick_queue_insert(timer_delay);
    schedule();
    
    interrupt_enable(cpu);
}


void my_timer(void  *arg)
{
    os_printf("my_timer runing\n");
}



/*
 * Test softtimer for tick-queue function
 *
 * */
#if 0
TICK my_timer_str;
void test_tick(void *arg)
{     
    timer_req(&my_timer_str, my_timer, 100, 1); 
}
#endif
