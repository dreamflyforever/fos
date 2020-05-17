/*
 * Copyright (c) 2013, Shanjin Yang.<sjyangv0@gmail.com>
 * All rights reserved.
 *
 * This file is part of the FOS.
 *
 * The latest version of FOS download by <https://github.com/yangshanjin/YSJ_OS>
 *
 */

#ifndef __LIB_H__
#define __LIB_H__

#ifndef NULL
#define NULL ((void *)0)
#endif

/* memcpy */
void *fos_memcpy( void* s1, void* s2, unsigned int n);

/* strlen */
unsigned int fos_strlen(const char*s);  

/* memchr */
void *fos_memchr(const void *s, int c, unsigned int n);

/* Seed for LFSR function used in rand() */
/* This seed was derived from running the LFSR with a seed of 1 - helps skip the
   first iterations which outputs the value shifting through. */
/* Pseudo-random number generation */
unsigned long int fos_rand ();

void *fos_memset(void *src, unsigned char byte, unsigned int count);

int fos_strcmp(const char *cs, const char *ct);

#endif
