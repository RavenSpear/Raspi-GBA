#include <stddef.h>
#include "pl011.h"
#include "mmio.h"

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

static int strlen(const char* ptr)
{
    int i = 0;
    while(*ptr != 0)
    {
        i++;
        ptr += 1;
    }

    return i;
}

static void config_gpio_uart0(void)
{
    uint32_t reg_read = mmio_read(GPFSEL1);
    reg_read &= ~(7<<12);                   /* clean gpio14 */
    reg_read |= 4<<12;                      /* set alt0 for gpio14 */
    reg_read &= ~(7<<15);                   /* clean gpio15 */
    reg_read |= 4<<15;                      /* set alt0 for gpio 15 */
    mmio_write(GPFSEL1, reg_read);

    reg_read = mmio_read(GPPUPPDN0);     
    mmio_write(GPPUPPDN0, reg_read & ~((3<<28)|(3<<30))); /* set 00 */
}

static void is_tx_complete(void)
{
    while (((mmio_read(UART0_FR) >> 3U) & 1U) != 0) {}
}

void uart_init()
{
    uint32_t reg_read;
    config_gpio_uart0();

    // Disable UART before anything else
    reg_read = mmio_read(UART0_CR);
    mmio_write(UART0_CR, reg_read & ~(1<<0));

    // Wait for any ongoing transmissions to complete
    is_tx_complete();

    // Flush FIFOs
    reg_read = mmio_read(UART0_LCR);
    mmio_write(UART0_LCR, reg_read & ~(1<<4));

    // Set frequency divisors (UARTIBRD and UARTFBRD) to configure the speed;
    mmio_write(UART0_IBRD, 0x1AU);  /* 48MHz /(16 * 115200 baud) */
    mmio_write(UART0_FBRD, 0x3U); /* fraction = 3/64 */

    // Configure data frame format according to the parameters (UARTLCR_H). ((0x3U << 5) | (0x0U << 4))
    reg_read = mmio_read(UART0_LCR);
    mmio_write(UART0_LCR, (reg_read & ~((0x3U << 5) | ( 0x1U << 4))) | ((0x3U << 5) | (0x0U << 4)));

    // Mask all interrupts by setting corresponding bits to 1
    mmio_write(UART0_IMSC, 0x7ff);

    // Disable DMA by setting all bits to 0
    mmio_write(UART0_DMACR, 0x0U);

    // Finally enable UART    
    mmio_write(UART0_CR, 0x301U);
}

void pl011_uart_puts(const char *data)
{
    size_t len = strlen(data);

    is_tx_complete();

    for (size_t i = 0; i < len; ++i) {
        if (data[i] == '\n') {
            mmio_write(UART0_DR, '\r');
            is_tx_complete();
        }
        mmio_write(UART0_DR, data[i]);
        is_tx_complete();
    }
}
