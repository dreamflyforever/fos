/*
 * File      : mem_block.c
 * This file is part of FOS
 * 
 * Copyright (c) 2013, Shanjin Yang.<sjyangv0@gmail.com>
 * All rights reserved.
 *
 * The license and distribution terms for this file: GPL 
 *
 * date           modified             content
 * 2014-11-8      Shanjin Yang      the first version
 * 2020-08-5      Shanjin Yang      
 */

#ifndef __MEM_BLOCK_H__
#define __MEM_BLOCK_H__

typedef struct MEM_POOL {
	LIST head;
	U32 size;
	U32 sum;
	U32 valid;
	void *valid_ptr;
} MEM_POOL;

typedef struct MEM_BLOCK {
	LIST list;
	void *ptr;
	U8 if_valid;
	U32 flag;
} MEM_BLOCK;

U8 mem_create(MEM_POOL *ptr, void *start, U32 sum, U32 block_size);

void *mem_alloca(MEM_POOL *ptr, U32 size);

void *mem_free(MEM_POOL *ptr, void *address);
#endif
