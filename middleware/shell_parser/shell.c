/*
 * discription: The shell parser
 *
 * Copyright (c) 2013, Shanjin Yang.<sjyangv0@gmail.com>
 * All rights reserved.
 *
 * GPL license
 *
 * author           time         content
 * Shanjin Yang     2014.9.19    first version 
 * Shanjin Yang     2014.10.11   port to FOS 
 */

#include "shell.h"

/*The buff for input commad*/
U8 buff_cmd[128];

SYMBOL sym_head;

void symbol__list_init(SYMBOL *sym)
{
    _list_init(&sym->list);
}

void symbol__list_insert(SYMBOL *sym)
{
    _list_insert(&sym_head.list, &sym->list);
}

SYMBOL malloc[10];
void export(FUNC_PTR func, U8 *name)
{   
    /*the destination of token is just to malloc different memory object*/
    //SYMBOL * token(func) = malloc(sizeof(SYMBOL));  
    static int temp = 0;
    SYMBOL * token(func) = &malloc[temp];
    temp++;  
    token(func)->func = func;
    token(func)->name = name;
    symbol__list_insert(token(func));  
}

int parser(U8 *cmd)
{
    SYMBOL *sym_tmp;
    _LIST *tmp = &sym_head.list;

    for (; !_is_list_empty(tmp); tmp = tmp->next)
    { 
        sym_tmp = _list_entry(tmp->next, SYMBOL, list);
        if (!strcmp((const char *)cmd, (const char *)sym_tmp->name))
        {
            sym_tmp->func(NULL);
            return TURE;
        }
    }
    return FALSE;
}
