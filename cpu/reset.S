#include "include/spr-defs.h"
#include "include/ptrace.h"

/* parameter defines */
#define REDZONE 128
#define STK_FRAME_SIZE (128 + REDZONE)

    .global task_switch
	.global	TimerISR     
    .global UsrISR   
    .extern old_task
    .extern new_task
    .extern hardware_timer
    .extern usr_handle
/* Macro Definitions */
#define CLEAR_GPR(gpr)				\
    l.or    gpr,r0,r0 
		
#define LOAD_SYMBOL_2_GPR(gpr,symbol)		\
	.global symbol;			\
    l.movhi gpr,hi(symbol);	\
    l.ori   gpr,gpr,lo(symbol)

/* Really goes to configurable interrupt handler */
#define EXCEPTION_HANDLER(symbol);               \
	l.addi  r1,r1, -STK_FRAME_SIZE;		\
	l.sw	PT_GPR3(r1), r3;	/* we mass r3 ,so save r3 to stack first */\
	LOAD_SYMBOL_2_GPR(r3,symbol);      \
	l.jr r3;	\
	l.nop	

/* Store the cpu context on the stack */
#define STORE_CONTEXT		;\
	l.sw PT_GPR2(r1) ,r2	;\
	l.sw PT_GPR4(r1) ,r4	;\
	l.sw PT_GPR5(r1) ,r5	;\
	l.sw PT_GPR6(r1) ,r6	;\
	l.sw PT_GPR7(r1) ,r7	;\
	l.sw PT_GPR8(r1) ,r8	;\
	l.sw PT_GPR9(r1) ,r9	;\
	l.sw PT_GPR10(r1) ,r10	;\
	l.sw PT_GPR11(r1) ,r11	;\
	l.sw PT_GPR12(r1) ,r12	;\
	l.sw PT_GPR13(r1) ,r13	;\
	l.sw PT_GPR14(r1) ,r14	;\
	l.sw PT_GPR15(r1) ,r15	;\
	l.sw PT_GPR16(r1) ,r16	;\
	l.sw PT_GPR17(r1) ,r17	;\
	l.sw PT_GPR18(r1) ,r18	;\
	l.sw PT_GPR19(r1) ,r19	;\
	l.sw PT_GPR20(r1) ,r20	;\
	l.sw PT_GPR21(r1) ,r21	;\
	l.sw PT_GPR22(r1) ,r22	;\
	l.sw PT_GPR23(r1) ,r23	;\
	l.sw PT_GPR24(r1) ,r24	;\
	l.sw PT_GPR25(r1) ,r25	;\
	l.sw PT_GPR26(r1) ,r26	;\
	l.sw PT_GPR27(r1) ,r27	;\
	l.sw PT_GPR28(r1) ,r28	;\
	l.sw PT_GPR29(r1) ,r29	;\
	l.sw PT_GPR30(r1) ,r30	;\
	l.sw PT_GPR31(r1) ,r31	;

/* Restore the cpu context from the stack */
#define RESTORE_CONTEXT	    ;\
	l.lwz r2 ,PT_GPR2(r1)   ;\
	l.lwz r3 ,PT_GPR3(r1)   ;\
	l.lwz r4 ,PT_GPR4(r1)   ;\
	l.lwz r5 ,PT_GPR5(r1)   ;\
	l.lwz r6 ,PT_GPR6(r1)   ;\
	l.lwz r7 ,PT_GPR7(r1)   ;\
	l.lwz r8 ,PT_GPR8(r1)   ;\
	l.lwz r9 ,PT_GPR9(r1)   ;\
	l.lwz r10 ,PT_GPR10(r1)	;\
	l.lwz r11 ,PT_GPR11(r1)	;\
	l.lwz r12 ,PT_GPR12(r1)	;\
	l.lwz r13 ,PT_GPR13(r1)	;\
	l.lwz r14 ,PT_GPR14(r1)	;\
	l.lwz r15 ,PT_GPR15(r1)	;\
	l.lwz r16 ,PT_GPR16(r1)	;\
	l.lwz r17 ,PT_GPR17(r1)	;\
	l.lwz r18 ,PT_GPR18(r1)	;\
	l.lwz r19 ,PT_GPR19(r1)	;\
	l.lwz r20 ,PT_GPR20(r1)	;\
	l.lwz r21 ,PT_GPR21(r1)	;\
	l.lwz r22 ,PT_GPR22(r1)	;\
	l.lwz r23 ,PT_GPR23(r1)	;\
	l.lwz r24 ,PT_GPR24(r1)	;\
	l.lwz r25 ,PT_GPR25(r1)	;\
	l.lwz r26 ,PT_GPR26(r1)	;\
	l.lwz r27 ,PT_GPR27(r1)	;\
	l.lwz r28 ,PT_GPR28(r1)	;\
	l.lwz r29 ,PT_GPR29(r1)	;\
	l.lwz r30 ,PT_GPR30(r1)	;\
	l.lwz r31 ,PT_GPR31(r1)	;

/* Linked from 0x0 */
.section .vectors, "ax"

/* ===========================[ exceptions]========================== === */
/* ---[ 0x100: RESET exception ]----------------------------------------- */
	.org 0x100    
	LOAD_SYMBOL_2_GPR(r3, _reset)
	l.jr	r3
	l.nop    
/* ---[ 0x200: BUS exception ]------------------------------------------- */
	.org 0x200
    
/* ---[ 0x300: Data Page Fault exception ]------------------------------- */
	.org 0x300
 
/* ---[ 0x400: Insn Page Fault exception ]------------------------------- */
	.org 0x400
 

/* ---[ 0x500: Timer exception ]----------------------------------------- */
	.org 0x500
	EXCEPTION_HANDLER(TimerISR)    

/* ---[ 0x600: Aligment exception ]-------------------------------------- */
	.org 0x600
	EXCEPTION_HANDLER(_align)
 
/* ---[ 0x700: Illegal insn exception ]---------------------------------- */
	.org 0x700
 
/* ---[ 0x800: External interrupt exception ]---------------------------- */
	.org 0x800
	
	EXCEPTION_HANDLER(UsrISR)
 
 /* ---[ 0x900: DTLB miss exception ]------------------------------------ */
	.org 0x900

/* ---[ 0xa00: ITLB miss exception ]------------------------------------- */
	.org 0xa00

/* ---[ 0xb00: Range exception ]----------------------------------------- */
	.org 0xb00

	
/*c00: Syscall exception ]------------------------------------------------ */
	.org 0xc00
	EXCEPTION_HANDLER(task_switch)	


/* ---[ 0xd00: Trap exception ]------------------------------------------ */
	.org 0xd00

/* ---[ 0xe00: Trap exception ]------------------------------------------ */
	.org 0xe00
 
/* ---[ 0xf00: Reserved exception ]-------------------------------------- */
	.org 0xf00

/* ---[ 0x1000: Reserved exception ]------------------------------------- */
	.org 0x1000

/* ---[ 0x1100: Reserved exception ]------------------------------------- */
	.org 0x1100

/* ---[ 0x1200: Reserved exception ]------------------------------------- */
	.org 0x1200

/* ---[ 0x1300: Reserved exception ]------------------------------------- */
	.org 0x1300

/* ---[ 0x1400: Reserved exception ]------------------------------------- */
	.org 0x1400

/* ---[ 0x1500: Reserved exception ]------------------------------------- */
	.org 0x1500

/* ---[ 0x1600: Reserved exception ]------------------------------------- */
	.org 0x1600

/* ---[ 0x1700: Reserved exception ]------------------------------------- */
	.org 0x1700

/* ---[ 0x1800: Reserved exception ]------------------------------------- */
	.org 0x1800

/* ---[ 0x1900: Reserved exception ]------------------------------------- */
	.org 0x1900

/* ---[ 0x1a00: Reserved exception ]------------------------------------- */
	.org 0x1a00

/* ---[ 0x1b00: Reserved exception ]------------------------------------- */
	.org 0x1b00

/* ---[ 0x1c00: Reserved exception ]------------------------------------- */
	.org 0x1c00

/* ---[ 0x1d00: Reserved exception ]------------------------------------- */
	.org 0x1d00

/* ---[ 0x1e00: Reserved exception ]------------------------------------- */
	.org 0x1e00

/* ---[ 0x1f00: Reserved exception ]------------------------------------- */
	.org 0x1f00

/* ========================================================= [ entry ] === */	
 	//.org 0x2000
	 	
	.section .text
_reset :
	l.movhi r0,0x0000	; #r0 = 0
	l.ori r0,r0,0x0000	;
	
        /* set up super mode */
        l.addi  r3,r0,SPR_SR_SM
        l.mtspr r0,r3,SPR_SR

	/* Clear BSS */
	LOAD_SYMBOL_2_GPR(r28,___bss_start)
	LOAD_SYMBOL_2_GPR(r30,__end)
1:
	l.sw	(0)(r28), r0
	l.sfltu    r28, r30
	l.bf	1b
	l.addi	  r28, r28, 4
	      
        // clear all registers
        CLEAR_GPR(r1)
        CLEAR_GPR(r2)
        CLEAR_GPR(r3)
        CLEAR_GPR(r4)
        CLEAR_GPR(r5)
        CLEAR_GPR(r6)
        CLEAR_GPR(r7)
        CLEAR_GPR(r8)
        CLEAR_GPR(r9)
        CLEAR_GPR(r10)
        CLEAR_GPR(r11)
        CLEAR_GPR(r12)
        CLEAR_GPR(r13)
        CLEAR_GPR(r14)
        CLEAR_GPR(r15)
        CLEAR_GPR(r16)
        CLEAR_GPR(r17)
        CLEAR_GPR(r18)
        CLEAR_GPR(r19)
        CLEAR_GPR(r20)
        CLEAR_GPR(r21)
        CLEAR_GPR(r22)
        CLEAR_GPR(r23)
        CLEAR_GPR(r24)
        CLEAR_GPR(r25)
        CLEAR_GPR(r26)
        CLEAR_GPR(r27)
        CLEAR_GPR(r28)
        CLEAR_GPR(r29)
        CLEAR_GPR(r30)
        CLEAR_GPR(r31)

    	/* Clear timer  */
	l.mtspr r0, r0, SPR_TTMR  	
	
        /* Early Stack initilization */
        LOAD_SYMBOL_2_GPR(r1 ,_stack)
	
	/* Jump to start of program */
	l.jal main

_align:
	l.ori	r3,r0,0x600
	l.nop	NOP_REPORT
	l.mfspr r3,r0,SPR_EPCR_BASE
	l.nop	NOP_REPORT
	l.lwz	r3,0(r3)
	l.nop	NOP_REPORT		
	l.mfspr r3,r0,SPR_EEAR_BASE
	l.nop	NOP_REPORT
	/* Loop in place, cause simulator to exit */
	l.ori	r3,r0,1
	l.j 	0
	l.nop	NOP_EXIT

		
#define LOAD_SYMBOL_2_GPR(gpr,symbol)		\
	.global symbol;			\
        l.movhi gpr,hi(symbol);	\
        l.ori   gpr,gpr,lo(symbol)


.global     start_schedule
start_schedule:
    LOAD_SYMBOL_2_GPR(r3, new_task)
    l.lwz   r3,0(r3)                /* r3 = &new_task->stack_ptr */
    l.lwz   r1,0(r3)               /* sp = *(&new->stack_ptr) */

    l.lwz   r2,PT_PC(r1)                /* load context for task to be resumed */
    l.mtspr r0,r2,SPR_EPCR_BASE
    l.lwz   r2,PT_SR(r1)
    l.mtspr r0,r2,SPR_ESR_BASE

    RESTORE_CONTEXT

    l.addi  r1,r1, STK_FRAME_SIZE
    l.rfe
    l.nop

/*schedule:*/

/* task switch function */
task_switch:
        /* l.sys exception for now so we are in supervisor mode */
        /* exception  - recover pc from epcr */       
		
        l.mfspr r3,r0,SPR_EPCR_BASE     /* save program counter that was put in exception register */
        l.sw    PT_PC(r1),r3
        l.mfspr r3,r0,SPR_ESR_BASE      /* save status register that was put in exception register */
        l.sw    PT_SR(r1),r3
	
        STORE_CONTEXT
	
					/* Store current stack pointer   */
        LOAD_SYMBOL_2_GPR(r3, old_task)            /* r3= &old_task */
        l.lwz   r3,0(r3)                /* r3 = &old_task->stack_ptr */
        l.sw    0(r3),r1                /* old_task->stack_ptr = SP */

/* task switch in interrupt routine */
interrupt_switch:
	
	/*  */
        LOAD_SYMBOL_2_GPR(r2, new_task)        /* r2= &new_task */
        l.lwz   r2,0(r2)                                                /* r2 = &new_task->stack_ptr  */
        l.lwz   r1,0(r2)               /* sp = *(&new_task->task_stack) */
        
        l.lwz   r2,PT_PC(r1)                /* load context for task to be resumed */
        l.mtspr r0,r2,SPR_EPCR_BASE
        
        l.lwz   r2,PT_SR(r1)
        l.mtspr r0,r2,SPR_ESR_BASE

        RESTORE_CONTEXT

        l.addi  r1,r1,STK_FRAME_SIZE	
        l.rfe
        l.nop

TimerISR:
	l.mfspr r3,r0,SPR_EPCR_BASE     /* save program counter that was put in exception register */
	l.sw    PT_PC(r1),r3
	l.mfspr r3,r0,SPR_ESR_BASE      /* save status register that was put in exception register */
	l.sw    PT_SR(r1),r3		
	
	STORE_CONTEXT

	/* Timer interrupt handler */
	l.jal hardware_timer
	l.nop       
#if 1
	l.lwz   r2,PT_PC(r1)		/* load context for task to be resumed */
	l.mtspr r0,r2,SPR_EPCR_BASE
	l.lwz   r2,PT_SR(r1)
	l.mtspr r0,r2,SPR_ESR_BASE
#endif

	RESTORE_CONTEXT
	
	l.addi	r1,r1,STK_FRAME_SIZE
	l.rfe
	l.nop	

UsrISR:
	l.mfspr r3,r0,SPR_EPCR_BASE     /* save program counter that was put in exception register */
	l.sw    PT_PC(r1),r3
	l.mfspr r3,r0,SPR_ESR_BASE      /* save status register that was put in exception register */
	l.sw    PT_SR(r1),r3		
	
	STORE_CONTEXT

	/* Timer interrupt handler */
	l.jal usr_handle
	l.nop       
#if 1
	l.lwz   r2,PT_PC(r1)		/* load context for task to be resumed */
	l.mtspr r0,r2,SPR_EPCR_BASE
	l.lwz   r2,PT_SR(r1)
	l.mtspr r0,r2,SPR_ESR_BASE
#endif

	RESTORE_CONTEXT
	
	l.addi	r1,r1,STK_FRAME_SIZE
	l.rfe
	l.nop	


