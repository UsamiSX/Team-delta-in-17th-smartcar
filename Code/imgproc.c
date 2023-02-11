#include "imgproc.h"
#include "processImage.h"


void ImageInit(uint8 *Img)
{
    for(int i = 0; i < MT9V03X_H; i++){
        for(int j = 0; j < MT9V03X_W; j++){
            mt9v03x_image[i][j] = 13*sqrt(Img[i*MT9V03X_W + j]);
        }
    }
}


int clip(int x, int low, int up)
{
    return x > up ? up : x < low ? low : x;
}


/* 前进方向定义：
 *   0
 * 3   1
 *   2
 */
const int dir_front[4][2] = {{0, -1},
                             {1,  0},
                             {0,  1},
                             {-1, 0}};
const int dir_frontleft[4][2] = {{-1,-1},
                                 {1, -1},
                                 {1,  1},
                                 {-1, 1}};
const int dir_frontright[4][2] = {{1, -1},
                                  {1,  1},
                                  {-1, 1},
                                  {-1,-1}};


/**
 * @brief findline_lefthand_adaptive 左手迷宫巡线
 *
 * @param   *img          传入的图像数据
 * @param   block_size    卷积核大小(奇数)
 * @param   clip_value    二值化校准值
 * @param   x             要二值化的位置x坐标
 * @param   y             要二值化的位置y坐标
 * @param   pts           原图左右边线输出
 * @param   *num          图像高度
 *
 * @return  None
 */
void findline_lefthand_adaptive(image_t *img, uint8 block_size, uint8 clip_value, uint16 x, uint16 y, uint16 pts[][2], uint16 *num)
{
//    assert(img && img->data);
//    assert(num && *num >= 0);
//    assert(block_size > 1 && block_size % 2 == 1);
    uint16_t half = block_size / 2;
    uint16_t step = 0, dir = 0, turn = 0;
    while (step < *num && half < x && x < img->width - half - 1 && half < y && y < img->height - half - 1 && turn < 4)
    {
        //二值化开始
        int local_thres = 0;
        for (int dy = -half; dy <= half; dy++)
        {
            for (int dx = -half; dx <= half; dx++)
            {
                local_thres += AT(img, x + dx, y + dy);
            }
        }
        local_thres /= block_size * block_size;
        local_thres -= clip_value;
        //二值化结束

        //开始寻找边线
        //uint16_t current_value = AT(img, x, y);y指的是行，x指的是列
        uint16_t front_value = AT(img, x + dir_front[dir][0], y + dir_front[dir][1]);//要二值化的图像的
        uint16_t frontleft_value = AT(img, x + dir_frontleft[dir][0], y + dir_frontleft[dir][1]);
        if (front_value < local_thres) //说明前面是黑色，则转向，转向的处理为dir+1
        {
            dir = (dir + 1) % 4;//dir只能取0 1 2 3四个数，所以取余4，顺时针换向
            turn++;  //换了四次向前全是黑色，则退出循环，如果按左手摸墙走，不会走进黑色的包围圈中
        }
        else if (frontleft_value < local_thres) //说明前白左前黑，则向前走，处理为pts储存下前一点的坐标
        {
            x += dir_front[dir][0];
            y += dir_front[dir][1];
            pts[step][0] = x;
            pts[step][1] = y;
            step++;//0-图片高度储存数据
            turn = 0;
        }
        else //前白左前白，则pts储存下左前一点的坐标，且换向，
        {
            x += dir_frontleft[dir][0];
            y += dir_frontleft[dir][1];
            dir = (dir + 3) % 4;//逆时针换向
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        }
    }
    *num = step;
}


/**
 * @brief findline_righthand_adaptive 右手迷宫巡线
 *
 * @param   *img          传入的图像数据
 * @param   block_size    卷积核大小(奇数)
 * @param   clip_value    二值化校准值   二值化后的阈值减去此值
 * @param   x             要二值化的位置x坐标
 * @param   y             要二值化的位置x坐标
 * @param   pts           原图左右边线输出
 * @param   *num          图像高度
 *
 * @return  None
 */
void findline_righthand_adaptive(image_t *img, uint8_t block_size, uint8_t clip_value, uint16_t x, uint16_t y, uint16_t pts[][2], uint16_t *num) {
//    assert(img && img->data);
//    assert(num && *num >= 0);
//    assert(block_size > 1 && block_size % 2 == 1);
//    assert(y < img->height);
//    assert(x < img->width);
    uint16_t half = block_size / 2;
    int step = 0, dir = 0, turn = 0;
    while (step < *num && 0 < x && x < img->width - 1 && 0 < y && y < img->height - 1 && turn < 4)
    {
        //二值化开始
        int local_thres = 0;
        for (int dy = -half; dy <= half; dy++) {
            for (int dx = -half; dx <= half; dx++) {
                local_thres += AT(img, x + dx, y + dy);
            }
        }
        local_thres /= block_size * block_size;
        local_thres -= clip_value;
        //二值化结束


        //开始寻找边线
        //uint16_t current_value = AT(img, x, y);
        uint16_t front_value = AT(img, x + dir_front[dir][0], y + dir_front[dir][1]);
        uint16_t frontright_value = AT(img, x + dir_frontright[dir][0], y + dir_frontright[dir][1]);
        if (front_value < local_thres) {
            dir = (dir + 3) % 4;
            turn++;
        }
        else if (frontright_value < local_thres) {
            x += dir_front[dir][0];
            y += dir_front[dir][1];
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        }
        else {
            x += dir_frontright[dir][0];
            y += dir_frontright[dir][1];
            dir = (dir + 1) % 4;
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        }
    }
    *num = step;
}



//void findBase(uint8* img, uint8 x_base, uint8 y_base)
//{
//    for(int a = 93; a >= 0; a--)
//    {
//        if(img[a+5*188] == 0)
//        {
//            x_base = a+1;
//            y_base = 104;
//            return;
//        }
//    }
//}



void blur_points(uint16 pts_in[][2], int num, float pts_out[][2], int kernel)
{
    int half = kernel / 2;
    for (int i = 0; i < num; i++)
    {
        pts_out[i][0] = pts_out[i][1] = 0;
        for (int j = -half; j <= half; j++)
        {
            pts_out[i][0] += pts_in[clip(i + j, 0, num - 1)][0] * (half + 1 - fabs(j));
            pts_out[i][1] += pts_in[clip(i + j, 0, num - 1)][1] * (half + 1 - fabs(j));
        }
        pts_out[i][0] /= (2 * half + 2) * (half + 1) / 2;
        pts_out[i][1] /= (2 * half + 2) * (half + 1) / 2;
    }
}

// 点集等距采样  使走过的采样前折线段的距离为`dist`
void resample_points(float pts_in[][2], int num1, float pts_out[][2], int *num2, float dist) {
    int remain = 0, len = 0;
    for(int i=0; i<num1-1 && len < *num2; i++){
        float x0 = pts_in[i][0];
        float y0 = pts_in[i][1];
        float dx = pts_in[i+1][0] - x0;
        float dy = pts_in[i+1][1] - y0;
        float dn = sqrt(dx*dx+dy*dy);
        dx /= dn;
        dy /= dn;

        while(remain < dn && len < *num2){
            x0 += dx * remain;
            pts_out[len][0] = x0;
            y0 += dy * remain;
            pts_out[len][1] = y0;

            len++;
            dn -= remain;
            remain = dist;
        }
        remain -= dn;
    }
    *num2 = len;
}



// 点集局部角度变化率
void local_angle_points(float pts_in[][2], int num, float angle_out[], int dist)
{
    for (int i = 0; i < num; i++)
    {
        if (i <= 0 || i >= num - 1)
        {
            angle_out[i] = 0;
            continue;
        }
        float dx1 = pts_in[i][0] - pts_in[clip(i - dist, 0, num - 1)][0];
        float dy1 = pts_in[i][1] - pts_in[clip(i - dist, 0, num - 1)][1];
        float dn1 = sqrtf(dx1 * dx1 + dy1 * dy1);
        float dx2 = pts_in[clip(i + dist, 0, num - 1)][0] - pts_in[i][0];
        float dy2 = pts_in[clip(i + dist, 0, num - 1)][1] - pts_in[i][1];
        float dn2 = sqrtf(dx2 * dx2 + dy2 * dy2);
        float c1 = dx1 / dn1;
        float s1 = dy1 / dn1;
        float c2 = dx2 / dn2;
        float s2 = dy2 / dn2;
        angle_out[i] = atan2f(c1 * s2 - c2 * s1, c2 * c1 + s2 * s1);
    }
}

// 角度变化率非极大抑制
void nms_angle(float angle_in[], int num, float angle_out[], int kernel)
{
//    zf_assert(kernel % 2 == 1);
    int half = kernel / 2;
    for (int i = 0; i < num; i++)
    {
        angle_out[i] = angle_in[i];
        for (int j = -half; j <= half; j++)
        {
            if (fabs(angle_in[clip(i + j, 0, num - 1)]) > fabs(angle_out[i]))
            {
                angle_out[i] = 0;
                break;
            }
        }
    }
}


/**
* @brief       大津法计算二值化阈值
* @param       threshold
* @return      threshold 二值化阈值
*/
uint8 OTSU(uint8 *IMG, uint16 col, uint16 row)   //注意计算阈值的一定要是原图像
{
    #define GrayScale 256
    uint16 width = col;
    uint16 height = row;
    uint32 gray_sum=0;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height/4;
    uint8 threshold = 0;

    uint8* data = IMG;  //指向像素数据的指针
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }


    //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < height; i+=2)
    {
        for (j = 0; j < width; j+=2)
        {
            pixelCount[(int)data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
            gray_sum+=(int)data[i * width + j];       //灰度值总和
        }
    }

    //计算每个像素值的点在整幅图像中的比例

    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
    }

    //遍历灰度级[0,255]
    for (j = 0; j < GrayScale; j++)
    {
        w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
        u0tmp += j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值

        w1=1-w0;
        u1tmp=gray_sum/pixelSum-u0tmp;

        u0 = u0tmp / w0;              //背景平均灰度
        u1 = u1tmp / w1;              //前景平均灰度
        u = u0tmp + u1tmp;            //全局平均灰度
        deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = j;
        }
        if (deltaTmp < deltaMax)
        {
            break;
        }
    }
    printf("%d \n",threshold);
    seekfree_wireless_send_buff(&threshold,sizeof(threshold)-1);
    return threshold;
}

/**
* @brief       大津法计算局部区域二值化阈值
* @param       threshold
* @return      threshold 二值化阈值
*/
uint8 OTSUrange(uint8 *IMG, uint16 x0, uint16 y0, uint16 x1, uint16 y1)
{
//    assert(x0 >= 0 && x0 <= MT9V03X_W && y0 >= 0 && y0 <= MT9V03X_H);
//    assert(x1 >= 0 && x1 <= MT9V03X_W && y1 >= 0 && y1 <= MT9V03X_H);
//    assert(x1 >= x0 && y1 >= y0);

    #define GrayScale 256
    uint16 width = x1-x0;
    uint16 height = y1-y0;
    uint32 gray_sum=0;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height/4;
    uint8 threshold = 0;

    uint8* data = IMG;  //指向像素数据的指针
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }


    //统计灰度级中每个像素在整幅图像中的个数
    for (i = y0; i < y1; i += 2)//行
    {
        for (j = x0; j < x1; j += 2)//列
        {
            pixelCount[(int)data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
            gray_sum+=(int)data[i * width + j];       //灰度值总和
        }
    }

    //计算每个像素值的点在整幅图像中的比例

    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
    }

    //遍历灰度级[0,255]
    for (j = 0; j < GrayScale; j++)
    {
        w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
        u0tmp += j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值

        w1=1-w0;
        u1tmp = gray_sum/pixelSum-u0tmp;

        u0 = u0tmp / w0;              //背景平均灰度
        u1 = u1tmp / w1;              //前景平均灰度
        u = u0tmp + u1tmp;            //全局平均灰度
        deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = j;
        }
        if (deltaTmp < deltaMax)
        {
            break;
        }
    }
    //printf("[info] camera.c: threshold : %d \n",threshold);
    return threshold;
}


//自适应二值化
void adaptiveThreshold(uint8* img_data, uint8* output_data, uint16 width, uint16 height, uint16 block, uint8 clip_value)
{
    int half_block = block / 2;
    for(int y = half_block; y < height-half_block; y++)
    {
        for(int x = half_block; x < width-half_block; x++)
        {
            // 计算局部阈值
            int thres = 0;
            for(int dy=-half_block; dy<=half_block; dy++)
            {
                for(int dx=-half_block; dx<=half_block; dx++)
                {
                    thres += img_data[(x+dx)+(y+dy)*width];
                }
            }
            thres = thres / (block * block) - clip_value;
            // 进行二值化
            output_data[x+y*width] = img_data[x+y*width]>thres ? 255 : 0;
        }
    }
}




