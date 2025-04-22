#include "pl011.h"

void main()
{
    uart_init();
    pl011_uart_puts("Helloworld!\n");
    while (1);
}
