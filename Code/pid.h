#ifndef _PID_H
#define _PID_H

#include "common.h"

typedef struct
{
    float kp;           //PI闭环的比例参数
    float ki;           //PI闭环的积分参数
    float kd;
    int32 real_speed;   //当前电机转速
    int32 error;        //目标速度与实际速度误差
    int32 lastError;
    int32 last2Error;
    int32 out_max;      //闭环最大输出
    float pout;         //闭环输出值
    float iout;         //闭环输出值
    float dout;         //闭环输出值
    int32 out;          //闭环输出值
}brushless_highSpeed_struct;

typedef struct
{
    float kp;           //PI闭环的比例参数
    float ki;           //PI闭环的积分参数
    float kd;
    int32 real_speed;   //当前电机转速
    int32 error;        //目标速度与实际速度误差
    int32 lastError;
    int32 last2Error;
    int32 out_max;      //闭环最大输出
    float pout;         //闭环输出值
    float iout;         //闭环输出值
    float dout;         //闭环输出值
    int32 out;          //闭环输出值
}brushless_lowSpeed_struct;

typedef struct
{
    float kp;           //PI闭环的比例参数
    float ki;           //PI闭环的积分参数
    float kd;
    float realValue;   //当前电机转速
    float error;        //目标速度与实际速度误差
    float lastError;
    float last2Error;
    float setValue;
    float out_max;      //闭环最大输出
    float pout;         //闭环输出值
    float iout;         //闭环输出值
    float dout;         //闭环输出值
    float out;          //闭环输出值
}pid_position_struct;

typedef struct {
    float k1;
    float k2;
    float k3;
    int32 speed;
} lqr_t;

typedef struct
{
    int16 error;
    int16 lastError;
    float kp;
    float ki;
    float kd;
    int16 duty;
} motor_pid_struct;

extern brushless_highSpeed_struct brushless_highSpeed;
extern brushless_lowSpeed_struct brushless_lowSpeed;
extern pid_position_struct angleSpeed;
extern pid_position_struct angle;
extern pid_position_struct brushlessSpeed;

int32 LQR_control(int angle, float gyro, int32 speed);
int32 brushless_highSpeed_pid(int32 read_error);
int32 brushless_lowSpeed_pid(int32 read_error);
int32 motor_pid(int32 error);
float angleSpeed_pid(float read_error);
float angle_pid(float read_error);
float angle_pid_gyro(float read_error, float gyro);
float brushlessSpeed_pid(float read_error);
float brushlessSpeed_incremental_pid(float read_error);
int32 servo_pid(float error);
void brushless_highSpeed_pid_init(void);
void brushless_lowSpeed_pid_init(void);
void brushlessSpeed_pid_init(void);
void servo_pid_init(void);
float DiffRSum(uint16 data1, uint16 data2);
float angle_incremental_pid(float read_error);
float angle_pid_zero(float read_error, float gyro);


#endif
