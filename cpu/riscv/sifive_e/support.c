/*
 * Copyright (c) 2020, Shanjin Yang.<sjyangv0@qq.com>
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
 * The latest version of FOS download by <https://github.com/dreamflyforever/fos>
 *
 */

#include "var_define.h"

void trap_handler();
extern char _bss_start;
extern char _bss_end;
extern char _memory_end;

typedef struct cpu_context_st {
	uint32_t          mepc;
	uint32_t          mstatus;
	union { uint32_t  x1,  ra; };
	union { uint32_t  x3,  gp; };
	union { uint32_t  x4,  tp; };
	union { uint32_t  x5,  t0; };
	union { uint32_t  x6,  t1; };
	union { uint32_t  x7,  t2; };
	union { uint32_t  x8,  s0, fp; };
	union { uint32_t  x9,  s1; };
	union { uint32_t x10,  a0; };
	union { uint32_t x11,  a1; };
	union { uint32_t x12,  a2; };
	union { uint32_t x13,  a3; };
	union { uint32_t x14,  a4; };
	union { uint32_t x15,  a5; };
	union { uint32_t x16,  a6; };
	union { uint32_t x17,  a7; };
	union { uint32_t x18,  s2; };
	union { uint32_t x19,  s3; };
	union { uint32_t x20,  s4; };
	union { uint32_t x21,  s5; };
	union { uint32_t x22,  s6; };
	union { uint32_t x23,  s7; };
	union { uint32_t x24,  s8; };
	union { uint32_t x25,  s9; };
	union { uint32_t x26, s10; };
	union { uint32_t x27, s11; };
	union { uint32_t x28,  t3; };
	union { uint32_t x29,  t4; };
	union { uint32_t x30,  t5; };
	union { uint32_t x31,  t6; };
} cpu_context_t;

void *stack_init(STACK *stack_base, uint32_t stack_size,
		TASK_ENTRY entry, void *arg, void *task_exit)
{
	STACK *stk;
	cpu_context_t *regs = 0;

	stk = (uint32_t *)&stack_base[stack_size];
	stk = (uint32_t *)((uint32_t)(stk) & 0xFFFFFFFC);

	stk  -= (sizeof(cpu_context_t)/sizeof(uint32_t));

	regs = (cpu_context_t*) stk;

	for(int i=1; i<(sizeof(cpu_context_t)/sizeof(uint32_t)); i++) {
		*(stk + i) = 0xACEADD00 | ((i / 10) << 4) | (i % 10);
	}

	volatile uint32_t gp;
	asm("mv %0, gp":"=r"(gp));

	regs->gp        = (uint32_t)gp;           /*global pointer*/
	regs->a0        = (uint32_t)arg;          /*argument*/
	regs->ra        = (uint32_t)task_exit;         /*return address*/
	regs->mstatus   = (uint32_t)0x00001880;   /*return to machine mode and enable interrupt*/
	regs->mepc      = (uint32_t)entry;        /*task entry*/

	return (STACK*)stk;
}

void *_memset(void *dest, int c, int n)
{
	char *p = dest;
	while (n-- > 0) {
		*(char*)dest++ = c;
	}
	return p;
}

void riscv_init()
{
	memset(&_bss_start, 0, &_bss_end - &_bss_start);
}
