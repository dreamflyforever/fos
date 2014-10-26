/*
 * Copyright (c) 2013, Shanjin Yang.<sjyangv0@gmail.com>
 * All rights reserved.
 *
 * This file is part of the FOS.
 *
 * The latest version of FOS download by <https://github.com/yangshanjin/YSJ_OS>
 *
 */

#ifndef __HW_TIMER_H_
#define __HW_TIMER_H_

void hw_timer_init(void);
void hw_timer_disable(void);
void hw_timer_disable_interrupt(void);
void hw_timer_clear_interrupt(void);

#endif
