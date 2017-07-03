/*
 * File      : event.c
 * This file is part of FOS
 * 
 * Copyright (c) 2013, Shanjin Yang.<sjyangv0@gmail.com>
 * All rights reserved.
 *
 * The license and distribution terms for this file: GPL 
 *
 * Author         Date              content
 * Shanjin Yang   2017-7-3         the first version
 */

#include <stdio.h>
/*system support event number*/
#define EVENT_NUMBER 2

/*the callback function of event*/
typedef int (*CB)(void *arg);
CB func[EVENT_NUMBER];

int event_init()
{
	int i;
	for (i = 0; i < EVENT_NUMBER; i++) {
		func[i] = NULL;
	}
	return 0;
}

int reg(CB user_func, int event)
{
	if ((user_func == NULL) && (event >= EVENT_NUMBER)) {
		printf("error: %s event(%d) >= EVENT_NUMBER(%d)\n",
			__func__, event, EVENT_NUMBER);
		return -1;
	}
	if (func[event] != NULL) {
		printf("attention: event is registered, now overwrite\n");
	}
	func[event] = user_func;

	return 0;
}

int unreg(int event)
{
	if (event >= EVENT_NUMBER) {
		printf("error: %s event(%d) >= EVENT_NUMBER(%d)\n",
			__func__, event, EVENT_NUMBER);
		return -1;
	}

	func[event] = NULL;
	return 0;
}

int send(int event, void *arg)
{
	if (event >= EVENT_NUMBER) {
		printf("error %s event(%d) >= EVENT_NUMBER(%d)\n",
			__func__, event, EVENT_NUMBER);
		return -1;
	}

	if (func[event] != NULL)
		func[event](arg);
	else
		printf("event(%d) not register\n", event);
	return 0;
}
