#include <stdio.h>
#include <string.h>
#include "sha1.c"

int sig_output(char *text)
{
	char digest[40];
	sha1(text, strlen(text), digest);
	printf("digest: %.*s\n", (int)strlen(digest), digest);
	return 0;
}
