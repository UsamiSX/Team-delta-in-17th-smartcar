#include "zf_gpio.h"
#include "hall.h"
#include "pid.h"
#include "bldc_config.h"
#include "IfxCcu6.h"
#include "ccu6_pwm.h"
#include "zf_gtm_pwm.h"
#include "move_filter.h"
#include "SEEKFREE_FUN.h"
#include "motor.h"

int16 brushlessDuty;

motor_struct motor_control;
extern pid_position_struct brushlessSpeed;
uint8 clear_i = 0;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      电机参数初始化
//  @param      void
//  @return     void
//  @since
//-------------------------------------------------------------------------------------------------------------------
void brushless_motor_init(void)
{
    motor_control.brake_flag = 1;                   //刹车使能
    motor_control.dir = FORWARD;                    //设置默认的方向
    motor_control.set_speed = 0;                    //设置速度
    motor_control.max_speed = BLDC_MAX_SPEED;       //设置最大正转速度
    motor_control.min_speed = -BLDC_MAX_SPEED;      //设置最大反转速度
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      开启A相上桥，B相下桥     其他关闭
//  @param      duty    ：  PWM占空比
//  @return     void
//  @since
//-------------------------------------------------------------------------------------------------------------------
void mos_q1q4_open(uint16 duty)
{
    ccu6SFR->MODCTR.B.T12MODEN = 0x38;

    IfxCcu6_setT12CompareValue(ccu6SFR, IfxCcu6_T12Channel_2, brushlessDuty);
    IfxCcu6_setT12CompareValue(ccu6SFR, IfxCcu6_T12Channel_1, 0);
    IfxCcu6_enableShadowTransfer(ccu6SFR, TRUE, FALSE);
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      开启A相上桥，C相下桥     其他关闭
//  @param      duty    ：  PWM占空比
//  @return     void
//  @since
//-------------------------------------------------------------------------------------------------------------------
void mos_q1q6_open(uint16 duty)
{
    ccu6SFR->MODCTR.B.T12MODEN = 0x32;

    IfxCcu6_setT12CompareValue(ccu6SFR, IfxCcu6_T12Channel_2, brushlessDuty);
    IfxCcu6_setT12CompareValue(ccu6SFR, IfxCcu6_T12Channel_0, 0);
    IfxCcu6_enableShadowTransfer(ccu6SFR, TRUE, FALSE);
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      开启B相上桥，A相下桥     其他关闭
//  @param      duty    ：  PWM占空比
//  @return     void
//  @since
//-------------------------------------------------------------------------------------------------------------------
void mos_q3q2_open(uint16 duty)
{
    ccu6SFR->MODCTR.B.T12MODEN = 0x3C;

    IfxCcu6_setT12CompareValue(ccu6SFR, IfxCcu6_T12Channel_1, brushlessDuty);
    IfxCcu6_setT12CompareValue(ccu6SFR, IfxCcu6_T12Channel_2, 0);
    IfxCcu6_enableShadowTransfer(ccu6SFR, TRUE, FALSE);
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      开启B相上桥，C相下桥     其他关闭
//  @param      duty    ：  PWM占空比
//  @return     void
//  @since
//-------------------------------------------------------------------------------------------------------------------
void mos_q3q6_open(uint16 duty)
{
    ccu6SFR->MODCTR.B.T12MODEN = 0x0E;

    IfxCcu6_setT12CompareValue(ccu6SFR, IfxCcu6_T12Channel_1, brushlessDuty);
    IfxCcu6_setT12CompareValue(ccu6SFR, IfxCcu6_T12Channel_0, 0);
    IfxCcu6_enableShadowTransfer(ccu6SFR, TRUE, FALSE);
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      开启C相上桥，A相下桥     其他关闭
//  @param      duty    ：  PWM占空比
//  @return     void
//  @since
//-------------------------------------------------------------------------------------------------------------------
void mos_q5q2_open(uint16 duty)
{
    ccu6SFR->MODCTR.B.T12MODEN = 0x23;

    IfxCcu6_setT12CompareValue(ccu6SFR, IfxCcu6_T12Channel_0, brushlessDuty);
    IfxCcu6_setT12CompareValue(ccu6SFR, IfxCcu6_T12Channel_2, 0);
    IfxCcu6_enableShadowTransfer(ccu6SFR, TRUE, FALSE);
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      开启C相上桥，B相下桥     其他关闭
//  @param      duty    ：  PWM占空比
//  @return     void
//  @since
//-------------------------------------------------------------------------------------------------------------------
void mos_q5q4_open(uint16 duty)
{
    ccu6SFR->MODCTR.B.T12MODEN = 0x0B;

    IfxCcu6_setT12CompareValue(ccu6SFR, IfxCcu6_T12Channel_0, brushlessDuty);
    IfxCcu6_setT12CompareValue(ccu6SFR, IfxCcu6_T12Channel_1, 0);
    IfxCcu6_enableShadowTransfer(ccu6SFR, TRUE, FALSE);
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      关闭所有上桥，开启所有下桥，使电机在线圈的自感电动势的作用下自己完成刹车
//  @param      void
//  @return     void
//  @since
//-------------------------------------------------------------------------------------------------------------------
void mos_close(void)
{
    ccu6SFR->MODCTR.B.T12MODEN = 0x2A;
    IfxCcu6_setT12CompareValue(ccu6SFR, IfxCcu6_T12Channel_0, 0);
    IfxCcu6_setT12CompareValue(ccu6SFR, IfxCcu6_T12Channel_1, 0);
    IfxCcu6_setT12CompareValue(ccu6SFR, IfxCcu6_T12Channel_2, 0);
    IfxCcu6_enableShadowTransfer(ccu6SFR, TRUE, FALSE);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      电机换相控制
//  @param      void
//  @return     void
//  @since      根据计算出来的目标相位进行相位输出
//-------------------------------------------------------------------------------------------------------------------
void motor_commutation(int8 except_hall)
{
    //使能开关关闭时 无论如何都关闭输出
    if(!gpio_get(EN_PIN))
    {
        brushlessDuty=0;
        clear_i = 1;
    }
    else {
        if(clear_i == 1) {

            clear_i = 0;
            brushlessSpeed.pout = 0;         //闭环输出值
            brushlessSpeed.iout = 0;         //闭环输出值
            brushlessSpeed.dout = 0;         //闭环输出值
            brushlessSpeed.out = 0;
            angleSpeed.pout = 0;
            angleSpeed.iout = 0;
            angleSpeed.dout = 0;
            angleSpeed.out = 0;
            angle.pout = 0;
            angle.iout = 0;
            angle.dout = 0;
            angle.out = 0;

        }

    }
    if(brushlessDuty) {
        switch(except_hall) {
           case 1:     mos_q1q6_open(brushlessDuty);    break;
           case 2:     mos_q1q4_open(brushlessDuty);    break;
           case 3:     mos_q5q4_open(brushlessDuty);    break;
           case 4:     mos_q5q2_open(brushlessDuty);    break;
           case 5:     mos_q3q2_open(brushlessDuty);    break;
           case 6:     mos_q3q6_open(brushlessDuty);    break;
           default:    mos_close();            break;
        }
    }
    else {
        mos_close();
    }
}
