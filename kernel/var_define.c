#include "include/var_define.h"

TCB task_prio_queue[SYSTEM_BYTE];
TCB *new_task;
TCB *old_task;

BOOL schedule_is_lock;

U32 task_prio_map;
