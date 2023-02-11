#ifndef __IMGPROC_H
#define __IMGPROC_H


#include "headfile.h"
#include <stdint.h>
#include <math.h>
#include <assert.h>


#define AT_IMAGE(img, x, y)          ((img)->data[(y)*(img)->step+(x)])  //读取图像像素点数据  step为一行有多少数据
#define DEF_IMAGE(ptr, w, h)         {.data=ptr, .width=w, .height=h, .step=w}
#define AT         AT_IMAGE



typedef struct image
{
    uint8 *data;
    uint32 width;
    uint32 height;
    uint32 step;
} image_t;



void ImageInit(uint8 *Img);

// 左手巡线同时自适应二值化，从(x,y)开始向上沿白边线走
void findline_lefthand_adaptive(image_t *img, uint8 block_size, uint8 clip_value, uint16 x, uint16 y, uint16 pts[][2], uint16 *num);

// 右手巡线同时自适应二值化，从(x,y)开始向上沿白边线走
void findline_righthand_adaptive(image_t *img, uint8 block_size, uint8 clip_value, uint16 x, uint16 y, uint16 pts[][2], uint16 *num);

// 大津法计算二值化阈值
uint16 getOSTUThreshold(image_t *img, uint8 MinThreshold, uint8 MaxThreshold);

// 点集三角滤波
void blur_points(uint16_t pts_in[][2], int num, float pts_out[][2], int kernel);

// 点集局部角度变化率
void local_angle_points(float pts_in[][2], int num, float angle_out[], int dist);

// 点集等距采样  使走过的采样前折线段的距离为`dist`
void resample_points(float pts_in[][2], int num1, float pts_out[][2], int *num2, float dist);

// 角度变化率非极大抑制
void nms_angle(float angle_in[], int num, float angle_out[], int kernel);

//大津法二值化
uint8 OTSU(uint8 *IMG, uint16 col, uint16 row);

//范围大津法二值化
uint8 OTSUrange(uint8 *IMG, uint16 x0, uint16 y0, uint16 x1, uint16 y1);

//自适应二值化
void adaptiveThreshold(uint8* img_data, uint8* output_data, uint16 width, uint16 height, uint16 block, uint8 clip_value);


#endif
