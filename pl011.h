#ifndef PL011_H
#define PL011_H

#define PERIPHERAL_BASE     0xfe000000

#define GPIO_BASE           PERIPHERAL_BASE + 0x200000
#define GPFSEL0             GPIO_BASE + 0x000
#define GPFSEL1             GPIO_BASE + 0x004
#define GPSET0              GPIO_BASE + 0x01c
#define GPCLR0              GPIO_BASE + 0x028
#define GPPUPPDN0           GPIO_BASE + 0x0e4

#define UART0_BASE          PERIPHERAL_BASE + 0x201000
#define UART0_DR            UART0_BASE + 0x000
#define UART0_FR            UART0_BASE + 0x018
#define UART0_IBRD          UART0_BASE + 0x024
#define UART0_FBRD          UART0_BASE + 0x028
#define UART0_LCR           UART0_BASE + 0x02c
#define UART0_CR            UART0_BASE + 0x030
#define UART0_IMSC          UART0_BASE + 0x038
#define UART0_DMACR         UART0_BASE + 0x048

void uart_init(void);
void pl011_uart_puts(const char *data);

#endif
