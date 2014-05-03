#include "include/list.h"
void list_init(LIST *head)
{
    head->next = head;
    head->prev = head;   
}

BOOL is_list_last(LIST *head)
{
    return (head->next == head);
}


void list_insert(LIST *head, LIST *node)
{
    node->next = node;
    node->prev = head;
    head->next = node;
}

void list_insert_spec(LIST *head, LIST *node)
{
    if (is_list_last(head))
    {
        list_insert(head, node);
    }
    
    node->next = head->next;
    node->prev = head;

    head->next->prev = node;
    head->next = node;
}

void list_delete(LIST *node)
{
    if (is_list_last(node))
    {
        node->prev->next = node->prev;
        return ;
    }

    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void list_insert_behind(LIST *head, LIST *node)
{
    LIST *list = head;
    while(!is_list_last(list))
    {
        list = list->next;
    }

    node->next = node;
    node->prev = list;
    list->next = node;
}
