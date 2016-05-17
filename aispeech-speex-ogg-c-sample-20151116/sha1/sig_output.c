#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void sha1(void *message, int size, char sig[40]);

char *sig_output(char *text)
{
	char *digest = (char *)malloc(41);
	memset(digest, 0, 41);
	sha1(text, strlen(text), digest);
	printf("digest: %.*s\n", 41, digest);
	return digest;
}
