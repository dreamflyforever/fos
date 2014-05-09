#include "../kernel/include/var_define.h"
#include "../cpu/include/hw_include.h"


U32 stack1[4*100];
U32 stack2[4*100];
U32 stack3[4*100];
U32 stack4[4*100];
TCB tcb1;
TCB tcb2;
TCB tcb3;
TCB tcb4;

SEM sem;


void task_creat(TCB *tcb,  TASK_ENTRY fun, STACK *stack, U8 prio, BOOL state)
{
    tcb->stack_ptr = stack_init(stack, 4*100, fun);
    tcb->prio      = prio;
    tcb->state     = state;
    bit_set(task_prio_map, prio);
    prio_ready_queue_insert(tcb);
}

void fun1(void *arg)
{
    U32 i = 0;
    while (1)
    {
        i = i + 2;
        os_printf("task1 print i = %d\n", i);
        sem_put(&sem);
        //cpu = interrupt_disable();
        schedule();
        //os_delay(3);
        //interrupt_enable(cpu);
    }
}

void fun2(void *arg)
{
    //os_printf("fun2 print\n");
    U32 cpu;
    while (1)
    {
        //cpu = interrupt_disable();
        os_printf("task2 runing\n");
        sem_get(&sem);
        schedule();
        //interrupt_enable(cpu);
        //while (1);
    };
}

void fun3(void *arg)
{
    U32 cpu;
    while (1)
    {
        os_printf("task3 runing\n");
        //schedule();
        os_delay(5);
    }
}

void fun4(void *arg)
{
    U32 i = 0;
    while (1)
    {
        i++;
        os_printf("task4 print i = %d\n", i);
        //sem_get(&sem);
        //os_delay(5);
        //schedule();
        os_delay2(10);
        os_printf("after 100 handle\n");
    }
}


void main()
{
    uart_init();

    os_printf("FOS\n");
    
    prio_ready_queue_init();

    tick_queue_init();


    old_task = NULL;

    task_creat(&tcb1, fun1, stack1, 5, 1);
    task_creat(&tcb2, fun2, stack2, 3, 1);
    task_creat(&tcb3, fun3, stack3, 1, 1);
    task_creat(&tcb4, fun4, stack4, 1, 1);
    task_creat(&idle_tcb, idle_task, idle_stack, 31, 1);
    hw_timer_init();
 
    sem_init(&sem, 1);
    //extern void test_tick();
    //test_tick();
    
    /*Never reach here*/
    start_which_task(&tcb1); 
    while (1)
    {
        os_printf("hello");
        ;
    };
}




