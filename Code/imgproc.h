#ifndef __IMGPROC_H
#define __IMGPROC_H


#include "headfile.h"
#include <stdint.h>
#include <math.h>
#include <assert.h>


#define AT_IMAGE(img, x, y)          ((img)->data[(y)*(img)->step+(x)])  //��ȡͼ�����ص�����  stepΪһ���ж�������
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

// ����Ѳ��ͬʱ����Ӧ��ֵ������(x,y)��ʼ�����ذױ�����
void findline_lefthand_adaptive(image_t *img, uint8 block_size, uint8 clip_value, uint16 x, uint16 y, uint16 pts[][2], uint16 *num);

// ����Ѳ��ͬʱ����Ӧ��ֵ������(x,y)��ʼ�����ذױ�����
void findline_righthand_adaptive(image_t *img, uint8 block_size, uint8 clip_value, uint16 x, uint16 y, uint16 pts[][2], uint16 *num);

// ��򷨼����ֵ����ֵ
uint16 getOSTUThreshold(image_t *img, uint8 MinThreshold, uint8 MaxThreshold);

// �㼯�����˲�
void blur_points(uint16_t pts_in[][2], int num, float pts_out[][2], int kernel);

// �㼯�ֲ��Ƕȱ仯��
void local_angle_points(float pts_in[][2], int num, float angle_out[], int dist);

// �㼯�Ⱦ����  ʹ�߹��Ĳ���ǰ���߶εľ���Ϊ`dist`
void resample_points(float pts_in[][2], int num1, float pts_out[][2], int *num2, float dist);

// �Ƕȱ仯�ʷǼ�������
void nms_angle(float angle_in[], int num, float angle_out[], int kernel);

//��򷨶�ֵ��
uint8 OTSU(uint8 *IMG, uint16 col, uint16 row);

//��Χ��򷨶�ֵ��
uint8 OTSUrange(uint8 *IMG, uint16 x0, uint16 y0, uint16 x1, uint16 y1);

//����Ӧ��ֵ��
void adaptiveThreshold(uint8* img_data, uint8* output_data, uint16 width, uint16 height, uint16 block, uint8 clip_value);


#endif
