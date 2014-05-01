#ifndef __LIST_H__
#define __LIST_H__

#define U32 unsigned int 
#define U8  unsigned char
#define BOOL U8

#define list_entry(node, type, member)    ((type *)((U8*)(node) - (U32)(&((type *)0)->member)))

typedef struct LIST{
    struct LIST *prev;
    struct LIST *next;
}LIST;

void list_init(LIST *head);
void list_insert(LIST *head, LIST *node);
void list_insert_spec(LIST *head, LIST *node);
void list_delete(LIST *node);
BOOL is_list_last(LIST *node);
void list_insert_behind(LIST *head, LIST *node);
/*
 * Test double list function
 * */
#if 0 
typedef struct TEST{
    LIST task_list;
    int num;
    
}TEST;

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
    TEST  *d;
    while (!is_list_last(tmp))
    {
        d = list_entry(tmp, TEST, task_list);
        tmp = tmp->next;
        printf("%d\n", d->num);   
          } 
        d = list_entry(tmp, TEST, task_list);
        printf("%d\n", d->num);
}
#endif

#endif

