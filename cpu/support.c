/*
 * Copyright (c) 2013, Shanjin Yang.<sjyangv0@gmail.com>
 * All rights reserved.
 *
 * This file is part of the FOS.
 *
 * The latest version of FOS download by <https://github.com/yangshanjin/YSJ_OS>
 *
 */

#include <hw_include.h>

/* For writing into SPR. */
void mtspr(unsigned long spr, unsigned long value)
{
    asm("l.mtspr\t\t%0,%1,0": : "r" (spr), "r" (value));
}
 
/* For reading SPR. */
unsigned long mfspr(unsigned long spr)
{
    unsigned long value;
    asm("l.mfspr\t\t%0,%1,0" : "=r" (value) : "r" (spr));
    return value;
}
