 /*
  * Copyright (c) 2013, Shanjin Yang.<sjyangv0@gmail.com>
  * All rights reserved.
  *
  * GPL license
  *
  * author           time         content
  * Shanjin Yang     2014.9.19    first version 
  * Shanjin Yang     2014.10.11   port to FOS 
  */
#ifndef __SHELL_H__
#define __SHELL_H__

#include "_list.h"

#define token(abc) func_##abc

extern U8 buff_cmd[128];
//typedef  void (*FUNC_PTR)(void);

typedef struct SYMBOL {
	_LIST list;
	FUNC_PTR func;
	void *arg;
	U8 *name;
} SYMBOL;

extern SYMBOL sym_head;

void symbol_list_init();
void symbol_list_insert();
void symbol_list_delete();

void export(FUNC_PTR func, U8 * name);

int parser(U8 * cmd);

#endif
