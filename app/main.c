/*
 * The part of FOS, test FOS api
 * Autor Shanjin Yang
 *
 * */
#define msg_queue_test 0
#define sem_test 0
#define tick_test 0
#include "../kernel/include/var_define.h"

U32 stack1[4*100];
U32 stack2[4*100];
U32 stack3[4*100];
U32 stack4[4*100];
TCB tcb1;
TCB tcb2;
TCB tcb3;
TCB tcb4;

#if sem_test 
SEM sem;
#endif

#if msg_queue_test
QUEUE my_queue;
MSG msg1;
MSG msg2;
MSG msg3;
U8 buffer[10];
#endif

void fun1(void *arg)
{
    while (1)
    {
#if msg_queue_test
        msg_put(&my_queue, &msg1, FIFO); 
        msg_put(&my_queue, &msg2, FIFO); 
        msg_put(&my_queue, &msg3, FIFO);
#endif

#if sem_test        
        sem_put(&sem);
        os_printf("task1 = %d\n", sem.count );
#endif
        os_printf("task1 running\n" );
        schedule();
    }
}

void fun2(void *arg)
{
    while (1)
    {
#if msg_queue_test
        memset(buffer, 0, 10);
        msg_get(&my_queue, buffer);
        os_printf("first read = %s\n", buffer);
#endif

#if sem_test
        sem_get(&sem);
        os_printf("task2: %d\n", sem.count);
#endif
        os_printf("task2 running\n" );
        os_delay(3);
        schedule();
    };
}

void fun3(void *arg)
{
    while (1)
    {
        os_printf("task3 runing\n");
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
        os_delay2(10);
        os_printf("after 100 handle\n");
    }
}

int main()
{
    uart_init();

    os_printf("FOS\n");
    
    prio_ready_queue_init();

    sem_block_queue_init();
   
    tick_queue_init();

#if msg_queue_test
    msg_queue_create(&my_queue, 1000, "first", 0);
#endif

    block_queue_init();

    old_task = NULL;

    task_creat(&tcb1, fun1, stack1, 5, 1);
    task_creat(&tcb2, fun2, stack2, 3, 1);
    //task_creat(&tcb3, fun3, stack3, 1, 1);
    //task_creat(&tcb4, fun4, stack4, 1, 1);
    task_creat(&idle_tcb, idle_task, idle_stack, 31, 1);
    hw_timer_init();

#if sem_test
    sem_init(&sem, "sem1", 1);
#endif 

#if tick_test    
    extern void test_tick();
    test_tick();
#endif

#if msg_queue_test
    msg1.buff = "abcd";
    msg2.buff = "abce";
    msg3.buff = "abcf";
#endif

    start_which_task(&tcb2); 
      
    /*Never reach here*/
     while (1)
    {
        os_printf("hello");
        ;
    };

    return 0;
}




