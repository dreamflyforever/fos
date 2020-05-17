/*
 * Copyright (c) 2013, Shanjin Yang.<sjyangv0@gmail.com>
 * All rights reserved.
 *
 * This file is part of the FOS.
 *
 * The latest version of FOS download by <https://github.com/yangshanjin/YSJ_OS>
 *
 */

/* Based C function lib */
#include <lib.h>
#include <var_define.h>

void *fos_memcpy( void* s1, void* s2, unsigned int n)
{
	char* r1 = (char *) s1;
	const char* r2 =  (const char*) s2;
	while (n) {
		*r1++ = *r2++;
		--n;
	}
	return s1;
}

unsigned int fos_strlen(const char*s)
{
	const char* p;
	for (p=s; *p; p++);
	return p - s;
}

void *fos_memchr(const void *s, int c, unsigned int n)
{
	const unsigned char *r = (const unsigned char *) s;
#define np n
	while (np) {
		if (*r == ((unsigned char)c)) {
			return (void *) r;     /* silence the warning */
		}
		++r;
		--np;
	}
	return NULL;
}

unsigned long int fos_rand ()
{
	static unsigned long int lfsr = 0x14b6bc3c;
	static int period = 0;
	/* taps: 32 31 29 1; characteristic polynomial: x^32 + x^31 + x^29 + x + 1 */
	lfsr = (lfsr >> 1) ^ (unsigned long int)((0 - (lfsr & 1u)) & 0xd0000001u);
	++period;
	return lfsr;
}

void *fos_memset(void *src, unsigned char byte, unsigned int count)
{
	char *xs = src;

	while (count--) {

		*xs++ = byte;
	}

	return src;
}

int fos_strcmp(const char *cs, const char *ct)
{
	unsigned char c1, c2;

	while (1) {
		c1 = *cs++;
		c2 = *ct++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
	}
	return 0;
}

#define isdigit(c)  ((unsigned)((c) - '0') < 10)

static int skip_atoi(const char **s)
{
	register int i=0;
	while (isdigit(**s))
		i = i * 10 + *((*s)++) - '0';

	return i;
}
#define ZEROPAD     (1 << 0)    /* pad with zero */
#define SIGN        (1 << 1)    /* unsigned/signed long */
#define PLUS        (1 << 2)    /* show plus */
#define SPACE       (1 << 3)    /* space if plus */
#define LEFT        (1 << 4)    /* left justified */
#define SPECIAL     (1 << 5)    /* 0x */
#define LARGE       (1 << 6)    /* use 'ABCDEF' instead of 'abcdef' */

U32 divide(long int *n, U32 base)
{
	U32 res;

	/* optimized for processor which does not support divide instructions. */
	if (base == 10)
	{
		res = ((U32)*n) % 10U;
		*n = ((U32)*n) / 10U;
	}
	else
	{
		res = ((U32)*n) % 16U;
		*n = ((U32)*n) / 16U;
	}

	return res;
}
