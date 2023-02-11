#ifndef _HALL_H
#define _HALL_H

#include "common.h"

//定义霍尔值输入引脚
#define HALLA_PIN   P15_5
#define HALLB_PIN   P15_7
#define HALLC_PIN   P15_8

//定义电机极对数
#define POLEPAIRS           1

#define PWM_PIT_NUM         60*20000  //20000  1000

#define COMMUTATION_TIMEOUT 5000


#define MOTOR_STOP  0
#define MOTOR_RUN   1
#define DELAY_MAX   20  //延时换相最大时长

extern uint32   commutation_delay;
extern int8     hall_value_now;     //当前霍尔的值
extern int8     next_hall_value;    //下一次霍尔的值
extern uint16   commutation_time;
extern uint16   commutation_delay_ratio;
extern uint8 motor_state;

void hall_init(void);
void read_hall_value(void);
void scan_hall_status(void);
extern inline void calc_speed(void);

#endif
