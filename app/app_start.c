#include <var_define.h>

#define msg_queue_test        0
#define sem_test              0
#define mutex_test            0

#define STACK_SIZE            4 * 1024
STACK stack1[STACK_SIZE];
STACK stack2[STACK_SIZE];
STACK stack3[STACK_SIZE];
STACK stack4[STACK_SIZE];
TCB tcb1;
TCB tcb2;
TCB tcb3;
TCB tcb4;

#if sem_test
SEM sem;
#endif

#if mutex_test
MUTEX mutex;
#endif

#if msg_queue_test
QUEUE queue;
QUEUE queue1;
QUEUE queue2;
MSG msg1;
MSG msg2;
MSG msg3;
MSG msg4;
MSG msg5;
MSG msg6;
MSG msg7;
MSG msg8;
MSG msg9;
U8 buffer[10];
#endif

void task1(void *arg)
{
	os_printf("task1 start....\n");
	for (;;) {
#if msg_queue_test
		msg_put(&queue, &msg1, FIFO);
		msg_put(&queue, &msg2, FIFO);
		msg_put(&queue, &msg3, FIFO);
		msg_put(&queue1, &msg4, FIFO);
		msg_put(&queue1, &msg5, FIFO);
		msg_put(&queue1, &msg6, FIFO);
		msg_put(&queue2, &msg7, FIFO);
		msg_put(&queue2, &msg8, FIFO);
		msg_put(&queue2, &msg9, FIFO);
#endif

#if sem_test
		sem_put(&sem);
		os_printf("task1 sem.count = %d\n", sem.count);
#endif

#if mutex_test
		mutex_get(&mutex);
		os_printf("task1 priority: %d\n", new_task->prio);
		schedule();
		os_printf("task1 running\n");
		mutex_put(&mutex);
#endif
		os_delay(300);
	}
}

void task2(void *arg)
{
	os_printf("task2 start....\n");
	for (;;) {
#if msg_queue_test
		memset(buffer, 0, 10);
		if (EMPTY != msg_get(&queue, buffer)) {
			os_printf("queue 0 read = %s\n", buffer);
		} else {
			os_printf("empty\n");
		}
		memset(buffer, 0, 10);
		msg_get(&queue1, buffer);
		os_printf("queue 1 read = %s\n", buffer);
		memset(buffer, 0, 10);
		msg_get(&queue2, buffer);
		os_printf("queue 2 read = %s\n", buffer);
#endif

#if sem_test
		sem_get(&sem);
		os_printf("task2 sem.count = %d\n", sem.count);
#endif

#if mutex_test
		os_printf("task2 running\n");
		mutex_get(&mutex);
		os_printf("task2 priority: %d\n", new_task->prio);
		mutex_put(&mutex);
#endif
		os_delay(100);
	};
}

void task3(void *arg)
{
	os_printf("task1 start....\n");
	for (;;) {
		os_printf("task3 runing\n");
		os_delay(10);
	}
}

void task4(void *arg)
{
	os_printf("task1 start....\n");
	for (;;) {
		os_printf("task4 running\n");
		os_delay(10);
	}
}

void app_main()
{
#if sem_test
	sem_init(&sem, (U8 *) "sem1", 1);
#endif

#if mutex_test
	mutex_init(&mutex, (U8 *) "mutex1");
#endif

#if msg_queue_test
	msg_queue_create(&queue, 2, (U8 *) "queue");
	msg_queue_create(&queue1, 1, (U8 *) "queue1");
	msg_queue_create(&queue2, 1, (U8 *) "queue2");
	msg1.buff = (U8 *) "1aaa";
	msg2.buff = (U8 *) "2bbb";
	msg3.buff = (U8 *) "3ccc";
	msg4.buff = (U8 *) "4aaa";
	msg5.buff = (U8 *) "5bbb";
	msg6.buff = (U8 *) "6ccc";
	msg7.buff = (U8 *) "7aaa";
	msg8.buff = (U8 *) "8bbb";
	msg9.buff = (U8 *) "9ccc";
#endif
	task_create(&tcb1, (U8 *) "task1", task1, NULL, stack1, STACK_SIZE, 3,
		    1);
	task_create(&tcb2, (U8 *) "task2", task2, NULL, stack2, STACK_SIZE, 4,
		    1);
	//task_create(&tcb3, (U8 *)"task3", task3, NULL, stack3, STACK_SIZE, 3, 1);
	//task_create(&tcb4, (U8 *)"task4", task4, NULL, stack4, STACK_SIZE, 1, 1);
}
