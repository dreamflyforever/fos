/*
 * Copyright (c) 2013, Shanjin Yang.<sjyangv0@gmail.com>
 * All rights reserved.
 *
 * This file is part of the FOS.
 *
 * The latest version of FOS download by <https://github.com/yangshanjin/YSJ_OS>
 *
 */

#include <lib.h>

/* Based C function lib */

void* memcpy( void* s1, void* s2, size_t n)
{
    char* r1 = (char *) s1;
    const char* r2 =  (const char*) s2;
    while (n) {
      *r1++ = *r2++;
      --n;
    }
    return s1;
}

size_t strlen(const char*s)
{
    const char* p;
    for (p=s; *p; p++);
    return p - s;
}

void *memchr(const void *s, int c, size_t n)
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

unsigned long int
rand ()
{
    static unsigned long int lfsr = 0x14b6bc3c;
    static int period = 0;
    /* taps: 32 31 29 1; characteristic polynomial: x^32 + x^31 + x^29 + x + 1 */
    lfsr = (lfsr >> 1) ^ (unsigned long int)((0 - (lfsr & 1u)) & 0xd0000001u); 
    ++period;
    return lfsr;
}

void *memset(void *src, unsigned char byte, unsigned int count)
{
    char *xs = src;

    while (count--) {
        
        *xs++ = byte;
    }
    
    return src;
}

int strcmp(const char *cs, const char *ct)
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
