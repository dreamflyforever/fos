
/*
 * Copyright (c) 2020, Shanjin Yang.<sjyangv0@qq.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Shanjin Yang.
 * 4. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the FOS.
 *
 * The latest version of FOS download by <https://github.com/dreamflyforever/fos>
 *
 */

.equ    MSTATUS_MPP,        0x00001800
.equ __reg_mepc_OFFSET       , 0x00
.equ __reg_mstatus_OFFSET    , 0x04
.equ __reg_x1_OFFSET         , 0x08
.equ __reg_x3_OFFSET         , 0x0C
.equ __reg_x4_OFFSET         , 0x10
.equ __reg_x5_OFFSET         , 0x14
.equ __reg_x6_OFFSET         , 0x18
.equ __reg_x7_OFFSET         , 0x1C
.equ __reg_x8_OFFSET         , 0x20
.equ __reg_x9_OFFSET         , 0x24
.equ __reg_x10_OFFSET        , 0x28
.equ __reg_x11_OFFSET        , 0x2C
.equ __reg_x12_OFFSET        , 0x30
.equ __reg_x13_OFFSET        , 0x34
.equ __reg_x14_OFFSET        , 0x38
.equ __reg_x15_OFFSET        , 0x3C
.equ __reg_x16_OFFSET        , 0x40
.equ __reg_x17_OFFSET        , 0x44
.equ __reg_x18_OFFSET        , 0x48
.equ __reg_x19_OFFSET        , 0x4C
.equ __reg_x20_OFFSET        , 0x50
.equ __reg_x21_OFFSET        , 0x54
.equ __reg_x22_OFFSET        , 0x58
.equ __reg_x23_OFFSET        , 0x5C
.equ __reg_x24_OFFSET        , 0x60
.equ __reg_x25_OFFSET        , 0x64
.equ __reg_x26_OFFSET        , 0x68
.equ __reg_x27_OFFSET        , 0x6C
.equ __reg_x28_OFFSET        , 0x70
.equ __reg_x29_OFFSET        , 0x74
.equ __reg_x30_OFFSET        , 0x78
.equ __reg_x31_OFFSET        , 0x7C

.equ __reg_mepc__OFFSET      , __reg_mepc_OFFSET
.equ __reg_mstatus__OFFSET   , __reg_mstatus_OFFSET
.equ __reg_ra__OFFSET        , __reg_x1_OFFSET
.equ __reg_gp__OFFSET        , __reg_x3_OFFSET
.equ __reg_tp__OFFSET        , __reg_x4_OFFSET
.equ __reg_t0__OFFSET        , __reg_x5_OFFSET
.equ __reg_t1__OFFSET        , __reg_x6_OFFSET
.equ __reg_t2__OFFSET        , __reg_x7_OFFSET
.equ __reg_s0__OFFSET        , __reg_x8_OFFSET
.equ __reg_fp__OFFSET        , __reg_x8_OFFSET
.equ __reg_s1__OFFSET        , __reg_x9_OFFSET
.equ __reg_a0__OFFSET        , __reg_x10_OFFSET
.equ __reg_a1__OFFSET        , __reg_x11_OFFSET
.equ __reg_a2__OFFSET        , __reg_x12_OFFSET
.equ __reg_a3__OFFSET        , __reg_x13_OFFSET
.equ __reg_a4__OFFSET        , __reg_x14_OFFSET
.equ __reg_a5__OFFSET        , __reg_x15_OFFSET
.equ __reg_a6__OFFSET        , __reg_x16_OFFSET
.equ __reg_a7__OFFSET        , __reg_x17_OFFSET
.equ __reg_s2__OFFSET        , __reg_x18_OFFSET
.equ __reg_s3__OFFSET        , __reg_x19_OFFSET
.equ __reg_s4__OFFSET        , __reg_x20_OFFSET
.equ __reg_s5__OFFSET        , __reg_x21_OFFSET
.equ __reg_s6__OFFSET        , __reg_x22_OFFSET
.equ __reg_s7__OFFSET        , __reg_x23_OFFSET
.equ __reg_s8__OFFSET        , __reg_x24_OFFSET
.equ __reg_s9__OFFSET        , __reg_x25_OFFSET
.equ __reg_s10__OFFSET       , __reg_x26_OFFSET
.equ __reg_s11__OFFSET       , __reg_x27_OFFSET
.equ __reg_t3__OFFSET        , __reg_x28_OFFSET
.equ __reg_t4__OFFSET        , __reg_x29_OFFSET
.equ __reg_t5__OFFSET        , __reg_x30_OFFSET
.equ __reg_t6__OFFSET        , __reg_x31_OFFSET

.global start_schedule
.type start_schedule, %function
start_schedule:
    j       __first_task_switch

.type __task_switch, %function
__task_switch:
    addi    sp, sp, -60

    sw      x1, 0(sp)
    sw      x3, 4(sp)
    sw      x4, 8(sp)
    sw      x5, 12(sp)
    sw      x6, 16(sp)
    sw      x7, 20(sp)
    sw      x8, 24(sp)
    sw      x9, 28(sp)
    sw      x10, 32(sp)
    sw      x11, 36(sp)
    sw      x12, 40(sp)
    sw      x13, 44(sp)
    sw      x14, 48(sp)
    sw      x15, 52(sp)

    sw      ra, 56(sp)

    la      a1, new_task
    lw      a1, (a1)
    sw      sp, (a1)

__first_task_switch:
    lw      t0, new_task
    /*sp = new_task->sp*/
    lw      sp, (t0)

    lw      t0,   __reg_mepc_OFFSET(sp)
    csrw    mepc, t0

    lw      t0,   __reg_mstatus_OFFSET(sp)
    csrw    mstatus, t0

    lw x1, __reg_x1_OFFSET(sp)
    lw x3, __reg_x3_OFFSET(sp)
    lw x4, __reg_x4_OFFSET(sp)
    lw x5, __reg_x5_OFFSET(sp)
    lw x6, __reg_x6_OFFSET(sp)
    lw x7, __reg_x7_OFFSET(sp)
    lw x8, __reg_x8_OFFSET(sp)
    lw x9, __reg_x9_OFFSET(sp)
    lw x10, __reg_x10_OFFSET(sp)
    lw x11, __reg_x11_OFFSET(sp)
    lw x12, __reg_x12_OFFSET(sp)
    lw x13, __reg_x13_OFFSET(sp)
    lw x14, __reg_x14_OFFSET(sp)
    lw x15, __reg_x15_OFFSET(sp)
    lw x16, __reg_x16_OFFSET(sp)
    lw x17, __reg_x17_OFFSET(sp)
    lw x18, __reg_x18_OFFSET(sp)
    lw x19, __reg_x19_OFFSET(sp)
    lw x20, __reg_x20_OFFSET(sp)
    lw x21, __reg_x21_OFFSET(sp)
    lw x22, __reg_x22_OFFSET(sp)
    lw x23, __reg_x23_OFFSET(sp)
    lw x24, __reg_x24_OFFSET(sp)
    lw x25, __reg_x25_OFFSET(sp)
    lw x26, __reg_x26_OFFSET(sp)
    lw x27, __reg_x27_OFFSET(sp)
    lw x28, __reg_x28_OFFSET(sp)
    lw x29, __reg_x29_OFFSET(sp)
    lw x30, __reg_x30_OFFSET(sp)
    lw x31, __reg_x31_OFFSET(sp)
    addi    sp, sp, 128

    mret

.global port_schedule
.align 2
.type port_schedule, %function
port_schedule:
    addi   sp, sp, -128
    sw x1, __reg_x1_OFFSET(sp)
    sw x3, __reg_x3_OFFSET(sp)
    sw x4, __reg_x4_OFFSET(sp)
    sw x5, __reg_x5_OFFSET(sp)
    sw x6, __reg_x6_OFFSET(sp)
    sw x7, __reg_x7_OFFSET(sp)
    sw x8, __reg_x8_OFFSET(sp)
    sw x9, __reg_x9_OFFSET(sp)
    sw x10, __reg_x10_OFFSET(sp)
    sw x11, __reg_x11_OFFSET(sp)
    sw x12, __reg_x12_OFFSET(sp)
    sw x13, __reg_x13_OFFSET(sp)
    sw x14, __reg_x14_OFFSET(sp)
    sw x15, __reg_x15_OFFSET(sp)
    sw x16, __reg_x16_OFFSET(sp)
    sw x17, __reg_x17_OFFSET(sp)
    sw x18, __reg_x18_OFFSET(sp)
    sw x19, __reg_x19_OFFSET(sp)
    sw x20, __reg_x20_OFFSET(sp)
    sw x21, __reg_x21_OFFSET(sp)
    sw x22, __reg_x22_OFFSET(sp)
    sw x23, __reg_x23_OFFSET(sp)
    sw x24, __reg_x24_OFFSET(sp)
    sw x25, __reg_x25_OFFSET(sp)
    sw x26, __reg_x26_OFFSET(sp)
    sw x27, __reg_x27_OFFSET(sp)
    sw x28, __reg_x28_OFFSET(sp)
    sw x29, __reg_x29_OFFSET(sp)
    sw x30, __reg_x30_OFFSET(sp)
    sw x31, __reg_x31_OFFSET(sp)

    sw     ra,  __reg_mepc_OFFSET(sp)

    csrr   t0,  mstatus
    li     t1,  MSTATUS_MPP
    or     t0,  t0, t1
    sw     t0,  __reg_mstatus_OFFSET(sp)


switch_task:
    la      t0, old_task        
    la      t1, new_task       
    
    lw      t2, (t0)
    sw      sp, (t2)

    lw      t1, (t1)           
    sw      t1, (t0)

    lw      sp, (t1)

restore_context:
    /* restore context*/
    lw      t0,   __reg_mepc_OFFSET(sp)
    csrw    mepc, t0

    lw      t0,   __reg_mstatus_OFFSET(sp)
    csrw    mstatus, t0

    lw x1, __reg_x1_OFFSET(sp)
    lw x3, __reg_x3_OFFSET(sp)
    lw x4, __reg_x4_OFFSET(sp)
    lw x5, __reg_x5_OFFSET(sp)
    lw x6, __reg_x6_OFFSET(sp)
    lw x7, __reg_x7_OFFSET(sp)
    lw x8, __reg_x8_OFFSET(sp)
    lw x9, __reg_x9_OFFSET(sp)
    lw x10, __reg_x10_OFFSET(sp)
    lw x11, __reg_x11_OFFSET(sp)
    lw x12, __reg_x12_OFFSET(sp)
    lw x13, __reg_x13_OFFSET(sp)
    lw x14, __reg_x14_OFFSET(sp)
    lw x15, __reg_x15_OFFSET(sp)
    lw x16, __reg_x16_OFFSET(sp)
    lw x17, __reg_x17_OFFSET(sp)
    lw x18, __reg_x18_OFFSET(sp)
    lw x19, __reg_x19_OFFSET(sp)
    lw x20, __reg_x20_OFFSET(sp)
    lw x21, __reg_x21_OFFSET(sp)
    lw x22, __reg_x22_OFFSET(sp)
    lw x23, __reg_x23_OFFSET(sp)
    lw x24, __reg_x24_OFFSET(sp)
    lw x25, __reg_x25_OFFSET(sp)
    lw x26, __reg_x26_OFFSET(sp)
    lw x27, __reg_x27_OFFSET(sp)
    lw x28, __reg_x28_OFFSET(sp)
    lw x29, __reg_x29_OFFSET(sp)
    lw x30, __reg_x30_OFFSET(sp)
    lw x31, __reg_x31_OFFSET(sp)
    addi    sp, sp, 128
    mret
