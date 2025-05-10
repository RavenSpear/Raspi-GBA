#ifndef __FB_H
#define __FB_H

#include <stdint.h>

void fb_init(void);
void drawPixel(int x, int y, uint8_t attr);
void drawChar(uint8_t ch, int x, int y, uint8_t attr);
void drawString(int x, int y, char *s, uint8_t attr);
void drawRect(int x1, int y1, int x2, int y2, uint8_t attr, int fill);
void drawCircle(int x0, int y0, int radius, uint8_t attr, int fill);
void drawLine(int x1, int y1, int x2, int y2, uint8_t attr);

#endif
