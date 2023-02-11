#include "bsp_draw.h"
#include "zf_assert.h"
#include "SEEKFREE_18TFT.h"


/**
 * @brief IPS_Draw_Circle 屏幕绘制圆
 *
 * @param x0
 * @param y0
 * @param r
 * @param color
 *
 */
void IPS_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r, uint16_t color)
{
//    assert(x0 <= TFT_X_MAX);
//    assert(y0 <= TFT_Y_MAX);
//    assert(y0+r <= TFT_Y_MAX);
//    assert(x0+r <= TFT_X_MAX);
//    assert(y0-r >= 0);
//    assert(x0-r >= 0);
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r<<1);             //判断下个点位置的标志
    while(a <= b) {
        lcd_drawpoint(x0+a,y0-b, color);             //5
        lcd_drawpoint(x0+b,y0-a, color);             //0
        lcd_drawpoint(x0+b,y0+a, color);             //4
        lcd_drawpoint(x0+a,y0+b, color);             //6
        lcd_drawpoint(x0-a,y0+b, color);             //1
        lcd_drawpoint(x0-b,y0+a, color);
        lcd_drawpoint(x0-a,y0-b, color);             //2
        lcd_drawpoint(x0-b,y0-a, color);             //7
        a++;
        // 使用Bresenham算法画圆
        if(di<0)di += 4*a+6;
        else {
            di += 10+4*(a-b);
            b--;
        }
    }
}



/**
 * @brief IPS_DrawLine 屏幕绘制直线
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param color
 *
 */
void IPS_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
//    assert(x1 <= TFT_X_MAX);
//    assert(y1 <= TFT_Y_MAX);
//    assert(x1 <= TFT_X_MAX);
//    assert(y2 <= TFT_Y_MAX);
//    assert(x2 <= TFT_X_MAX);
//    assert(y2 <= TFT_Y_MAX);

    uint16 t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x=x2-x1; //计算坐标增量
    delta_y=y2-y1;
    uRow=x1;
    uCol=y1;
    if(delta_x>0) incx=1; //设置单步方向
    else if(delta_x==0) incx=0;//垂直线
    else {
        incx=-1;
        delta_x=-delta_x;
    }
    if(delta_y>0) incy=1;
    else if(delta_y==0) incy=0;//水平线
    else{
        incy=-1;
        delta_y=-delta_y;
    }
    if(delta_x>delta_y) distance=delta_x; //选取基本增量坐标轴
    else distance = delta_y;
    for(t=0;t<=distance+1;t++) { //画线输出

        lcd_drawpoint(uRow,uCol, color);//画点
        xerr+=delta_x ;
        yerr+=delta_y ;
        if(xerr>distance) {
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance) {
            yerr-=distance;
            uCol+=incy;
        }
    }
}


/**
 * @brief IPS_DrawRectangle 屏幕绘制矩形
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param color
 *
 */
void IPS_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    IPS_DrawLine(x1, y1, x2, y1, color);
    IPS_DrawLine(x1, y1, x1, y2, color);
    IPS_DrawLine(x1, y2, x2, y2, color);
    IPS_DrawLine(x2, y1, x2, y2, color);
}



void IPS_Drawcross(uint16_t x, uint16_t y, uint16_t color)
{
    if(x <= 4) x = 4;
    else if(x >= TFT_Y_MAX-5) x = TFT_X_MAX-5;
    if(y <= 4) y = 4;
    else if(y >= TFT_Y_MAX-5) y = TFT_Y_MAX-5;

//    assert(x-4 >= 0 && x <= TFT_X_MAX-5);
//    assert(y-4 >= 0 && y <= TFT_Y_MAX-5);
    IPS_DrawLine(x-4, y-4, x+4, y+4, color);
    IPS_DrawLine(x-4, y+4, x+4, y-4, color);
}
