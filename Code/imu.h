#ifndef _IMU_H
#define _IMU_H

#include "headfile.h"
#include "common.h"

typedef struct{
    float q0;
    float q1;
    float q2;
    float q3;
}quaterInfo_t;

typedef struct{
    float pitch;
    float roll;
    float yaw;
}eulerianAngles_t;

typedef struct{
    float accX;
    float accY;
    float accZ;
    float gyroX;
    float gyroY;
    float gyroZ;
}icm_t;


extern float values[10];

void IMU_getValues();
void IMU_quaterToEulerianAngles(void);
void Kalman_Filter();
void imu_offset();


#endif
