/*
 * Protothread implement
 * cutdown memory need, only 2 bytes to keep the thread next address,
 * but not keep the context, so be careful to use it.
*/

#define PT_CREATE(p, arg) \
	if (p == NULL) { \
		return 0; \
	} \
	p(arg);

#define BEGIN 0
#define BLOCK 2

#define PT_INIT() \
	static int event = BEGIN;\
	switch (event) \
{\
	case BEGIN: \
		    {

/*notic: use `\` means on the one line*/
#define PT_BLOCK(condition) \
		    }\
	event = __LINE__;\
	break; \
	case __LINE__: \
		       {\
	if (!condition)	\
		return BLOCK\


#define PT_DEINIT() \
		       }\
	break; \
	default:\
		print("event: %d\n", event); \
	break;\
}

#define PT_RESUME(p) \
	p(NULL);


#if 0
#define print(format, ...) \
	{printf("[%s : %s : %d] ", \
		__FILE__, __func__, __LINE__); \
	printf(format, ##__VA_ARGS__);}

/*test potothread*/
#include "stdio.h"
int phread_one(void *arg)
{
	PT_INIT();
	print("block\n")
		PT_BLOCK(1);
	print("run\n");
	PT_DEINIT();
	return 0;
}

int phread_two(void *arg)
{
	PT_INIT();
	print("run\n");
	PT_RESUME(phread_one);
	print("resume\n");
	PT_DEINIT();
	return 0;
}

int main()
{
	int event = BEGIN;
	PT_CREATE(phread_one, &event);
	PT_CREATE(phread_two, &event);
	return 0;
}
#endif
