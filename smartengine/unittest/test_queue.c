#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "q.h"

#define NUM 10
queue_t *obj;
static int _index;
char EXPECT_OUTPUT[NUM][100];
char REAL_OUTPUT[NUM][100];

int INPUT(char *buf, int size)
{
	int retval = 0;
	if (-1 != msg_put_buf(obj, buf, size)) {
		retval = 1;
		memcpy(&EXPECT_OUTPUT[_index][0], buf, size);
		printf("input[%d]: %s\t", _index, &EXPECT_OUTPUT[_index][0]);
		_index++;
	}
	printf("\t");
	return retval;
}

int EXPECT_OUTPUT_QUEUE()
{
	int i;
	char buf[1024] = {0};
	for (i = 0; i < _index; i++) {
		msg_get_buf(obj, buf, 1024);
		memcpy(&REAL_OUTPUT[i][0], buf, 1024);
		printf("real_output[%d]: %s\t", i, &REAL_OUTPUT[i][0]);
		memset(&buf[0], 0, 1024);
	}
	return 0;
}

int COMPARE(char expect_output[][100], char real_output[][100])
{
	int i = 0;
	int pass = 0;
	for (i = 0; i < _index; i++) {
		if (strncmp(expect_output[i],
			real_output[i],
			strlen(&expect_output[i][0])) == 0) {
			pass++;
		}
	}
	printf("\n\nAll Test Done!\n\n");
	printf("\nResult: %2d/%2d [PASS/TOTAL]\n\n", pass, _index);
	return 0;
}

int main()
{
	msg_init(&obj, "helloworld", 100*1024*10);
	printf("\n===============================================================================\n\n");

	INPUT("12345", 5);
	INPUT("abc", 3);
	INPUT("aaaaaaaaaaaaaaa", 10);
	INPUT("bbbbbbbbbbbbbbb", 10);
	printf("\n");
	EXPECT_OUTPUT_QUEUE();
	COMPARE(EXPECT_OUTPUT, REAL_OUTPUT);
	printf("\n===============================================================================\n\n");
	msg_deinit(obj);
	return 0;
}
