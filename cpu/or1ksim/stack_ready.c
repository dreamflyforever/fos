/*
 * Copyright (c) 2013, Shanjin Yang.<sjyangv0@gmail.com>
 * All rights reserved.
 *
 * This file is part of the FOS.
 *
 * The latest version of FOS download by <https://github.com/yangshanjin/YSJ_OS>
 *
 */

#include <hw_include.h>

void *stack_init( STACK_COPY *p_stk_base, U32 stk_size, TASK_ENTRY p_task, void
		  * arg, TASK_EXIT task_exit)
{
    U32 *stk;
    U32 temp = (U32)(p_stk_base + stk_size - 128);
    stk = (U32  *)temp;
    stk--;
    *stk-- = (U32)31;                     /* r31 = 0  */
    *stk-- = (U32)30;                     /* r30 = 0  */
    *stk-- = (U32)29;                     /* r29 = 0  */
    *stk-- = (U32)28;                     /* r28 = 0  */
    *stk-- = (U32)27;                     /* r27 = 0  */
    *stk-- = (U32)26;                     /* r26 = 0  */
    *stk-- = (U32)25;                     /* r25 = 0  */
    *stk-- = (U32)24;                     /* r24 = 0  */
    *stk-- = (U32)23;                     /* r23 = 0  */
    *stk-- = (U32)22;                     /* r22 = 0  */
    *stk-- = (U32)21;                     /* r21 = 0  */
    *stk-- = (U32)20;                     /* r20 = 0  */
    *stk-- = (U32)19;                     /* r19 = 0  */
    *stk-- = (U32)18;                     /* r18 = 0  */
    *stk-- = (U32)17;                     /* r17 = 0  */
    *stk-- = (U32)16;                     /* r16 = 0  */
    *stk-- = (U32)15;                     /* r15 = 0  */
    *stk-- = (U32)14;                     /* r14 = 0  */
    *stk-- = (U32)13;                     /* r13 = 0  */
    *stk-- = (U32)12;                     /* r12 = 0  */
    *stk-- = (U32)11;                     /* r11 = 0 return value */
    *stk-- = (U32)10;                     /* r10 = 0 */
    *stk-- = (U32)task_exit;              /* r09 = 0 link register, task_exit is for task return*/
    *stk-- = (U32)8;                      /* r08 = 0 function paramters*/
    *stk-- = (U32)7;                      /* r07 = 0 */
    *stk-- = (U32)6;                      /* r06 = 0 */
    *stk-- = (U32)5;                      /* r05 = 0 */
    *stk-- = (U32)4;                      /* r04 = 0 */
    *stk-- = (U32)arg;                      /* r03 = arg0 */
    *stk-- = (U32)2;                      /* r02 = frame pointer */
    *stk-- = (U32)(SPR_SR_IEE | SPR_SR_TEE | SPR_SR_SM); /* status word */
    *stk   = (U32)(p_task);                /* program counter */

    return ((U32 *)stk);
}

void port_schedule()
{
    __asm__ ("l.sys 0");
    __asm__ ("l.nop");
}
