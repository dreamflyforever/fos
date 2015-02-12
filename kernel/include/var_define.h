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

#ifndef __VAR_DEFINE__
#define __VAR_DEFINE__

#include <list.h>
#include <hw_include.h>

#include <printf.h>
#include <queue.h>
#include <mem_block.h>

#define LINUX 1
#if LINUX
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define os_printf printf
#endif

#ifndef TRUE
#define TRUE              1
#define FALSE             0
#endif

#define SYSTEM_WORD       32
#define DEBUG             0
#define NON_RUNNING_STATE 0
#define CAN_RUNNING_STATE 1
#define SCHED             1
#define NO_SCHED          0

#define STACK unsigned int

enum{
    NO_SEMAPHORE = 10,
    NO_MUTEX,
    NO_MSG,
    NO_TCB
};
#define IDLE_STACK_SIZE 4 *1024
/*system tick*/
#define TICKS_PER_SECOND 100

/*for timer function*/
#define SOFTWARE_TIMER 0
#define CYCLE          1
#define TIMEOUT        0

#define OS_ASSERT(X)                                                   \
if (!(X))                                                              \
{                                                                      \
    os_printf("(%s) is wrong at %s: %d\n", #X, __FUNCTION__, __LINE__);\
    while(1);                                                          \
}

#define OS_LOG(X)                                                      \
if (!(X))                                                              \
{                                                                      \
    os_printf("log: (%s) is wrong at %s: %d\n", #X, __FUNCTION__,      \
    __LINE__);                                                         \
}

/*for delay*/
#define DELAY          2
void os_delay(U32 timeslice);

typedef  void (*TASK_ENTRY)(void *);
typedef  void (*FUNC_PTR)(void *p_arg);

/*time manage*/
typedef struct TICK_STR{
    LIST list;
    U8   style; // timer,delay,wait signer
    U32  timeout;
    struct TCB_STR  *tcb;
    FUNC_PTR func; // for software timer
    void *func_arg;
    BOOL  period;
    U32  timeout_copy;
}TICK;

/*for task control block*/
typedef struct TCB_STR{
    STACK   *stack_ptr;
    TICK    delay;
    U8      *name;
    LIST    list;
    U8      prio;
    BOOL    state;
}TCB;

/*for semaphore*/
typedef struct SEM_STR{
    LIST       list;
    U32        count;
    const U8   *name;
    TCB        *tcb;
}SEM;

#define PRIO_MUTEX 1
typedef struct MUTEX_STR{
    LIST     list;
    BOOL     flag;
    const U8 *name;
    U32      copy_prio;
    TCB      *tcb;
}MUTEX;

U8 task_create(TCB *tcb, U8 *name, TASK_ENTRY fun, void * arg, STACK *stack, U32 stack_size, U8 prio, BOOL state);
U8 task_prio_change(TCB *tcb, U32 prio);

U8 sem_init(SEM *semaphore, const U8 *name, U32 num);
U8 sem_put(SEM *semaphore);
U8 sem_get(SEM *semaphore);

void mut_block_queue_init();
void mut_init(MUTEX *mutex, const U8 *name);
void mut_put(MUTEX *mutex);
void mut_get(MUTEX *mutex);

void block_queue_init();

extern MUTEX mut_block_queue;
extern SEM sem_block_queue;
extern BOOL schedule_is_lock;
extern TCB task_prio_queue[SYSTEM_WORD];
extern TCB *new_task;
extern TCB *old_task;
extern U32 task_prio_map;
extern ULONG fos_tick;

extern TCB idle_tcb;
extern U32 idle_stack[IDLE_STACK_SIZE];
extern U32 task_prio_map;
//extern void start_schedule();

#define bit_clear(num, i) num = num & (~(1<<i))
#define bit_set(num, i)   num = num | (1<<i)

TCB* bit_first_one_search(U32 num);
void prio_ready_queue_init();
void prio_ready_queue_insert_tail(TCB *tcb);
void schedule();
void prio_ready_queue_delete(TCB *tcb);
void prio_ready_queue_insert_head(TCB *tcb);

void tick_queue_init();
ULONG tick_get();
/*For prot function*/
void hardware_timer();

BOOL start_which_task(TCB *tcb);
void idle_task(void *);
void schedule_lock();
void schedule_unlock();

/*For port function*/
void port_schedule();
void start_schedule(TCB * tcb);

#endif
