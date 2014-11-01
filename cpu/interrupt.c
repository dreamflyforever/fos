/*
 * Copyright (c) 2013, Shanjin Yang.<sjyangv0@gmail.com>
 * All rights reserved.
 *
 * This file is part of the FOS.
 *
 * The latest version of FOS download by <https://github.com/yangshanjin/YSJ_OS>
 *
 */

#include <var_define.h>
#include <hw_include.h>

#if 0
U32 int_stack[32][4*100];
TCB int_tcb[32];
#endif

/* Disable or1200 interrupt */
U32 interrupt_disable(void)
{
    U32 cpu_sr;
 
    cpu_sr = mfspr(SPR_SR);
    mtspr(SPR_SR, cpu_sr & ~(SPR_SR_IEE | SPR_SR_TEE));

    return cpu_sr;
}

/* Enable or1200 interrupt */ 
void interrupt_enable(U32 cpu_sr)
{
    mtspr(SPR_SR, cpu_sr); /* | (SPR_SR_IEE | SPR_SR_TEE) */
}

/* Interrupt handlers table */
struct ihnd int_handlers[MAX_INT_HANDLERS];

/* Initialize interrupt handlers table routine */
int hw_interrupt_init()
{
    int i;

    for(i = 0; i < MAX_INT_HANDLERS; i++){
        int_handlers[i].handler = NULL;
        int_handlers[i].arg = NULL;
      }

    mtspr(SPR_PICMR, 0x00000000);
    /* set OR1200 to accept exceptions */
    mtspr(SPR_SR, mfspr(SPR_SR) | SPR_SR_IEE);

    return 0;
}

#if 0
/* Add interrupt handler */ 
int hw_interrupt_install_by_prio(unsigned long vect, void (* handler)(void *), void *prio)
{
    if(vect >= MAX_INT_HANDLERS)
        return -1;

    int_handlers[vect].handler = handler;
    int_handlers[vect].arg = prio;

    mtspr(SPR_PICMR, mfspr(SPR_PICMR) | (0x00000001L << vect));

    return 0;
}
#endif

/* Add interrupt handler */ 
int hw_interrupt_install(unsigned long vect, void (* handler)(void *), void *arg)
{
    if(vect >= MAX_INT_HANDLERS)
        return -1;

    int_handlers[vect].handler = handler;
    int_handlers[vect].arg = arg;

    mtspr(SPR_PICMR, mfspr(SPR_PICMR) | (0x00000001L << vect));

    return 0;
}

/* Disable interrupt */ 
int hw_interrupt_mask(unsigned long vect)
{
    if(vect >= MAX_INT_HANDLERS)
        return -1;

    mtspr(SPR_PICMR, mfspr(SPR_PICMR) & ~(0x00000001L << vect));

    return 0;
}

/* Enable interrupt */ 
int hw_interrupt_unmask(unsigned long vect)
{
    if(vect >= MAX_INT_HANDLERS)
        return -1;

    mtspr(SPR_PICMR, mfspr(SPR_PICMR) | (0x00000001L << vect));

    return 0;
}

/* Main interrupt handler */
void usr_handle()
{
    /* process only the interrupts asserted at signal catch, ignore all during process */
    unsigned long picsr = mfspr(SPR_PICSR);
    unsigned int i = 0;

    for (; i < MAX_INT_HANDLERS; i++){

        if((picsr & (0x01L << i)) && (int_handlers[i].handler != NULL)) {

            int_handlers[i].handler(int_handlers[i].arg); 
        }
    }

    mtspr(SPR_PICSR, 0);    //clear interrupt status: all modules have level interrupts, which have to be cleared by software,
                            //thus this is safe, since non processed interrupts will get re-asserted soon enough
}

