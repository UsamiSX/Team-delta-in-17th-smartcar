#include "ifxCcu6.h"
#include "bldc_config.h"
#include "isr_config.h"
#include "ccu6_pwm.h"

Ifx_CCU6 *ccu6SFR = &MODULE_CCU61;
void ccu6_pwm_init(void)
{

    IfxCcu6_enableModule(ccu6SFR);
    if (IfxCcu6_getTimerAvailabilityStatus(ccu6SFR, IfxCcu6_TimerId_t12) == FALSE)
    {
        IfxCcu6_enableTimer(ccu6SFR, IfxCcu6_TimerId_t12);
    }

    IfxCcu6_setT12Frequency(ccu6SFR, (float32)FCY, FCY/FPWM, IfxCcu6_T12CountMode_centerAligned);
    IfxCcu6_setT12CounterValue(ccu6SFR, 0);

    IfxCcu6_setDeadTimeValue(ccu6SFR, 50);
    IfxCcu6_enableDeadTime(ccu6SFR, IfxCcu6_T12Channel_0);
    IfxCcu6_enableDeadTime(ccu6SFR, IfxCcu6_T12Channel_1);
    IfxCcu6_enableDeadTime(ccu6SFR, IfxCcu6_T12Channel_2);

    IfxCcu6_setT12ChannelMode(ccu6SFR, IfxCcu6_T12Channel_0, IfxCcu6_T12ChannelMode_compareMode);
    IfxCcu6_setT12ChannelMode(ccu6SFR, IfxCcu6_T12Channel_1, IfxCcu6_T12ChannelMode_compareMode);
    IfxCcu6_setT12ChannelMode(ccu6SFR, IfxCcu6_T12Channel_2, IfxCcu6_T12ChannelMode_compareMode);

    IfxCcu6_setT12CompareValue(ccu6SFR, IfxCcu6_T12Channel_0, 0);
    IfxCcu6_setT12CompareValue(ccu6SFR, IfxCcu6_T12Channel_1, 0);
    IfxCcu6_setT12CompareValue(ccu6SFR, IfxCcu6_T12Channel_2, 0);

    IfxCcu6_enableModulationOutput(ccu6SFR, IfxCcu6_TimerId_t12, IfxCcu6_ChannelOut_cc0);
    IfxCcu6_enableModulationOutput(ccu6SFR, IfxCcu6_TimerId_t12, IfxCcu6_ChannelOut_cc1);
    IfxCcu6_enableModulationOutput(ccu6SFR, IfxCcu6_TimerId_t12, IfxCcu6_ChannelOut_cc2);
    IfxCcu6_enableModulationOutput(ccu6SFR, IfxCcu6_TimerId_t12, IfxCcu6_ChannelOut_cout0);
    IfxCcu6_enableModulationOutput(ccu6SFR, IfxCcu6_TimerId_t12, IfxCcu6_ChannelOut_cout1);
    IfxCcu6_enableModulationOutput(ccu6SFR, IfxCcu6_TimerId_t12, IfxCcu6_ChannelOut_cout2);

    ccu6SFR->CMPSTAT.U = (ccu6SFR->CMPSTAT.U & (~((uint16)0x3f<<8))) | (0x15<<8);


    const IfxCcu6_Cc60_Out *cc60Out = &IfxCcu61_CC60_P33_13_OUT;
    const IfxCcu6_Cc61_Out *cc61Out = &IfxCcu61_CC61_P33_11_OUT;
    const IfxCcu6_Cc62_Out *cc62Out = &IfxCcu61_CC62_P33_9_OUT;
    IfxCcu6_Cout60_Out *cout60 = &IfxCcu61_COUT60_P33_12_OUT;
    IfxCcu6_Cout61_Out *cout61 = &IfxCcu61_COUT61_P33_10_OUT;
    IfxCcu6_Cout62_Out *cout62 = &IfxCcu61_COUT62_P33_8_OUT;
    IfxCcu6_initCc60OutPin(cc60Out, IfxPort_OutputMode_pushPull, IfxPort_PadDriver_cmosAutomotiveSpeed1);//Q1
    IfxCcu6_initCc61OutPin(cc61Out, IfxPort_OutputMode_pushPull, IfxPort_PadDriver_cmosAutomotiveSpeed1);//Q3
    IfxCcu6_initCc62OutPin(cc62Out, IfxPort_OutputMode_pushPull, IfxPort_PadDriver_cmosAutomotiveSpeed1);//Q5
    IfxCcu6_initCout60Pin(cout60, IfxPort_OutputMode_pushPull, IfxPort_PadDriver_cmosAutomotiveSpeed1);//Q2
    IfxCcu6_initCout61Pin(cout61, IfxPort_OutputMode_pushPull, IfxPort_PadDriver_cmosAutomotiveSpeed1);//Q4
    IfxCcu6_initCout62Pin(cout62, IfxPort_OutputMode_pushPull, IfxPort_PadDriver_cmosAutomotiveSpeed1);//Q6

    //��ʼ��trap pin���ţ�����ɲ������
    IfxCcu6_initCtrapPinWithPadLevel(&IfxCcu61_CTRAPC_P33_4_IN, IfxPort_InputMode_pullUp, IfxPort_PadDriver_cmosAutomotiveSpeed1);
    //����ͨ������trap
    IfxCcu6_enableTrap(ccu6SFR, IfxCcu6_ChannelOut_cc0);
    IfxCcu6_enableTrap(ccu6SFR, IfxCcu6_ChannelOut_cc1);
    IfxCcu6_enableTrap(ccu6SFR, IfxCcu6_ChannelOut_cc2);
    IfxCcu6_enableTrap(ccu6SFR, IfxCcu6_ChannelOut_cout0);
    IfxCcu6_enableTrap(ccu6SFR, IfxCcu6_ChannelOut_cout1);
    IfxCcu6_enableTrap(ccu6SFR, IfxCcu6_ChannelOut_cout2);
    IfxCcu6_setTrapMode(ccu6SFR, IfxCcu6_TrapMode_automatic);
    IfxCcu6_enableTrapPin(ccu6SFR);

    IfxCcu6_enableShadowTransfer(ccu6SFR, TRUE, FALSE);
    IfxCcu6_startTimer(ccu6SFR, TRUE, FALSE);

    IfxCcu6_enableInterrupt(ccu6SFR, IfxCcu6_InterruptSource_t12PeriodMatch);
    IfxCcu6_routeInterruptNode(ccu6SFR, IfxCcu6_InterruptSource_t12PeriodMatch, IfxCcu6_ServiceRequest_2);

    volatile Ifx_SRC_SRCR *src;
    src = IfxCcu6_getSrcAddress(ccu6SFR, IfxCcu6_ServiceRequest_2);
    IfxSrc_init(src, CCU60_T12_INT_SERVICE, CCU60_T12_ISR_PRIORITY);
    IfxSrc_enable(src);
}


uint8 ccu61_get_trap_flag(void)
{
   return ccu6SFR->IS.B.TRPS;
}


