/*
 * Copyright (c) 2013, Shanjin Yang.<sjyangv0@gmail.com>
 * All rights reserved.
 *
 * This file is part of the FOS.
 *
 * The latest version of FOS download by <https://github.com/yangshanjin/YSJ_OS>
 *
 */

#ifndef _PRINTF_H_
#define _PRINTF_H_

int sprintf(char* str, const char *fmt, ...);

#define PRINTFBUFFER_SIZE 512
extern char PRINTFBUFFER[PRINTFBUFFER_SIZE]; // Declare a global printf buffer

int printf_to_uart(const char *fmt, ...);
int printf_to_sim(const char *fmt, ...);

int putchar_to_uart(int c);
int putchar_to_sim(int c);

int puts_to_uart(const char *str);
int puts_to_sim(const char *str);

/* for os_printf */
//#define os_printf printf_to_uart
#define putchar putchar_to_uart
#define puts puts_to_uart

#endif
