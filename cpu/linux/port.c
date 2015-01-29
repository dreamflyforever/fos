#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define U8    unsigned char
#define U32   unsigned int
#define STACK unsigned char

typedef void (* TASK_ENTRY)(void);

typedef struct tcb_str{
	STACK        * stack_ptr;
	char         * name;
	TASK_ENTRY     entry;
}TCB;

TCB new_tcb;
TCB old_tcb;

void task_exit()
{
	printf("Task return\n");
	while(1);
}

STACK * stack_init(STACK* stack_ptr, U32 stack_size, TASK_ENTRY p_task, void *task_exit)
{
	ucontext_t *stack = (ucontext_t *)malloc(sizeof(ucontext_t));
	assert(stack);
	memset(stack, 0, sizeof(ucontext_t));

	if (getcontext(stack) == -1)
		handle_error("getcontext");

	stack->uc_link = NULL;

	stack->uc_stack.ss_sp = stack_ptr;

	stack->uc_stack.ss_size = stack_size;

	stack->uc_stack.ss_flags = 0;

	makecontext(stack, p_task, 0);

	return (STACK *)stack;
}

U8 task_create(TCB *tcb, U32 stack_size, U8 *name, TASK_ENTRY task, STACK *stack)
{
	if (tcb == NULL || task == NULL || stack == NULL)
		assert(0);
	tcb->stack_ptr = stack_init(stack, stack_size, task, task_exit);
	tcb->name      = name;
	return 0;
}

void schedule()
{
	if (swapcontext( (ucontext_t *)old_tcb.stack_ptr, (ucontext_t *)new_tcb.stack_ptr) == -1){
		handle_error("swapcontext");
	}
}

void start_task(TCB * tcb)
{
	if (setcontext( (ucontext_t *)new_tcb.stack_ptr) == -1)
		handle_error("swapcontext");

}

/*-----------------------------------------------------------------------------------*/

#define STACK_SIZE 4 * 1024
STACK stack1[STACK_SIZE];
STACK stack2[STACK_SIZE];
TCB   tcb1;
TCB   tcb2;
ucontext_t uctx_main;
void task1(void)
{
	printf("%s running\n", new_tcb.name);
	old_tcb = new_tcb;
	new_tcb = tcb2;
	schedule();
	printf("task 1 exiting\n");
}

void task2(void)
{
	assert(new_tcb.name);
	printf("%s running\n", new_tcb.name);
	old_tcb = new_tcb;
	new_tcb = tcb1;
	schedule();
	printf("task 2 exiting\n");
}

void func()
{
	printf("timeout\n");
}

void timer_init()
{	struct itimerval t;
	t.it_interval.tv_usec = 0;
	t.it_interval.tv_sec = 1;
	t.it_value.tv_usec = 0;
	t.it_value.tv_sec = 1;

	if( setitimer( ITIMER_REAL, &t, NULL) < 0 ){
		printf("error\n");
	}
	signal( SIGALRM, func);
}

int main()
{
	timer_init();
	task_create(&tcb1, STACK_SIZE, "task1", task1, stack1);
	task_create(&tcb2, STACK_SIZE, "task2", task2, stack2);
	new_tcb = tcb1;
	start_task(&new_tcb);

	return 0;
}
