#ifndef __PL011_H
#define __PL011_H

#include "reg.h"

void uart_init(void);
void pl011_uart_puts(const char *data);

#endif
