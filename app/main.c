/*
 * The part of FOS, test FOS api
 * Autor Shanjin Yang
 *
 * */
#include "../kernel/include/var_define.h"

U32 stack1[4*100];
U32 stack2[4*100];
U32 stack3[4*100];
U32 stack4[4*100];
TCB tcb1;
TCB tcb2;
TCB tcb3;
TCB tcb4;

SEM sem;

QUEUE my_queue;
MSG msg1;
MSG msg2;
MSG msg3;
U8 buffer[10];

void fun1(void *arg)
{
    U32 i = 0;
    while (1)
    {
        msg_put(&my_queue, &msg1, FIFO); 
        msg_put(&my_queue, &msg2, FIFO); 
        msg_put(&my_queue, &msg3, FIFO);

        i = i + 2;
        os_printf("task1 print i = %d\n", i);
        //sem_put(&sem);
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
        memset(buffer, 0, 10);
        msg_get(&my_queue, buffer);
        os_printf("first read = %s\n", buffer);
        //cpu = interrupt_disable();
        os_printf("task2 runing\n");
        //sem_get(&sem);
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
 
    msg_queue_create(&my_queue, 1000, "first", 0);
    
    block_queue_init();

    old_task = NULL;

    task_creat(&tcb1, fun1, stack1, 5, 1);
    task_creat(&tcb2, fun2, stack2, 3, 1);
    //task_creat(&tcb3, fun3, stack3, 1, 1);
    //task_creat(&tcb4, fun4, stack4, 1, 1);
    task_creat(&idle_tcb, idle_task, idle_stack, 31, 1);
    hw_timer_init();
 
    sem_init(&sem, 1);
    //extern void test_tick();
    //test_tick();
 
    msg1.buff = "abcd";
    msg2.buff = "abce";
    msg3.buff = "abcf";

    start_which_task(&tcb1); 
      
    /*Never reach here*/
     while (1)
    {
        os_printf("hello");
        ;
    };
}




