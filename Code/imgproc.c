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


/* ǰ�������壺
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
 * @brief findline_lefthand_adaptive �����Թ�Ѳ��
 *
 * @param   *img          �����ͼ������
 * @param   block_size    ����˴�С(����)
 * @param   clip_value    ��ֵ��У׼ֵ
 * @param   x             Ҫ��ֵ����λ��x����
 * @param   y             Ҫ��ֵ����λ��y����
 * @param   pts           ԭͼ���ұ������
 * @param   *num          ͼ��߶�
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
        //��ֵ����ʼ
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
        //��ֵ������

        //��ʼѰ�ұ���
        //uint16_t current_value = AT(img, x, y);yָ�����У�xָ������
        uint16_t front_value = AT(img, x + dir_front[dir][0], y + dir_front[dir][1]);//Ҫ��ֵ����ͼ���
        uint16_t frontleft_value = AT(img, x + dir_frontleft[dir][0], y + dir_frontleft[dir][1]);
        if (front_value < local_thres) //˵��ǰ���Ǻ�ɫ����ת��ת��Ĵ���Ϊdir+1
        {
            dir = (dir + 1) % 4;//dirֻ��ȡ0 1 2 3�ĸ���������ȡ��4��˳ʱ�뻻��
            turn++;  //�����Ĵ���ǰȫ�Ǻ�ɫ�����˳�ѭ���������������ǽ�ߣ������߽���ɫ�İ�ΧȦ��
        }
        else if (frontleft_value < local_thres) //˵��ǰ����ǰ�ڣ�����ǰ�ߣ�����Ϊpts������ǰһ�������
        {
            x += dir_front[dir][0];
            y += dir_front[dir][1];
            pts[step][0] = x;
            pts[step][1] = y;
            step++;//0-ͼƬ�߶ȴ�������
            turn = 0;
        }
        else //ǰ����ǰ�ף���pts��������ǰһ������꣬�һ���
        {
            x += dir_frontleft[dir][0];
            y += dir_frontleft[dir][1];
            dir = (dir + 3) % 4;//��ʱ�뻻��
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        }
    }
    *num = step;
}


/**
 * @brief findline_righthand_adaptive �����Թ�Ѳ��
 *
 * @param   *img          �����ͼ������
 * @param   block_size    ����˴�С(����)
 * @param   clip_value    ��ֵ��У׼ֵ   ��ֵ�������ֵ��ȥ��ֵ
 * @param   x             Ҫ��ֵ����λ��x����
 * @param   y             Ҫ��ֵ����λ��x����
 * @param   pts           ԭͼ���ұ������
 * @param   *num          ͼ��߶�
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
        //��ֵ����ʼ
        int local_thres = 0;
        for (int dy = -half; dy <= half; dy++) {
            for (int dx = -half; dx <= half; dx++) {
                local_thres += AT(img, x + dx, y + dy);
            }
        }
        local_thres /= block_size * block_size;
        local_thres -= clip_value;
        //��ֵ������


        //��ʼѰ�ұ���
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

// �㼯�Ⱦ����  ʹ�߹��Ĳ���ǰ���߶εľ���Ϊ`dist`
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



// �㼯�ֲ��Ƕȱ仯��
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

// �Ƕȱ仯�ʷǼ�������
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
* @brief       ��򷨼����ֵ����ֵ
* @param       threshold
* @return      threshold ��ֵ����ֵ
*/
uint8 OTSU(uint8 *IMG, uint16 col, uint16 row)   //ע�������ֵ��һ��Ҫ��ԭͼ��
{
    #define GrayScale 256
    uint16 width = col;
    uint16 height = row;
    uint32 gray_sum=0;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height/4;
    uint8 threshold = 0;

    uint8* data = IMG;  //ָ���������ݵ�ָ��
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }


    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    for (i = 0; i < height; i+=2)
    {
        for (j = 0; j < width; j+=2)
        {
            pixelCount[(int)data[i * width + j]]++;  //����ǰ�ĵ������ֵ��Ϊ����������±�
            gray_sum+=(int)data[i * width + j];       //�Ҷ�ֵ�ܺ�
        }
    }

    //����ÿ������ֵ�ĵ�������ͼ���еı���

    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
    }

    //�����Ҷȼ�[0,255]
    for (j = 0; j < GrayScale; j++)
    {
        w0 += pixelPro[j];  //��������ÿ���Ҷ�ֵ�����ص���ռ����֮��   ���������ֵı���
        u0tmp += j * pixelPro[j];  //�������� ÿ���Ҷ�ֵ�ĵ�ı��� *�Ҷ�ֵ

        w1=1-w0;
        u1tmp=gray_sum/pixelSum-u0tmp;

        u0 = u0tmp / w0;              //����ƽ���Ҷ�
        u1 = u1tmp / w1;              //ǰ��ƽ���Ҷ�
        u = u0tmp + u1tmp;            //ȫ��ƽ���Ҷ�
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
* @brief       ��򷨼���ֲ������ֵ����ֵ
* @param       threshold
* @return      threshold ��ֵ����ֵ
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

    uint8* data = IMG;  //ָ���������ݵ�ָ��
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }


    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    for (i = y0; i < y1; i += 2)//��
    {
        for (j = x0; j < x1; j += 2)//��
        {
            pixelCount[(int)data[i * width + j]]++;  //����ǰ�ĵ������ֵ��Ϊ����������±�
            gray_sum+=(int)data[i * width + j];       //�Ҷ�ֵ�ܺ�
        }
    }

    //����ÿ������ֵ�ĵ�������ͼ���еı���

    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
    }

    //�����Ҷȼ�[0,255]
    for (j = 0; j < GrayScale; j++)
    {
        w0 += pixelPro[j];  //��������ÿ���Ҷ�ֵ�����ص���ռ����֮��   ���������ֵı���
        u0tmp += j * pixelPro[j];  //�������� ÿ���Ҷ�ֵ�ĵ�ı��� *�Ҷ�ֵ

        w1=1-w0;
        u1tmp = gray_sum/pixelSum-u0tmp;

        u0 = u0tmp / w0;              //����ƽ���Ҷ�
        u1 = u1tmp / w1;              //ǰ��ƽ���Ҷ�
        u = u0tmp + u1tmp;            //ȫ��ƽ���Ҷ�
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


//����Ӧ��ֵ��
void adaptiveThreshold(uint8* img_data, uint8* output_data, uint16 width, uint16 height, uint16 block, uint8 clip_value)
{
    int half_block = block / 2;
    for(int y = half_block; y < height-half_block; y++)
    {
        for(int x = half_block; x < width-half_block; x++)
        {
            // ����ֲ���ֵ
            int thres = 0;
            for(int dy=-half_block; dy<=half_block; dy++)
            {
                for(int dx=-half_block; dx<=half_block; dx++)
                {
                    thres += img_data[(x+dx)+(y+dy)*width];
                }
            }
            thres = thres / (block * block) - clip_value;
            // ���ж�ֵ��
            output_data[x+y*width] = img_data[x+y*width]>thres ? 255 : 0;
        }
    }
}




