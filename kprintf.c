#include <stdint.h>
#include "pl011.h"
#include "kprintf.h"

void (*putc_f)(char);

static void uint_2_ascii(uint32_t num, uint8_t base, char *bf)
{
    uint8_t n = 0x0U;
    uint32_t d = 0x1U;
    while (num / d >= base)
        d *= base;
    while (d != 0x0U) {
        uint8_t dgt = num / d;
        num %= d;
        d /= base;
        if (n || dgt > 0x0U || d == 0x0U) {
            *bf++ = dgt + (dgt < 10U ? '0' : ('a' - 10));
            ++n;
        }
    }
    *bf = 0;
}

static void int_2_ascii(int num, char *bf)
{
    if (num < 0) {
        num = -num;
        *bf++ = '-';
    }
    uint_2_ascii(num, 10U, bf);
}

static void putstr(char *bf)
{
    char c;
    while ((c = *bf++))
        putc_f(c);
}

void p_f(const char *fmt, va_list va)
{
    char bf[12] = {0};
    char c;

    while ((c = *fmt++)) {
        if (c != '%')
            putc_f(c);
        else {
            c = *fmt++;
            if (c == '0') {
                c = *fmt++;
            }
            switch (c) {
                case 0:
                    goto exit;
                case 'd':
                    int_2_ascii(va_arg(va, int), bf);
                    putstr(bf);
                    break;
                case 'x':
                case 'X':
                    uint_2_ascii(va_arg(va, uint32_t), 16U, bf);
                    putstr(bf);
                    break;
                case 'u':
                    uint_2_ascii(va_arg(va, uint32_t), 10U, bf);
                    putstr(bf);
                    break;
                case 'c':
                    putc_f((char)(va_arg(va, int)));
                    break;
                case 's':
                    putstr(va_arg(va, char *));
                    break;
                case '%':
                    putc_f(c);
                default:
                    break;
            }
        }
    }
exit:
    return;
}

void kprintf(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    putc_f = pl011_uart_putc;
    p_f(fmt, va);
    va_end(va);
}
