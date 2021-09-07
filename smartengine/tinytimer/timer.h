#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include <string.h>

#include "_list.h"

typedef void (*FUNC)(int);

typedef struct TIMER {
	char *name;
	LIST list;
	int timeout;
	int copy;
	FUNC func;
} TIMER;

int timer_reset(char *name);

int timer_init();
int user_timer_create(char *name, int second, FUNC func);
int timer_deinit();
