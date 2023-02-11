#ifndef _MOVE_FILTER_H
#define _MOVE_FILTER_H

#include "common.h"

#define MOVE_AVERAGE_SIZE   32//32  //���建������С


typedef struct
{
    uint8 index;            //�±�
    uint8 buffer_size;      //buffer��С
    int32 data_buffer[MOVE_AVERAGE_SIZE];  //������
    int32 data_sum;         //���ݺ�
    int32 data_average;     //����ƽ��ֵ
    int32 lastData;
}move_filter_struct;

extern move_filter_struct speed_filter;


void move_filter_init(move_filter_struct *move_average);
void move_filter_calc(move_filter_struct *move_average, int32 new_data);



#endif /* CODE_MOVE_FILTER_H_ */
