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
#include <lib-utils.h>

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

static char *print_number(char *buf,
                          char *end,
                          long  num,
                          int   base,
                          int   s,
                          int   type)
{
    char c, sign;
    char tmp[16];
    const char *digits;
    static const char small_digits[] = "0123456789abcdef";
    static const char large_digits[] = "0123456789ABCDEF";
    register int i;
    register int size;

    size = s;

    digits = (type & LARGE) ? large_digits : small_digits;
    if (type & LEFT)
        type &= ~ZEROPAD;

    c = (type & ZEROPAD) ? '0' : ' ';

    /* get sign */
    sign = 0;
    if (type & SIGN)
    {
        if (num < 0)
        {
            sign = '-';
            num = -num;
        }
        else if (type & PLUS)
            sign = '+';
        else if (type & SPACE)
            sign = ' ';
    }

    i = 0;
    if (num == 0)
        tmp[i++]='0';
    else
    {
        while (num != 0)
            tmp[i++] = digits[divide(&num, base)];
    }

    size -= i;

    if (!(type&(ZEROPAD | LEFT)))
    {
        if ((sign)&&(size>0))
            size--;

        while (size-->0)
        {
            if (buf <= end)
                *buf = ' ';
            ++ buf;
        }
    }

    if (sign)
    {
        if (buf <= end)
        {
            *buf = sign;
            -- size;
        }
        ++ buf;
    }

    /* no align to the left */
    if (!(type & LEFT))
    {
        while (size-- > 0)
        {
            if (buf <= end)
                *buf = c;
            ++ buf;
        }
    }

    /* put number in the temporary buffer */
    while (i-- > 0)
    {
        if (buf <= end)
            *buf = tmp[i];
        ++ buf;
    }

    while (size-- > 0)
    {
        if (buf <= end)
            *buf = ' ';
        ++ buf;
    }

    return buf;
}

static U32 vsnprintf(char       *buf,
                            U32   size,
                            const char *fmt,
                            __builtin_va_list   args)
{
    U32 num;
    int i, len;
    char *str, *end, c;
    const char *s;

    U8 base;            /* the base of number */
    U8 flags;           /* flags to print number */
    U8 qualifier;       /* 'h', 'l', or 'L' for integer fields */
    U32 field_width;     /* width of output field */

    str = buf;
    end = buf + size - 1;

    /* Make sure end is always >= buf */
    if (end < buf)
    {
        end  = ((char *)-1);
        size = end - buf;
    }

    for (; *fmt ; ++fmt)
    {
        if (*fmt != '%')
        {
            if (str <= end)
                *str = *fmt;
            ++ str;
            continue;
        }

        /* process flags */
        flags = 0;

        while (1)
        {
            /* skips the first '%' also */
            ++ fmt;
            if (*fmt == '-') flags |= LEFT;
            else if (*fmt == '+') flags |= PLUS;
            else if (*fmt == ' ') flags |= SPACE;
            else if (*fmt == '#') flags |= SPECIAL;
            else if (*fmt == '0') flags |= ZEROPAD;
            else break;
        }

        /* get field width */
        field_width = -1;
        if (isdigit(*fmt)) field_width = skip_atoi(&fmt);
        else if (*fmt == '*')
        {
            ++ fmt;
            /* it's the next argument */
            field_width = va_arg(args, int);
            if (field_width < 0)
            {
                field_width = -field_width;
                flags |= LEFT;
            }
        }

        /* get the conversion qualifier */
        qualifier = 0;

        if (*fmt == 'h' || *fmt == 'l')
        {
            qualifier = *fmt;
            ++ fmt;
        }

        /* the default base */
        base = 10;

        switch (*fmt)
        {
        case 'c':
            if (!(flags & LEFT))
            {
                while (--field_width > 0)
                {
                    if (str <= end) *str = ' ';
                    ++ str;
                }
            }

            /* get character */
            c = (U8)va_arg(args, int);
            if (str <= end) *str = c;
            ++ str;

            /* put width */
            while (--field_width > 0)
            {
                if (str <= end) *str = ' ';
                ++ str;
            }
            continue;

        case 's':
            s = va_arg(args, char *);
            if (!s) s = "(NULL)";

            len = strlen(s);

            if (!(flags & LEFT))
            {
                while (len < field_width--)
                {
                    if (str <= end) *str = ' ';
                    ++ str;
                }
            }

            for (i = 0; i < len; ++i)
            {
                if (str <= end) *str = *s;
                ++ str;
                ++ s;
            }

            while (len < field_width--)
            {
                if (str <= end) *str = ' ';
                ++ str;
            }
            continue;

        case 'p':
            if (field_width == -1)
            {
                field_width = sizeof(void *) << 1;
                flags |= ZEROPAD;
            }

            str = print_number(str, end,
                               (long)va_arg(args, void *),
                               16, field_width, flags);
            continue;

        case '%':
            if (str <= end) *str = '%';
            ++ str;
            continue;

            /* integer number formats - set up the flags and "break" */
        case 'o':
            base = 8;
            break;

        case 'X':
            flags |= LARGE;
        case 'x':
            base = 16;
            break;

        case 'd':
        case 'i':
            flags |= SIGN;
        case 'u':
            break;

        default:
            if (str <= end) *str = '%';
            ++ str;

            if (*fmt)
            {
                if (str <= end) *str = *fmt;
                ++ str;
            }
            else
            {
                -- fmt;
            }
            continue;
        }

        if (qualifier == 'l')
        {
            num = va_arg(args, U32);
            if (flags & SIGN) num = (S32)num;
        }
        else if (qualifier == 'h')
        {
            num = (U16)va_arg(args, S32);
            if (flags & SIGN) num = (S16)num;
        }
        else
        {
            num = va_arg(args, U32);
            if (flags & SIGN) num = (S32)num;
        }

        str = print_number(str, end, num, base, field_width, flags);
    }

    if (str <= end) *str = '\0';
    else *end = '\0';

    /* the trailing null byte doesn't count towards the total
    * ++str;
    */
    return str - buf;
}

U32 snprintf(char *buf, unsigned long size, const char *fmt, ...)
{
    U32 n;
    va_list args;

    va_start(args, fmt);
    n = vsnprintf(buf, size, fmt, args);
    va_end(args);

    return n;
}
