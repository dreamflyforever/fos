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

#define msg_queue_test        0
#define sem_test              0
#define tick_tes              0
#define mutex_test            0
#define task_prio_change_test 0 
#define SHELL

STACK stack1[4*100];
STACK stack2[4*100];
STACK stack3[4*100];
STACK stack4[4*100];
TCB tcb1;
TCB tcb2;
TCB tcb3;
TCB tcb4;

#if sem_test 
SEM sem;
#endif

#if mutex_test 
static int mutex_resource;
MUTEX mutex;
#endif

#if msg_queue_test
QUEUE my_queue;
MSG msg1;
MSG msg2;
MSG msg3;
U8 buffer[10];
#endif

void task1(void *arg)
{
    while (1){

#if msg_queue_test
        msg_put(&my_queue, &msg1, FIFO); 
        msg_put(&my_queue, &msg2, FIFO); 
        msg_put(&my_queue, &msg3, FIFO);
#endif

#if sem_test
        sem_put(&sem);
        os_printf("task1 = %d\n", sem.count );
#endif

#if mutex_test
        mut_get(&mutex);
        mutex_resource++;
        os_printf("task1 mutex_resource:%d\n", mutex_resource );
        mut_put(&mutex);
#endif 
        os_printf("task1 running\n");

#if task_prio_change_test
        task_prio_change(&tcb1, 5);
#endif
        os_delay(3);
    }
}

void task2(void *arg)
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
        os_delay(10);
    };
}

void task3(void *arg)
{
    while (1)
    {
        os_printf("task3 runing\n");
        os_delay(5);
    }
}

void task4(void *arg)
{
    while (1)
    {
        os_printf("task4 runing\n");
        os_delay(5);
    }
}

/*init hardware and system resource*/
void system_init()
{
    hw_interrupt_init();

    uart_init();

    os_printf("FOS Copyright by Shanjin Yang\n\n");

    prio_ready_queue_init();

    sem_block_queue_init();

    mut_block_queue_init();

    tick_queue_init();

    block_queue_init();

    old_task = NULL;

    /*create idle task*/
    task_create(&idle_tcb, idle_task, idle_stack, 31, 1);

    hw_timer_init();

    ethoc_initialize(0, 0x92000000);
}

int main(void)
{
    system_init();

    //task_create(&tcb1, task1, stack1, 5, 1);
    //task_create(&tcb2, task2, stack2, 3, 1);
    //task_create(&tcb3, task3, stack3, 3, 1);
    //task_create(&tcb4, task4, stack4, 1, 1);

#ifdef SHELL
    extern void shell_init();
    shell_init();
#endif

#if sem_test
    sem_init(&sem, "sem1", 1);
#endif 

#if mutex_test
    mut_init(&mutex, (U8 *)"mutex1");
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

#if task_prio_change_test
    task_prio_change(&tcb1, 1);
#endif 

    /*which task run first*/
    BOOL result = start_which_task(&idle_tcb);
    if ( !result )
    {
        os_printf("First task is NULL\n");
        return 0;
    }

    /*Never reach here*/
     while (1)
    {
        os_printf("hello");
    };

    return 0;
}
