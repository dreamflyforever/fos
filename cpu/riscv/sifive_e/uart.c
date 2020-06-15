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

enum {
	HART0_CLOCK_FREQ           = 0x00010000,
	UART0_CLOCK_FREQ           = 0x00011000,
	UART0_BAUD_RATE            = 0x00011100,
	NS16550A_UART0_CTRL_ADDR   = 0x00030000,
	RISCV_HTIF_BASE_ADDR       = 0x00050000,
	SIFIVE_CLINT_CTRL_ADDR     = 0x55550000,
	SIFIVE_CLIC_CRTL_ADDR      = 0x55550001,
	SIFIVE_TEST_CTRL_ADDR      = 0x55550002,
	SIFIVE_UART0_CTRL_ADDR     = 0x55550010,
	SIFIVE_GPIO0_CTRL_ADDR     = 0x55550020,
	SIFIVE_SPI0_CTRL_ADDR      = 0x55550030,
};

enum {
	/* UART Registers */
	UART_REG_TXFIFO = 0,
	UART_REG_RXFIFO = 1,
	UART_REG_TXCTRL = 2,
	UART_REG_RXCTRL = 3,
	UART_REG_IE     = 4,
	UART_REG_IP     = 5,
	UART_REG_DIV    = 6,

	/* TXCTRL register */
	UART_TXEN       = 1,
	UART_TXSTOP     = 2,

	/* RXCTRL register */
	UART_RXEN       = 1,

	/* IP register */
	UART_IP_TXWM    = 1,
	UART_IP_RXWM    = 2
};

typedef struct auxval {
	unsigned long key;
	unsigned long val;
} auxval_t;

static volatile int *uart;

auxval_t __auxv[] = {
	{ UART0_CLOCK_FREQ,         32000000   },
	{ UART0_BAUD_RATE,          115200     },
	{ SIFIVE_UART0_CTRL_ADDR,   0x10013000 },
	{ SIFIVE_TEST_CTRL_ADDR,    0x100000   },
	{ 0, 0 }
};


enum {
	SIFIVE_TEST_FAIL = 0x3333,
	SIFIVE_TEST_PASS = 0x5555,
};

unsigned long getauxval(unsigned long key)
{
	auxval_t *auxv = __auxv;
	while(auxv->key) {
		if (auxv->key == key) {
			return auxv->val;
		}
		auxv++;
	}
	return 0;
}

static void sifive_uart_poweroff()
{
	*uart = SIFIVE_TEST_PASS;
	while (1) {
		asm volatile("");
	}
}


static void sifive_uart_init()
{
	uart = (int *)(void *)getauxval(SIFIVE_UART0_CTRL_ADDR);
	uint32_t uart_freq = getauxval(UART0_CLOCK_FREQ);
	uint32_t baud_rate = getauxval(UART0_BAUD_RATE);
	uint32_t divisor = uart_freq / baud_rate - 1;
	uart[UART_REG_DIV] = divisor;
	uart[UART_REG_TXCTRL] = UART_TXEN;
	uart[UART_REG_RXCTRL] = UART_RXEN;
	uart[UART_REG_IE] = 0;
}

static int sifive_uart_putchar(int ch)
{
	while (uart[UART_REG_TXFIFO] < 0);
	return uart[UART_REG_TXFIFO] = ch & 0xff;
}

static int sifive_uart_getchar()
{
    int ch = uart[UART_REG_RXFIFO];
    if (ch < 0) return -1;
    return ch;
}
