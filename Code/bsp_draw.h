#ifndef __BSP_DRAW_H
#define __BSP_DRAW_H

#include "headfile.h"
#include <stdint.h>
#include <assert.h>

void IPS_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r, uint16_t color);
void IPS_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void IPS_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void IPS_Drawcross(uint16_t x, uint16_t y, uint16_t color);

#endif
