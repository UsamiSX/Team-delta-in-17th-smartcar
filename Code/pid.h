#ifndef _PID_H
#define _PID_H

#include "common.h"

typedef struct
{
    float kp;           //PI�ջ��ı�������
    float ki;           //PI�ջ��Ļ��ֲ���
    float kd;
    int32 real_speed;   //��ǰ���ת��
    int32 error;        //Ŀ���ٶ���ʵ���ٶ����
    int32 lastError;
    int32 last2Error;
    int32 out_max;      //�ջ�������
    float pout;         //�ջ����ֵ
    float iout;         //�ջ����ֵ
    float dout;         //�ջ����ֵ
    int32 out;          //�ջ����ֵ
}brushless_highSpeed_struct;

typedef struct
{
    float kp;           //PI�ջ��ı�������
    float ki;           //PI�ջ��Ļ��ֲ���
    float kd;
    int32 real_speed;   //��ǰ���ת��
    int32 error;        //Ŀ���ٶ���ʵ���ٶ����
    int32 lastError;
    int32 last2Error;
    int32 out_max;      //�ջ�������
    float pout;         //�ջ����ֵ
    float iout;         //�ջ����ֵ
    float dout;         //�ջ����ֵ
    int32 out;          //�ջ����ֵ
}brushless_lowSpeed_struct;

typedef struct
{
    float kp;           //PI�ջ��ı�������
    float ki;           //PI�ջ��Ļ��ֲ���
    float kd;
    float realValue;   //��ǰ���ת��
    float error;        //Ŀ���ٶ���ʵ���ٶ����
    float lastError;
    float last2Error;
    float setValue;
    float out_max;      //�ջ�������
    float pout;         //�ջ����ֵ
    float iout;         //�ջ����ֵ
    float dout;         //�ջ����ֵ
    float out;          //�ջ����ֵ
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
