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

#ifndef __LIST_H__
#define __LIST_H__

#define U32    unsigned int
#define S32    signed int
#define U16    unsigned short
#define S16    signed short
#define U8     unsigned char
#define S8     signed char
#define BOOL   U8
#define ULONG  unsigned long

#define list_entry(node, type, member)    ((type *)((U8*)(node) - (U32)(&((type *)0)->member)))

typedef struct LIST {
	struct LIST *prev;
	struct LIST *next;
} LIST;

void list_init(LIST * head);
void list_insert(LIST * head, LIST * node);
void list_insert_spec(LIST * head, LIST * node);
void list_delete(LIST * node);
BOOL is_list_last(LIST * node);
void list_insert_behind(LIST * head, LIST * node);

/*
 * Test double list function
 * */
#if 0
typedef struct TEST {
	LIST task_list;
	int num;

} TEST;

TEST a;
TEST b;
TEST c;

void main()
{
	a.num = 10;
	b.num = 20;
	c.num = 30;

	printf("%0x\n", &a.task_list);
	printf("%0x\n", &b.task_list);
	printf("%0x\n", &c.task_list);
	list_init(&a.task_list);
	list_insert(&a.task_list, &b.task_list);
	list_insert_spec(&a.task_list, &c.task_list);
	LIST *tmp = &a.task_list;
	TEST *d;
	while (!is_list_last(tmp)) {
		d = list_entry(tmp, TEST, task_list);
		tmp = tmp->next;
		printf("%d\n", d->num);
	}
	d = list_entry(tmp, TEST, task_list);
	printf("%d\n", d->num);
}
#endif

#endif
