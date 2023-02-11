/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2021,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����ϵ�Ա��ͷ�
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            move_filter
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ3184284598)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        ADS v1.5.2
 * @Target core     TC264D
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-12-10
 ********************************************************************************************************************/

#include "move_filter.h"

move_filter_struct speed_filter;


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ƽ���˲�����
//  @param      void
//  @return     void
//  @since      ��Ҫ���ڶ������˲����洢Ŀ�����������n�����ݣ������ƽ��ֵ
//-------------------------------------------------------------------------------------------------------------------
void move_filter_calc(move_filter_struct *move_filter, int32 new_data)
{
//    if(abs(new_data - speed_filter.lastData) >= 3000) {
//        new_data = speed_filter.lastData;
//    }
    //�����µ���ֵ ��ȥ��ĩβ����ֵ ������µĺ�
    speed_filter.data_sum = speed_filter.data_sum + new_data - speed_filter.data_buffer[speed_filter.index];
    //������ƽ��ֵ
    speed_filter.data_average = speed_filter.data_sum / speed_filter.buffer_size;

    //������д�뻺����
    speed_filter.data_buffer[speed_filter.index] = new_data;
    speed_filter.index++;
    if(speed_filter.buffer_size <= speed_filter.index)
    {
        speed_filter.index = 0;
    }

//    if(myabs(speed_filter.data_average - speed_filter.lastData) >= 1000) speed_filter.data_average = speed_filter.lastData;
    speed_filter.lastData = new_data;
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ƽ���˲���ʼ��
//  @param      void
//  @return     void
//  @since      ��Ҫ���ڶ������˲����洢Ŀ�����������n�����ݣ������ƽ��ֵ
//-------------------------------------------------------------------------------------------------------------------
void move_filter_init(move_filter_struct *move_filter)
{
    speed_filter.data_average = 0;
    speed_filter.data_sum = 0;
    speed_filter.index = 0;
    //���û�������С
    speed_filter.buffer_size = MOVE_AVERAGE_SIZE;

    uint8 i;
    for(i=0; i < speed_filter.buffer_size; i++)
    {
        speed_filter.data_buffer[i] = 0;
    }
}




