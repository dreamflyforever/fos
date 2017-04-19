#ifndef __LIST_H_A_
#define __LIST_H_A_

#include <var_define.h>

#ifndef NULL
#define NULL (void *)0
#endif

typedef struct _LIST {
	struct _LIST *next;
} _LIST;

#define _list_entry(node, type, member)    ((type *)((U8 *)(node) - (U32)(&((type *)0)->member)))

void _list_init(_LIST * node);

void _list_insert(_LIST * head, _LIST * node);

int _is_list_empty(_LIST * node);

#endif
