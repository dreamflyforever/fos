#ifndef __BOARD_H__
#define __BOARD_H__

/* SYSTEM CLOCK */
#define SYS_CLK	40000000 // 40MHz
#define TICKS_PER_SECOND 100

/* UART */
#define UART_BAUD_RATE	38400
#define UART_BASE			0x90000000
#define UART_IRQ			2

/* SimpleGPIO */
#define GPIO_BASE	0x93000000

/* SimpleSpi */
#define SPI_BASE	0x94000000
#define SPI_IRQ	5
#define SPI_NUM_CORES	1
#define SPI_BASE_ADDRESSES_CSV	SPI_BASE

/* I2cMaster */
#define I2C_BASE    0x91000000
#define I2C_IRQ	3
#define I2C_RATE    100000	/* 100K */
#define I2C_MASTER_SLAVE_NUM_CORES 	1
#define I2C_MASTER_SLAVE_BASE_ADDRESSES_CSV	I2C_BASE

/* Ethernet */
#define ETHERNET_BASE		0x92000000
#define ETHERNET_IRQ		4

#endif /* _BOARD_H_ */
