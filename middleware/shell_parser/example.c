/*
 * discription: example for The shell parser
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

void help(void *arg)
{
    SYMBOL *sym_tmp;
    _LIST *tmp = &sym_head.list;

    for (; !_is_list_empty(tmp); tmp = tmp->next)
    {
        sym_tmp = _list_entry(tmp->next, SYMBOL, list);
        printf("symbol  --%s\n", sym_tmp->name);
    }
}

void version(void *arg)
{
    printf("No version, TMD\n");
}

void fuck(void *arg)
{
    printf("fuck fuck!!!\n");
}

void shell_thread(void * arg)
{
    U8 i;

    while (TRUE)
    {
        printf("tmd-> ");
        /*catch input string*/
        for (i = 0, buff_cmd[i] = getchar(); buff_cmd[i] != '\n'; i++, buff_cmd[i] = getchar());

        /*input enter key will go to restart*/
        if(buff_cmd[0] == '\n')
            goto restart;

        /*end of input string*/
        buff_cmd[i] = '\0';

        /*parser input string*/
        if ( !parser(buff_cmd) )
            printf("%s: command not found\n", buff_cmd);

        memset(buff_cmd, 0, 128);

restart:
        os_delay(1);
    }
}

#define STACK_SHELL_SIZE 4 * 1024
TCB tcb_shell;
U32 stack_shell[STACK_SHELL_SIZE];
void shell_init()
{
    export(help, (U8 *)"help");
    export(version,(U8 *)"version");
    export(fuck, (U8 *)"fuck");

    task_create(&tcb_shell, (U8 *)"shell_task", shell_thread, NULL, stack_shell, STACK_SHELL_SIZE, 30, 1);
}
