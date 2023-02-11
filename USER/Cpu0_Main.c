#include "Cpu0_Main.h"
#include "headfile.h"
#pragma section all "cpu0_dsram"

#define BUZZER P00_12

QFsm me;

//状态机事件的结构体数组
QEvent QEP_reservedEvt_[] =
{
    {(QState)Q_ENTRY_SIG, 0},
    {(QState)Q_EXIT_SIG, 0},
    {(QState)Q_INIT_SIG, 0},
    {(QState)Btn0Prsd, 0},
    {(QState)Btn1Prsd, 0},
    {(QState)Btn2Prsd, 0}
};

extern int numRotary;
extern int numRotary_n0;
extern int numRotary_n_0;
extern int numRotary_n1;
extern int numRotary_n_1;
extern uint8 signParmDisplay;
extern uint8 signImgDisplay;
extern uint8 signParmSetting;
extern float Battery_voltage;
extern int16 motorSpeed;
extern uint16 buffer[8];
extern float angleT;
extern float gyro;
extern float k_roll, k_pitch;
extern float angle_zero;

uint8 value = 0;
int numRotaryLast = 0;
int a = 0, b = 0;

extern uint16 ipts0[MT9V03X_H][2];
extern uint16 ipts1[MT9V03X_H][2];
extern uint16 ipts0_num, ipts1_num;
// 变换后左右边线+滤波
extern float rpts0b[MT9V03X_H][2];
extern float rpts1b[MT9V03X_H][2];
extern int rpts0b_num, rpts1b_num;
// 变换后左右边线+等距采样
extern float rpts0s[MT9V03X_H][2];
extern float rpts1s[MT9V03X_H][2];
extern int rpts0s_num, rpts1s_num;
// 左右边线局部角度变化率
extern float rpts0a[MT9V03X_H];
extern float rpts1a[MT9V03X_H];
extern int rpts0a_num, rpts1a_num;
// 左右边线局部角度变化率+非极大抑制
extern float rpts0an[MT9V03X_H];
extern float rpts1an[MT9V03X_H];



int core0_main(void)
{
	get_clk();
	seekfree_wireless_init();
//
	icm42605_init();
//
	systick_delay_ms(STM0, 500);
	imu_offset();
//
//	AD5124_init(0x4D, 0x65, 0x66, 0x48); //7位，0-128 最低位是x (无关位) L2, L1, L4 ,L3
//
//	initEVADC();
//	init_DMA();
//	adc_init(ADC_0, ADC0_CH1_A1);
//
//	//串级pid
	brushlessSpeed_pid_init();
	angle_pid_init();
    angleSpeed_pid_init();
//
//	//无刷电机初始化
    hall_init();
    move_filter_init(&speed_filter);
    brushless_motor_init();
    ccu6_pwm_init();
////
////
////    //有刷电机初始化
//    motor_init(17000, 0);
//    gpio_init(P33_1, GPO, 1, PUSHPULL); //电机方向引脚初始化
//    motor_pid_init();
////
////    //舵机初始化
    servo_init(50, 720); //左820-720-620右
    servo_pid_init();
////
////	lcd_init();
////	SettingPage();
//
//	mt9v03x_init();
////
////	//旋转编码器
//	gpt12_init(GPT12_T6, GPT12_T6INA_P20_3, GPT12_T6EUDA_P20_0);
//	gpt12_init(GPT12_T5, GPT12_T5INB_P10_3, GPT12_T5EUDB_P10_1);
//
////	gpio_init(BUZZER, GPO, 0, PUSHPULL);
//
//
//	gpio_init(P21_2, GPO, 0, PUSHPULL);
//	gpio_init(P21_3, GPO, 0, PUSHPULL);
//	gpio_init(P21_4, GPO, 0, PUSHPULL);

//
//	//状态机初始化
//    QFsm_ctor(&me, ParmSetting);
//    QFsm_init(&me, &QEP_reservedEvt_[Q_ENTRY_SIG]);



    pit_interrupt_ms(CCU6_0, PIT_CH0, 2);
    pit_interrupt_us(CCU6_0, PIT_CH1, 50);


	IfxCpu_emitEvent(&g_cpuSyncEvent);
	IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);

//    initGtmAtom();

	enableInterrupts();

	while (TRUE)
	{
	    printf("%d, %f, %f\n", brushlessDuty, (k_pitch + angle_zero), gyro);

//	    printf("init");

//	    printf("init");
//	    printf("%d, %d, %d, %f, %f, %f, %f\n", speed_filter.data_average, motor_control.set_speed, brushlessDuty, (k_pitch + angle_zero), gyro, angle.setValue, angleSpeed.setValue);
//	    printf("init");
//
//	    if(gpio_get(P23_3) == 0) {
//            QFsm_dispatch(&me,&QEP_reservedEvt_[Btn0Prsd]);
//            while(gpio_get(P23_3) == 0);
//        }
//        if(gpio_get(P23_4) == 0) {
//            QFsm_dispatch(&me,&QEP_reservedEvt_[Btn1Prsd]);
//            while(gpio_get(P23_4) == 0);
//        }
//        if(gpio_get(P23_5) == 0) {
//            QFsm_dispatch(&me,&QEP_reservedEvt_[Btn2Prsd]);
//            while(gpio_get(P23_5) == 0);
//        }
//
//	    if(signParmSetting == TRUE) {
//            numRotary = gpt12_get(GPT12_T5);
//
//            if(gpio_get(P10_0) == 0) {
//                if((numRotary-numRotaryLast) == 1) {
//                    numRotary_n0++;
//                    if(value == 0) value0 += 1;
//                    else if(value == 1) value1 += 0.1;
//                    else if(value == 2) value2 += 0.5;
//                    else if(value == 3) value3 += 0.1;
//                    else if(value == 4) value4 += 1;
//                    else if(value == 5) value5 += 0.001;
//                    else if(value == 6) value6 += 0.001;
//                }
//                if((numRotary-numRotaryLast) == -1) {
//                    numRotary_n_0++;
//                    if(value == 0) value0 -= 1;
//                    else if(value == 1) value1 -= 0.1;
//                    else if(value == 2) value2 -= 0.5;
//                    else if(value == 3) value3 -= 0.1;
//                    else if(value == 4) value4 -= 1;
//                    else if(value == 5) value5 -= 0.001;
//                    else if(value == 6) value6 -= 0.001;
//                }
//                lcd_showfloat(9 * 8,0,value0,2,2);
//                lcd_showfloat(9 * 8,1,value1,1,3);
//                lcd_showfloat(9 * 8,2,value2,2,2);
//                lcd_showfloat(9 * 8,3,value3,2,2);
//                lcd_showfloat(9 * 8,4,value4,2,2);
//                lcd_showfloat(9 * 8,5,value5,1,3);
//                lcd_showfloat(9 * 8,6,value6,1,3);
//            }
//            if(gpio_get(P10_0) == 1) {
//                if((numRotary-numRotaryLast) == 1) {
//                    numRotary_n1++;
//                    if(numRotary_n1 % 7 == 0) {
//                        value = 0;
//                        lcd_triangle(8, 8);
//                        lcd_triangle_b(8, 24);
//                        lcd_triangle_b(8, 40);
//                        lcd_triangle_b(8, 56);
//                        lcd_triangle_b(8, 72);
//                        lcd_triangle_b(8, 88);
//                        lcd_triangle_b(8, 104);
//                    }
//                    if(numRotary_n1 % 7 == 1) {
//                        value = 1;
//                        lcd_triangle_b(8, 8);
//                        lcd_triangle(8, 24);
//                        lcd_triangle_b(8, 40);
//                        lcd_triangle_b(8, 56);
//                        lcd_triangle_b(8, 72);
//                        lcd_triangle_b(8, 88);
//                        lcd_triangle_b(8, 104);
//                    }
//                    if(numRotary_n1 % 7 == 2) {
//                        value = 2;
//                        lcd_triangle_b(8, 8);
//                        lcd_triangle_b(8, 24);
//                        lcd_triangle(8, 40);
//                        lcd_triangle_b(8, 56);
//                        lcd_triangle_b(8, 72);
//                        lcd_triangle_b(8, 88);
//                        lcd_triangle_b(8, 104);
//                    }
//                    if(numRotary_n1 % 7 == 3) {
//                        value = 3;
//                        lcd_triangle_b(8, 8);
//                        lcd_triangle_b(8, 24);
//                        lcd_triangle_b(8, 40);
//                        lcd_triangle(8, 56);
//                        lcd_triangle_b(8, 72);
//                        lcd_triangle_b(8, 88);
//                        lcd_triangle_b(8, 104);
//
//                    }
//                    if(numRotary_n1 % 7 == 4) {
//                        value = 4;
//                        lcd_triangle_b(8, 8);
//                        lcd_triangle_b(8, 24);
//                        lcd_triangle_b(8, 40);
//                        lcd_triangle_b(8, 56);
//                        lcd_triangle(8, 72);
//                        lcd_triangle_b(8, 88);
//                        lcd_triangle_b(8, 104);
//                    }
//                    if(numRotary_n1 % 7 == 5) {
//                        value = 5;
//                        lcd_triangle_b(8, 8);
//                        lcd_triangle_b(8, 24);
//                        lcd_triangle_b(8, 40);
//                        lcd_triangle_b(8, 56);
//                        lcd_triangle_b(8, 72);
//                        lcd_triangle(8, 88);
//                        lcd_triangle_b(8, 104);
//                    }
//                    if(numRotary_n1 % 7 == 6) {
//                        value = 6;
//                        lcd_triangle_b(8, 8);
//                        lcd_triangle_b(8, 24);
//                        lcd_triangle_b(8, 40);
//                        lcd_triangle_b(8, 56);
//                        lcd_triangle_b(8, 72);
//                        lcd_triangle_b(8, 88);
//                        lcd_triangle(8, 104);
//                    }
//                    lcd_showfloat(9 * 8,0,value0,2,2);
//                    lcd_showfloat(9 * 8,1,value1,1,3);
//                    lcd_showfloat(9 * 8,2,value2,2,2);
//                    lcd_showfloat(9 * 8,3,value3,2,2);
//                    lcd_showfloat(9 * 8,4,value4,2,2);
//                    lcd_showfloat(9 * 8,5,value5,1,3);
//                    lcd_showfloat(9 * 8,6,value6,1,3);
//                }
//                if((numRotaryLast - numRotary) == 1) {
//                    numRotary_n1--;
//                    if(numRotary_n1 % 7 == 0) {
//                        value = 0;
//                        lcd_triangle(8, 8);
//                        lcd_triangle_b(8, 24);
//                        lcd_triangle_b(8, 40);
//                        lcd_triangle_b(8, 56);
//                        lcd_triangle_b(8, 72);
//                        lcd_triangle_b(8, 88);
//                        lcd_triangle_b(8, 104);
//                    }
//                    if(numRotary_n1 % 7 == 1) {
//                        value = 1;
//                        lcd_triangle_b(8, 8);
//                        lcd_triangle(8, 24);
//                        lcd_triangle_b(8, 40);
//                        lcd_triangle_b(8, 56);
//                        lcd_triangle_b(8, 72);
//                        lcd_triangle_b(8, 88);
//                        lcd_triangle_b(8, 104);
//                    }
//                    if(numRotary_n1 % 7 == 2) {
//                        value = 2;
//                        lcd_triangle_b(8, 8);
//                        lcd_triangle_b(8, 24);
//                        lcd_triangle(8, 40);
//                        lcd_triangle_b(8, 56);
//                        lcd_triangle_b(8, 72);
//                        lcd_triangle_b(8, 88);
//                        lcd_triangle_b(8, 104);
//                    }
//                    if(numRotary_n1 % 7 == 3) {
//                        value = 3;
//                        lcd_triangle_b(8, 8);
//                        lcd_triangle_b(8, 24);
//                        lcd_triangle_b(8, 40);
//                        lcd_triangle(8, 56);
//                        lcd_triangle_b(8, 72);
//                        lcd_triangle_b(8, 88);
//                        lcd_triangle_b(8, 104);
//
//                    }
//                    if(numRotary_n1 % 7 == 4) {
//                        value = 4;
//                        lcd_triangle_b(8, 8);
//                        lcd_triangle_b(8, 24);
//                        lcd_triangle_b(8, 40);
//                        lcd_triangle_b(8, 56);
//                        lcd_triangle(8, 72);
//                        lcd_triangle_b(8, 88);
//                        lcd_triangle_b(8, 104);
//                    }
//                    if(numRotary_n1 % 7 == 5) {
//                        value = 5;
//                        lcd_triangle_b(8, 8);
//                        lcd_triangle_b(8, 24);
//                        lcd_triangle_b(8, 40);
//                        lcd_triangle_b(8, 56);
//                        lcd_triangle_b(8, 72);
//                        lcd_triangle(8, 88);
//                        lcd_triangle_b(8, 104);
//                    }
//                    if(numRotary_n1 % 7 == 6) {
//                        value = 6;
//                        lcd_triangle_b(8, 8);
//                        lcd_triangle_b(8, 24);
//                        lcd_triangle_b(8, 40);
//                        lcd_triangle_b(8, 56);
//                        lcd_triangle_b(8, 72);
//                        lcd_triangle_b(8, 88);
//                        lcd_triangle(8, 104);
//                    }
//                    lcd_showfloat(9 * 8,0,value0,2,2);
//                    lcd_showfloat(9 * 8,1,value1,1,3);
//                    lcd_showfloat(9 * 8,2,value2,2,2);
//                    lcd_showfloat(9 * 8,3,value3,2,2);
//                    lcd_showfloat(9 * 8,4,value4,2,2);
//                    lcd_showfloat(9 * 8,5,value5,1,3);
//                    lcd_showfloat(9 * 8,6,value6,1,3);
//                }
//            }
//            numRotaryLast = numRotary;
//            systick_delay_ms(STM0, 10);
//	    }
//	    if(signParmDisplay == TRUE) {
//
//            lcd_showfloat(9 * 8,0,Battery_voltage,2,2);
//            lcd_showfloat(9 * 8,1,motorSpeed,1,3);
//            lcd_showfloat(9 * 8,2,angleT,2,2);
//            lcd_showfloat(9 * 8,3,buffer[4],1,3);
//            lcd_showfloat(9 * 8,4,buffer[0],2,2);
//            lcd_showfloat(9 * 8,5,buffer[1],1,3);
//            lcd_showfloat(9 * 8,6,buffer[2],1,3);
//	    }
//
//	    if(signImgDisplay == TRUE) {
//	        if(mt9v03x_finish_flag) {
//                process_image();
//                lcd_displayimage032_zoom(mt9v03x_image, MT9V03X_W, MT9V03X_H, MT9V03X_W/2 + 1, MT9V03X_H/2 - 1);
//                SendLine();
//                for(int i = 0; i < ipts0_num; i++)
//                {
//                    lcd_drawpoint(rpts0b[i][0]/2, rpts0b[i][1]/2, GREEN);
//                }
//    //                printf("%d, %d",rpts0b[i][0],rpts0b[i][1]); //打印左边界点测试梯形畸变由单线计算中线
//                for(int i = 0; i < ipts1_num; i++)
//                {
//                    lcd_drawpoint(rpts1b[i][0]/2, rpts1b[i][1]/2, YELLOW);
//                }
//    //                printf("%d, %d",rpts1b[i][0],rpts1b[i][1],rpts1b[i][1]-MT9V03X_W/2); //打印右边界点测试梯形畸变由单线计算中线
//                //绘制角点
//                for(int i = 0; i < MT9V03X_H; i++)
//                {
//                    //printf("%.2f ", rpts0an[i]); printf("%.2f ", rpts1an[i]);  //打印角度变化率
//                    if(fabs(rpts0an[i]) > 0.8)
//                    {
//                        IPS_Drawcross(rpts0b[i][0]/2, rpts0b[i][1]/2, WHITE);
//    //                    num0 = i;
//                    }
//                    if(fabs(rpts1an[i]) > 0.8)
//                    {
//                        IPS_Drawcross(rpts1b[i][0]/2, rpts1b[i][1]/2, WHITE);
//    //                    num1 = i;
//                    }
//                }
//
//                for(int i = 0; i < ipts0_num; i++)
//                {
//                    lcd_drawpoint(rpts0b[i][0]/2, rpts0b[i][1]/2, GREEN);
//                }
//    //                printf("%d, %d",rpts0b[i][0],rpts0b[i][1]); //打印左边界点测试梯形畸变由单线计算中线
//                for(int i = 0; i < ipts1_num; i++)
//                {
//                    lcd_drawpoint(rpts1b[i][0]/2, rpts1b[i][1]/2, YELLOW);
//                }
//    //                printf("%d, %d",rpts1b[i][0],rpts1b[i][1],rpts1b[i][1]-MT9V03X_W/2); //打印右边界点测试梯形畸变由单线计算中线
//                //绘制角点
//                for(int i = 0; i < MT9V03X_H; i++)
//                {
//                    //printf("%.2f ", rpts0an[i]); printf("%.2f ", rpts1an[i]);  //打印角度变化率
//                    if(fabs(rpts0an[i]) > 1.5)
//                    {
//                        IPS_Drawcross((int)rpts0b[i][0]/2, (int)rpts0b[i][1]/2, WHITE);
//    //                    printf("%f\n", fabs(rpts0an[i]));
//    //                    num0 = i;
//                    }
//                    if(fabs(rpts1an[i]) > 1.5)
//                    {
//                        IPS_Drawcross((int)rpts1b[i][0]/2, (int)rpts1b[i][1]/2, WHITE);
//    //                    printf("%f\n", fabs(rpts1an[i]));
//    //                    num1 = i;
//                    }
//                }
//                mt9v03x_finish_flag = 0;
//	            }
//            }

//        if(mt9v03x_finish_flag)
//        {
//            process_image();
//            lcd_displayimage032_zoom(mt9v03x_image, MT9V03X_W, MT9V03X_H, MT9V03X_W/2 + 1, MT9V03X_H/2 - 1);
////            SendLine();
////            for(int i = 0; i < ipts0_num; i++)
////            {
////                lcd_drawpoint(rpts0b[i][0]/2, rpts0b[i][1]/2, GREEN);
////            }
//////                printf("%d, %d",rpts0b[i][0],rpts0b[i][1]); //打印左边界点测试梯形畸变由单线计算中线
////            for(int i = 0; i < ipts1_num; i++)
////            {
////                lcd_drawpoint(rpts1b[i][0]/2, rpts1b[i][1]/2, YELLOW);
////            }
//////                printf("%d, %d",rpts1b[i][0],rpts1b[i][1],rpts1b[i][1]-MT9V03X_W/2); //打印右边界点测试梯形畸变由单线计算中线
////            //绘制角点
////            for(int i = 0; i < MT9V03X_H; i++)
////            {
////                //printf("%.2f ", rpts0an[i]); printf("%.2f ", rpts1an[i]);  //打印角度变化率
////                if(fabs(rpts0an[i]) > 0.8)
////                {
////                    IPS_Drawcross(rpts0b[i][0]/2, rpts0b[i][1]/2, WHITE);
//////                    num0 = i;
////                }
////                if(fabs(rpts1an[i]) > 0.8)
////                {
////                    IPS_Drawcross(rpts1b[i][0]/2, rpts1b[i][1]/2, WHITE);
//////                    num1 = i;
////                }
////            }
//
//            for(int i = 0; i < ipts0_num; i++)
//            {
//                lcd_drawpoint(rpts0b[i][0]/2, rpts0b[i][1]/2, GREEN);
//            }
////                printf("%d, %d",rpts0b[i][0],rpts0b[i][1]); //打印左边界点测试梯形畸变由单线计算中线
//            for(int i = 0; i < ipts1_num; i++)
//            {
//                lcd_drawpoint(rpts1b[i][0]/2, rpts1b[i][1]/2, YELLOW);
//            }
////                printf("%d, %d",rpts1b[i][0],rpts1b[i][1],rpts1b[i][1]-MT9V03X_W/2); //打印右边界点测试梯形畸变由单线计算中线
//            //绘制角点
//            for(int i = 0; i < MT9V03X_H; i++)
//            {
//                //printf("%.2f ", rpts0an[i]); printf("%.2f ", rpts1an[i]);  //打印角度变化率
//                if(fabs(rpts0an[i]) > 1.5)
//                {
//                    IPS_Drawcross((int)rpts0b[i][0]/2, (int)rpts0b[i][1]/2, WHITE);
////                    printf("init");
////                    printf("%f\n", fabs(rpts0an[i]));
//
////                    num0 = i;
//                }
//                if(fabs(rpts1an[i]) > 1.5)
//                {
//                    IPS_Drawcross((int)rpts1b[i][0]/2, (int)rpts1b[i][1]/2, WHITE);
////                    printf("%f\n", fabs(rpts1an[i]));
////                    num1 = i;
//                }
//            }
//            mt9v03x_finish_flag = 0;
//        }

	}
}

#pragma section all restore
