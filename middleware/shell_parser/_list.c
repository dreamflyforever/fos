#include "_list.h"

void _list_init(_LIST *node)
{
    node->next = NULL;
}

void _list_insert(_LIST *head, _LIST *node)
{
    node->next = head->next;
    head->next = node;
}

int _is_list_empty(_LIST *node)
{
    return node->next == NULL? 1: 0;
}
