/*
 * File      : mem_block.c
 * This file is part of FOS
 * 
 * Copyright (c) 2013, Shanjin Yang.<sjyangv0@gmail.com>
 * All rights reserved.
 *
 * The license and distribution terms for this file: GPL 
 *
 * Author         Date              content
 * 2014-11-8      Shanjin Yang      the first version
 */

/*
     raw os - Copyright (C)  Lingjun Chen(jorya_txj).

    This file is part of raw os.

    raw os is free software; you can redistribute it it under the terms of the 
    GNU General Public License as published by the Free Software Foundation; 
    either version 3 of the License, or  (at your option) any later version.

    raw os is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
    without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program. if not, write email to jorya.txj@gmail.com
                                      ---

    A special exception to the LGPL can be applied should you wish to distribute
    a combined work that includes raw os, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/


/*     2012-4  Created by jorya_txj
  *    xxxxxx   please added here
  */
  

#ifndef __MEM_BLOCK_H__
#define __MEM_BLOCK_H__

typedef struct MEM_POOL
{
    U8   *name;
    /* Define the number of available memory blocks in the pool.  */
    U32  block_pool_available;
    U32  block_size;

    /* Define the head pointer of the available block pool.  */
    U8   *block_pool_available_list;

} MEM_POOL;


U16 block_pool_create(MEM_POOL *pool_ptr, U8  *name_ptr, U32  block_size, void  *pool_start, U32  pool_size);
U16 block_allocate(MEM_POOL *pool_ptr, void **block_ptr);
U16 block_release(MEM_POOL *pool_ptr, void *block_ptr);

#endif
