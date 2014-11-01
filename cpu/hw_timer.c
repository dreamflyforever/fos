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

/*======================[ CPU Tick timer functions ]============================= */

/* Enable tick timer and interrupt generation */
/* Set restart mode */
void hw_timer_init(void){
    // clear timer interrupt
    mtspr(SPR_TTMR, mfspr(SPR_TTMR) & ~(SPR_TTMR_IP));

    // set OR1200 to accept exceptions
    mtspr(SPR_SR, mfspr(SPR_SR) | SPR_SR_TEE);

    //set initial value, and restart mode
    mtspr(SPR_TTMR, SPR_TTMR_IE | SPR_TTMR_RT | ((SYS_CLK/TICKS_PER_SECOND) & SPR_TTMR_TP));

};

/* Disable tick timer and interrupt generation */
void hw_timer_disable(void){
    // disable timer: clear it all !
    mtspr(SPR_SR, mfspr(SPR_SR) & ~SPR_SR_TEE);
    mtspr(SPR_TTMR, 0);

};

/* disable timer interrupt */
void hw_timer_disable_interrupt(void){
    mtspr(SPR_TTMR, mfspr(SPR_TTMR) & ~(SPR_TTMR_IE));
};

/* clear tick interrupt */
void hw_timer_clear_interrupt(void){
    mtspr(SPR_TTMR, mfspr(SPR_TTMR) & ~(SPR_TTMR_IP));
};

/* 

// Reset timer mode register to interrupt with same interval
mtspr(SPR_TTMR, SPR_TTMR_IE | SPR_TTMR_RT | ((SYS_CLK/RAW_TICKS_PER_SECOND) & SPR_TTMR_TP));

 */
