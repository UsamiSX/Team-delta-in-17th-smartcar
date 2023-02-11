#ifndef _isr_h
#define _isr_h

#include "headfile.h"
#include "zf_gpt12.h"


extern eulerianAngles_t eulerAngle;
extern icm_t icm;
extern float angleX;
extern float k_roll, k_pitch;
extern short gyroX,gyroY,gyroZ;
extern short accX,accY,accZ;
extern float gy_offset;
extern uint16 buffer[8];//L2, L3, L4, 0, L1, 0, 0, 0
extern pid_position_struct motor;
extern pid_position_struct servo;

#endif

