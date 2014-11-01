/*
 * File      : uart.c
 * This file is part of FOS RTOS
 * COPYRIGHT (C) 2013, Shanjin Yang
 *
 */

#include <hw_include.h>

/* both THR and TSR empty */
#define BOTH_EMPTY (UART_LSR_TEMT | UART_LSR_THRE)

/* jump out when write data to fifo (both THR and TSR empty) */
#define WAIT_FOR_XMITR \
    do { \
            lsr = REG8(UART_BASE + UART_LSR); \
    } while ((lsr & BOTH_EMPTY) != BOTH_EMPTY)

/* jump out when write data to fifo (THR empty) */
#define WAIT_FOR_THRE \
    do { \
            lsr = REG8(UART_BASE + UART_LSR); \
    } while ((lsr & UART_LSR_THRE) != UART_LSR_THRE)

/* decide if any data in receive fifo */
#define CHECK_FOR_CHAR (REG8(UART_BASE + UART_LSR) & UART_LSR_DR)

/* jump out when fifo receives data */
#define WAIT_FOR_CHAR \
    do { \
           lsr = REG8(UART_BASE + UART_LSR); \
    } while ((lsr & UART_LSR_DR) != UART_LSR_DR)

#define UART_TX_BUFF_LEN 32
#define UART_TX_BUFF_MASK (UART_TX_BUFF_LEN -1)

char tx_buff[UART_TX_BUFF_LEN];
volatile int tx_level, rx_level;

void uart_isr(unsigned char *buffer)
{
    /*clear pending interrupt flag*/
    mtspr(SPR_PICSR, mfspr(SPR_PICSR) & (~(1<<2)) );

    unsigned int interrupt_id;
    interrupt_id = REG8(UART_BASE + UART_IIR);

    /* Receive Line Status */
    if ( interrupt_id & UART_IIR_RDI )
    {
        /* If no error on RLS, normal ready, save into the data buffer. */
        /* Note: read RBR will clear the interrupt */
        char buf = REG8(UART_BASE + UART_RX);
        uart_putc(buf);
    }
    uart_putc('a');
}

void uart_init(void)
{
    int divisor;
    float float_divisor;

    /* Reset receiver and transmiter */
    //start up FIFO ,clear RCVR FIFO and XMIT FIFO ,and set FIFO size for 4 bytes
    REG8(UART_BASE + UART_FCR) = UART_FCR_ENABLE_FIFO | UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT | UART_FCR_TRIGGER_4;
    
    /* Disable all interrupts */
    REG8(UART_BASE + UART_IER) = 0x0;

    /* Enable RX and TX interrupt */
    //REG8(UART_BASE + UART_IER) = UART_IER_RDI | UART_IER_THRI;

    /* Set 8 bit char, 1 stop bit, no parity */
    REG8(UART_BASE + UART_LCR) = UART_LCR_WLEN8 & ~(UART_LCR_STOP | UART_LCR_PARITY);

    /* Set baud rate */
    float_divisor = (float)SYS_CLK/(16 * UART_BAUD_RATE);
    float_divisor += 0.50f; // Ensure round up
    divisor = (int)float_divisor;

    /* set corresponding register value */
    REG8(UART_BASE + UART_LCR) |= UART_LCR_DLAB;
    REG8(UART_BASE + UART_DLM) = (divisor >> 8) & 0x000000ff;
    REG8(UART_BASE + UART_DLL) = divisor & 0x000000ff;
    REG8(UART_BASE + UART_LCR) &= ~(UART_LCR_DLAB);

    //hw_interrupt_install(2, uart_isr, NULL);
    //REG8(UART_BASE + UART_IER) = UART_IER_RDI;
}

/* print a character via uart */
void uart_putc(char c)
{
    unsigned char lsr;
    WAIT_FOR_THRE;
    REG8(UART_BASE + UART_TX) = c;
   
    if(c == '\n') {
        WAIT_FOR_THRE;
        REG8(UART_BASE + UART_TX) = '\r';
    }
    
    WAIT_FOR_XMITR;
}

/* Only used when we know THRE is empty, typically in interrupt */
void uart_putc_noblock(char c)
{
    REG8(UART_BASE + UART_TX) = c;
}

/* get a character via uart */
char uart_getc(void)
{
    unsigned char lsr;
    char c;

    WAIT_FOR_CHAR;
    c = REG8(UART_BASE + UART_RX);
    return c;
}

/* print a string via uart */
void uart_print_str(char *p)
{
    while(*p != 0) {
        uart_putc(*p);
        p++;
    }
}

char uart_check_for_char()
{
    return CHECK_FOR_CHAR;
}

/* Enable receiver data interrupt */
void uart_rxint_enable()
{
    REG8(UART_BASE + UART_IER) |= UART_IER_RDI;
}

/* Disable receiver data interrupt */
void uart_rxint_disable()
{
    REG8(UART_BASE + UART_IER) &= ~(UART_IER_RDI);
}

/* Enable Transmitter holding register int. */
void uart_txint_enable()
{
    REG8(UART_BASE + UART_IER) |= UART_IER_THRI;
}

/* Disable Transmitter holding register int. */
void uart_txint_disable()
{
    REG8(UART_BASE + UART_IER) &= ~(UART_IER_THRI);
}

/* get status from IIR */
char uart_get_iir()
{
    return REG8(UART_BASE + UART_IIR);
}

/* get status from LSR */
char uart_get_lsr()
{
    return REG8(UART_BASE + UART_LSR);
}

/* print 0x... */
void uart_print_long(unsigned long ul)
{
    int i;
    char c;

    uart_print_str("0x");

    for(i=0; i<8; i++) {

        c = (char) (ul>>((7-i)*4)) & 0xf;
        if(c >= 0x0 && c<=0x9)
            c += '0';
        else
            c += 'a' - 10;

        uart_putc(c);
    }
}

/* print 0x0.. */
void uart_print_short(unsigned long ul)
{
    int i;
    char c;
    char flag = 0;

    uart_print_str("0x");

    for(i=0; i<8; i++) {

        c = (char) (ul>>((7-i)*4)) & 0xf;
        if(c >= 0x0 && c<=0x9)
            c += '0';
        else
            c += 'a' - 10;

        if ((c != '0') || (i==7))
            flag=1;

        if(flag)
            uart_putc(c);
    }
}
