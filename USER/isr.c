#include "isr_config.h"
#include "isr.h"

#define ANGLE_ZERO 95.5

uint8 count2ms = 0; //时间片计数
uint8 countMotor = 0; //时间片计数
int numRotary = 0; //旋转编码器计数
int numRotary_n0 = 0;
int numRotary_n_0 = 0;
int numRotary_n1 = 0;
int16 motorSpeed;
int16 motorDuty;
uint16 servoDuty = 620;
uint16 servoDutyLast;
float angle_zero = 94.8;
float angleT;
float angle_zero_ratio0 = -0.009;
float angle_zero_ratio1= -0.009;
float gyro;
float Battery_voltage;


//时间片定时器
IFX_INTERRUPT(cc60_pit_ch0_isr, CCU6_0_CH0_INT_VECTAB_NUM, CCU6_0_CH0_ISR_PRIORITY)
{
	enableInterrupts();//开启中断嵌套
	PIT_CLEAR_FLAG(CCU6_0, PIT_CH0);

    if(count2ms == 0) {
        Kalman_Filter();

        k_pitch *= 100;
        k_pitch = (int)k_pitch;
        k_pitch = (float)k_pitch/100;

        angleT = k_pitch+angle_zero;

        gyro = (icm.gyroY-gy_offset);

        gyro *= 100;
        gyro = (int)gyro;
        gyro = (float)gyro/100;

        angle.setValue = -brushlessSpeed_incremental_pid(speed_filter.data_average);
        angle.setValue /= 10000;

        angleSpeed.setValue = -angle_pid(k_pitch+angle_zero - angle.setValue);

        brushlessDuty = (int16)angleSpeed_pid(icm.gyroY-gy_offset - angleSpeed.setValue);
//
//
//        printf("%d, %d, %d, %f, %f, %f, %f\n", speed_filter.data_average, motor_control.set_speed, brushlessDuty, (k_pitch + angle_zero), gyro, angle.setValue, angleSpeed.setValue);
//
//        printf("%d, %d, %f\n", speed_filter.data_average, brushlessDuty, k_pitch+angle_zero);
        if(brushlessDuty >= 0) {
            motor_control.dir = FORWARD;
        }
        else if(brushlessDuty < 0) {
            motor_control.dir = REVERSE;
            brushlessDuty = -brushlessDuty;
        }
        if(k_pitch+angle_zero >= 15 || k_pitch+angle_zero <= -15) brushlessDuty = 0;


    }

    else if(count2ms == 1) {
//        if(countMotor >= 800) {
//            motorSpeed = gpt12_get(GPT12_T6);
//            gpt12_clear(GPT12_T6);
//
//            motor.setValue = 100;
//
//            motorDuty = -motor_pid(motorSpeed - motor.setValue);
//            if(k_pitch+angle_zero >= 15 || k_pitch+angle_zero <= -15) motorDuty = 0;
//            if(motorDuty >= 0) {
//                gpio_set(P33_1, 1); //正转
//                motor_duty(motorDuty);
//            }
//            else {
//                motorDuty = 0;
//            }
////        }
//        if(countMotor <= 800) {
//            countMotor++;
//        }
//        printf("%d, %d, %f, %f, %f\n", motorSpeed, motorDuty, motor.setValue, motor.pout, motor.out);
    }
    else if(count2ms == 2) {
        servo.setValue = DiffRSum(buffer[2], buffer[4]);
        servoDuty = 720; // servo_pid(servo.setValue);
//        if(servoDuty != servoDutyLast) {
//            if(servoDuty >= 720) angle_zero = ANGLE_ZERO + angle_zero_ratio0 * (servoDuty - 720);
//            if(servoDuty <= 720) angle_zero = ANGLE_ZERO + angle_zero_ratio1 * (servoDuty - 720);
//        }
//        servo_duty(servoDuty); //620 96.4 -0.9  820 94.6 0.9
//        printf("%d, %d, %d, %d, \n", buffer[4], buffer[0], buffer[1],  buffer[2]); //L1 L2 L3 L4
        servoDutyLast = servoDuty;
    }
    else if(count2ms == 3) {
//        Battery_voltage = (float)adc_convert(ADC_0, ADC0_CH1_A1, ADC_12BIT)/4096 * 3.3 * 4;
//        printf("%f\n", Battery_voltage);
    }
    count2ms++;

    if(count2ms == 5) count2ms = 0;
}

//电机换相
IFX_INTERRUPT(cc60_pit_ch1_isr, CCU6_0_CH1_INT_VECTAB_NUM, CCU6_0_CH1_ISR_PRIORITY)
{
	enableInterrupts();//开启中断嵌套
	PIT_CLEAR_FLAG(CCU6_0, PIT_CH1);

    scan_hall_status();

    if(1 > commutation_delay--)
    {
        commutation_delay = 0;
        motor_commutation(next_hall_value);
    }
}

//用于输出无刷电机6路PWM
//IFX_INTERRUPT(cc61_pit_ch0_isr, CCU6_1_CH0_INT_VECTAB_NUM, CCU6_1_CH0_ISR_PRIORITY)
//{
//	enableInterrupts();//开启中断嵌套
//	PIT_CLEAR_FLAG(CCU6_1, PIT_CH0);
//
//}
//
//IFX_INTERRUPT(cc61_pit_ch1_isr, CCU6_1_CH1_INT_VECTAB_NUM, CCU6_1_CH1_ISR_PRIORITY)
//{
//	enableInterrupts();//开启中断嵌套
//	PIT_CLEAR_FLAG(CCU6_1, PIT_CH1);
//
//}


IFX_INTERRUPT(eru_ch0_ch4_isr, ERU_CH0_CH4_INT_VECTAB_NUM, ERU_CH0_CH4_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
	if(GET_GPIO_FLAG(ERU_CH0_REQ4_P10_7))//通道0中断
	{
		CLEAR_GPIO_FLAG(ERU_CH0_REQ4_P10_7);
	}

	if(GET_GPIO_FLAG(ERU_CH4_REQ13_P15_5))//通道4中断
	{
		CLEAR_GPIO_FLAG(ERU_CH4_REQ13_P15_5);
	}
}

IFX_INTERRUPT(eru_ch1_ch5_isr, ERU_CH1_CH5_INT_VECTAB_NUM, ERU_CH1_CH5_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
	if(GET_GPIO_FLAG(ERU_CH1_REQ5_P10_8))//通道1中断
	{
		CLEAR_GPIO_FLAG(ERU_CH1_REQ5_P10_8);
	}

	if(GET_GPIO_FLAG(ERU_CH5_REQ1_P15_8))//通道5中断
	{
		CLEAR_GPIO_FLAG(ERU_CH5_REQ1_P15_8);
	}
}

//由于摄像头pclk引脚默认占用了 2通道，用于触发DMA，因此这里不再定义中断函数
//IFX_INTERRUPT(eru_ch2_ch6_isr, ERU_CH2_CH6_INT_VECTAB_NUM, ERU_CH2_CH6_INT_PRIO)
//{
//	enableInterrupts();//开启中断嵌套
//	if(GET_GPIO_FLAG(ERU_CH2_REQ7_P00_4))//通道2中断
//	{
//		CLEAR_GPIO_FLAG(ERU_CH2_REQ7_P00_4);
//
//	}
//	if(GET_GPIO_FLAG(ERU_CH6_REQ9_P20_0))//通道6中断
//	{
//		CLEAR_GPIO_FLAG(ERU_CH6_REQ9_P20_0);
//
//	}
//}



IFX_INTERRUPT(eru_ch3_ch7_isr, ERU_CH3_CH7_INT_VECTAB_NUM, ERU_CH3_CH7_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
	if(GET_GPIO_FLAG(ERU_CH3_REQ6_P02_0))//通道3中断
	{
		CLEAR_GPIO_FLAG(ERU_CH3_REQ6_P02_0);
		if		(CAMERA_GRAYSCALE == camera_type)	mt9v03x_vsync();
		else if (CAMERA_BIN_UART  == camera_type)	ov7725_uart_vsync();
		else if	(CAMERA_BIN       == camera_type)	ov7725_vsync();

	}
	if(GET_GPIO_FLAG(ERU_CH7_REQ16_P15_1))//通道7中断
	{
		CLEAR_GPIO_FLAG(ERU_CH7_REQ16_P15_1);

	}
}



IFX_INTERRUPT(dma_ch5_isr, ERU_DMA_INT_VECTAB_NUM, ERU_DMA_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套

	if		(CAMERA_GRAYSCALE == camera_type)	mt9v03x_dma();
	else if (CAMERA_BIN_UART  == camera_type)	ov7725_uart_dma();
	else if	(CAMERA_BIN       == camera_type)	ov7725_dma();
}


//串口中断函数  示例
IFX_INTERRUPT(uart0_tx_isr, UART0_INT_VECTAB_NUM, UART0_TX_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrTransmit(&uart0_handle);
}
IFX_INTERRUPT(uart0_rx_isr, UART0_INT_VECTAB_NUM, UART0_RX_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrReceive(&uart0_handle);
}
IFX_INTERRUPT(uart0_er_isr, UART0_INT_VECTAB_NUM, UART0_ER_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrError(&uart0_handle);
}

//串口1默认连接到摄像头配置串口
IFX_INTERRUPT(uart1_tx_isr, UART1_INT_VECTAB_NUM, UART1_TX_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrTransmit(&uart1_handle);
}
IFX_INTERRUPT(uart1_rx_isr, UART1_INT_VECTAB_NUM, UART1_RX_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrReceive(&uart1_handle);
    if		(CAMERA_GRAYSCALE == camera_type)	mt9v03x_uart_callback();
    else if (CAMERA_BIN_UART  == camera_type)	ov7725_uart_callback();
}
IFX_INTERRUPT(uart1_er_isr, UART1_INT_VECTAB_NUM, UART1_ER_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrError(&uart1_handle);
}


//串口2默认连接到无线转串口模块
IFX_INTERRUPT(uart2_tx_isr, UART2_INT_VECTAB_NUM, UART2_TX_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrTransmit(&uart2_handle);
}
IFX_INTERRUPT(uart2_rx_isr, UART2_INT_VECTAB_NUM, UART2_RX_INT_PRIO)
{
    enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrReceive(&uart2_handle);
    switch(wireless_type)
    {
        case WIRELESS_SI24R1:
        {
            wireless_uart_callback();
        }break;

        case WIRELESS_CH9141:
        {
            bluetooth_ch9141_uart_callback();
        }break;
        default:break;
    }
}
IFX_INTERRUPT(uart2_er_isr, UART2_INT_VECTAB_NUM, UART2_ER_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrError(&uart2_handle);
}



IFX_INTERRUPT(uart3_tx_isr, UART3_INT_VECTAB_NUM, UART3_TX_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrTransmit(&uart3_handle);
}
IFX_INTERRUPT(uart3_rx_isr, UART3_INT_VECTAB_NUM, UART3_RX_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrReceive(&uart3_handle);
}
IFX_INTERRUPT(uart3_er_isr, UART3_INT_VECTAB_NUM, UART3_ER_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrError(&uart3_handle);
}
