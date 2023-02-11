#ifndef _ccu6_pwm_H
#define _ccu6_pwm_H

#include "ifxccu6_regdef.h"
#include "common.h"

#define FCY             ((uint32)100000000)     //CCU6��ʱ��ʱ��
#define FPWM            ((uint16)20000)         //PWMƵ��
#define PWM_PRIOD_LOAD  (uint16)(FCY/FPWM/2)    //PWM����װ��ֵ  2500
#define DEADTIME_LOAD   (50)                    //����װ��ֵ



extern Ifx_CCU6 *ccu6SFR;


void ccu6_pwm_init(void);
uint8 ccu61_get_trap_flag(void);

#endif
