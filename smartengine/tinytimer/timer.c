#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include <string.h>
#include "timer.h"

static TIMER head;

int user_timer_create(char *name, int second, FUNC func)
{
	TIMER *t = malloc(sizeof(TIMER));
	t->name = name;
	t->timeout = second;
	t->copy = second;
	t->func = func;
	list_insert_behind(&head.list, &t->list);
	return 0;
}

void sigroutine(int signo)
{
	LIST *tmp = &head.list;
	TIMER *d;
	while (!is_list_last(tmp)) {
		d = list_entry(tmp->next, TIMER, list);
		tmp = tmp->next;
		if (d->timeout == 0) {
			d->func(0);
			d->timeout = d->copy;
		} else {
			d->timeout--;
		}
	}
}

int timer_init()
{
	list_init(&head.list);
	int second  = 1;
	struct itimerval value, ovalue;

	//printf("process id is %d\n", getpid());
	signal(SIGALRM, sigroutine);
	value.it_value.tv_sec = second;
	value.it_value.tv_usec = 0;
	value.it_interval.tv_sec = second;
	value.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &value, &ovalue);

	return 0;
}

int timer_reset(char *name)
{
	LIST *tmp = &head.list;
	TIMER *d;
	while (!is_list_last(tmp)) {
		d = list_entry(tmp->next, TIMER, list);
		tmp = tmp->next;
		if (0 == strncmp(d->name, name, strlen(name))) {
			d->timeout = d->copy;
			//printf("found %s\n", d->name);
		}
	}
	return 0;
}
#if 0
int main()
{
	list_init(&head.list);
	timer_init();

	user_timer_create("timer1", 5, b);
	user_timer_create("timer2", 10, a);
	for (;;);
	return 0;
}
#endif
