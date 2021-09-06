#ifndef __Q_H__
#define __Q_H__

#include "_list.h"
typedef struct queue_str {
	LIST head;
	U32 size;
	U8 *name;
	U8 index;
	U32 count;
} queue_t;

typedef struct msg_str {
	LIST list;
	U8 type;
	U32 length;
	U8 *buf;
} msg_t;

U8 msg_init(queue_t **obj, char *name, U32 size);
U8 msg_deinit(queue_t *obj);

U8 msg_put_buf(queue_t *obj, char *buf, U32 size);
U32 msg_get_buf(queue_t *obj, char *buf, U32 size);

/*debug info*/
U8 print_len(char *buf, int size);

#if 1
#define print(format, ...) \
	{printf("[%s : %d] ", \
	__func__, __LINE__); \
	printf(format, ##__VA_ARGS__);}
#else
#define print(format, ...) 
#endif

#endif
