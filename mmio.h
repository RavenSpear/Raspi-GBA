#ifndef __MMIO_H
#define __MMIO_H

#include <stdint.h>

inline volatile uint32_t mmio_read(long reg)
{
    return *(volatile uint32_t *)(reg);
}

inline void mmio_write(const long reg, uint32_t val)
{
    *(volatile uint32_t *)(reg) = val;
}

#endif
