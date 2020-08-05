/*
 * File      : mem_block.c
 * This file is part of FOS
 * 
 * Copyright (c) 2013, Shanjin Yang.<sjyangv0@gmail.com>
 * All rights reserved.
 *
 * The license and distribution terms for this file: GPL 
 *
 * date         modified        content
 * 2014-11-8      Shanjin Yang      the first version
 * 2020-08-5      Shanjin Yang      
 */

#include <var_define.h>

U8 mem_create(MEM_POOL *ptr, void *start, U32 sum, U32 block_size)
{
	ptr->size = block_size;
	/*figure up how many block*/
	ptr->valid = sum/block_size;
	/*record the begain address*/
	ptr->valid_ptr = start;
	list_init(&(ptr->head));
}

void *mem_alloca(MEM_POOL *ptr, U32 size)
{
	int n;
	void *address = NULL;
	MEM_BLOCK mb;
	/*figure up how many block need*/
	n = size/ptr->size;
	ptr->valid = ptr->valid - n -1;
	if (ptr->valid > 0) {
		address = ptr->valid_ptr;
		/*figure up next begain address*/
		ptr->valid_ptr = (char *)ptr->valid_ptr + ptr->valid * ptr->size;
		mb.flag = n + 1;
		/*mean block is useless for the next search*/
		mb.if_valid = 0;
		mb.ptr = address;
		list_insert_behind(&(ptr->head), &mb.list);
	}
	return address;
}

void *mem_free(MEM_POOL *ptr, void *address)
{
	LIST *tmp;
	tmp = &(ptr->head);
	MEM_BLOCK *mb;
	/*search the address, and flag available*/
	while (!is_list_last(tmp)) {
		mb = list_entry(tmp->next, MEM_BLOCK, list);
		tmp = tmp->next;
		if (mb->ptr == address) {
			mb->if_valid = 1;
			return TRUE;
		}
	}
	return FALSE;
}

#if 0
int main()
{
	char g_mem[100000];
	MEM_POOL mp;
	mem_create(&mp, &g_mem, 100, 10);
	char *a = mem_alloca(&mp, 9);
	mem_free(&mp, a);
}
#endif
