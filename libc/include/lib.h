#ifndef __LIB_H__
#define __LIB_H__

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef unsigned int size_t;

/* memcpy */
void* memcpy( void* s1, void* s2, size_t n);

/* strlen */
size_t strlen(const char*s);  

/* memchr */
void *memchr(const void *s, int c, size_t n);

/* Seed for LFSR function used in rand() */
/* This seed was derived from running the LFSR with a seed of 1 - helps skip the
   first iterations which outputs the value shifting through. */
/* Pseudo-random number generation */
unsigned long int rand ();

void *memset(void *src, unsigned char byte, unsigned int count);

int strcmp(const char *cs, const char *ct);

#endif
