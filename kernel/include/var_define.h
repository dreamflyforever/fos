#ifndef __VAR_DEFIEN__
#define __VAR_DEFINE__

#include "list.h"
#include "../../cpu/include/hw_include.h"
#include "../../libc/include/lib.h"
#include "../../libc/include/printf.h"
#include "queue.h"

#define SYSTEM_BYTE 32
#define TURE  1
#define FLASE 0
#define DEBUG 0

/*for timer function*/
#define SOFTWARE_TIMER 0
#define CYCLE 1

/*for delay*/
#define DELAY 2
void os_delay(U32 timeslice);

typedef  void (*TASK_ENTRY)(void *p_arg);
typedef  void (*FUNC_PTR)(void *p_arg);

#define STACK unsigned int 

/*for task control block*/
typedef struct TCB_STR{
    STACK *stack_ptr;
    LIST   list;
    U8     prio;
    BOOL   state; // Only two state, run or forbit run
}TCB;

/*time manage*/
typedef struct TICK_STR{
    LIST list;
    U8   style; // timer,delay,wait signer
    U32  timeout;
    TCB  *tcb;
    FUNC_PTR func; // for software timer
    void *func_arg;
    BOOL  period;
    U32  timeout_copy;
}TICK;

/*for semaphore*/
typedef struct SEM_STR{
    LIST list;
    U32 count;
    U8  *name;
    TCB *tcb;
}SEM;

#define PRIO_MUTEX 1
typedef struct MUTEX{
   LIST list;
   BOOL flag;
   U8 *name;
   U32 copy_prio;
   TCB *tcb; 
}MUTEX;

void task_create(TCB *tcb,  TASK_ENTRY fun, STACK *stack, U8 prio, BOOL state);

void sem_block_queue_init();
void sem_init(SEM *semaphore, U8 *name, U32 num);
void sem_put(SEM *semaphore);
void sem_get(SEM *semaphore);

void mut_block_queue_init();
void mut_init(MUTEX *mutex, U8 *name);
void mut_put(MUTEX *mutex);
void mut_get(MUTEX *mutex);

void block_queue_init();

extern MUTEX mut_block_queue;
extern SEM sem_block_queue;
extern BOOL schedule_is_lock;
extern TCB task_prio_queue[SYSTEM_BYTE];
extern TCB *new_task;
extern TCB *old_task;
extern U32 task_prio_map;

extern TCB idle_tcb;
extern U32 idle_stack[4*100];
extern U32 task_prio_map;

#define bit_clear(num, i) num = num & (~(1<<i))
#define bit_set(num, i)   num = num | (1<<i)

TCB* bit_first_one_search(U32 num);
void prio_ready_queue_init();
void prio_ready_queue_insert(TCB *tcb);
void schedule();
void prio_ready_queue_delete(TCB *tcb);
void prio_ready_queue_insert_head(TCB *tcb);


void tick_queue_init();

void start_which_task(TCB *tcb);
void idle_task(void *arg);
void schedule_lock();
void schedule_unlock();

#endif
