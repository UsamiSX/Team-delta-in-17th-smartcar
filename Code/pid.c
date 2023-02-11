#include "ccu6_pwm.h"
#include "SEEKFREE_FUN.h"
#include "motor.h"
#include "pid.h"
#include "hall.h"

brushless_highSpeed_struct brushless_highSpeed;
brushless_lowSpeed_struct brushless_lowSpeed;
pid_position_struct angleSpeed;
pid_position_struct angle;
pid_position_struct brushlessSpeed;
lqr_t lqr;
pid_position_struct motor;
pid_position_struct servo;

extern float k_roll, k_pitch;
float Encod;
float Encod1;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PI闭环计算
//  @param      read_speed  当前速度
//  @return     void
//  @since
//-------------------------------------------------------------------------------------------------------------------
int32 brushless_highSpeed_pid(int32 read_error)
{
    brushless_highSpeed.error = read_error;

    brushless_highSpeed.pout = brushless_highSpeed.error * brushless_highSpeed.kp;

    //closed_loop.pout = limit_ab(closed_loop.pout,-5,5);

    brushless_highSpeed.iout += brushless_highSpeed.error * brushless_highSpeed.ki;

    brushless_highSpeed.iout = limit_ab_f(brushless_highSpeed.iout,-130,130);

    brushless_highSpeed.dout = brushless_highSpeed.kd * (brushless_highSpeed.error - brushless_highSpeed.lastError);

    if(motor_state == MOTOR_STOP)
    {
        brushless_highSpeed.iout = 0;
    }

    brushless_highSpeed.out = brushless_highSpeed.pout + brushless_highSpeed.iout + brushless_highSpeed.dout;
    //输出限幅
    brushless_highSpeed.out = limit_ab(brushless_highSpeed.out,-brushless_highSpeed.out_max,brushless_highSpeed.out_max);
    brushless_highSpeed.lastError = brushless_highSpeed.error;
    return brushless_highSpeed.out;
}

int32 brushless_lowSpeed_pid(int32 read_error)
{
    brushless_lowSpeed.error = read_error;

    brushless_lowSpeed.pout = brushless_lowSpeed.error * brushless_lowSpeed.kp;

    //closed_loop.pout = limit_ab(closed_loop.pout,-5,5);

    brushless_lowSpeed.iout += brushless_lowSpeed.error * brushless_lowSpeed.ki;

    brushless_lowSpeed.iout = limit_ab_f(brushless_lowSpeed.iout,30,200);

    brushless_lowSpeed.dout = brushless_lowSpeed.kd * (brushless_lowSpeed.error - brushless_lowSpeed.lastError);

    if(motor_state == MOTOR_STOP)
    {
        brushless_lowSpeed.iout = 0;
    }

    brushless_lowSpeed.out = brushless_lowSpeed.pout + brushless_lowSpeed.iout + brushless_lowSpeed.dout;
    //输出限幅
    brushless_lowSpeed.out = limit_ab(brushless_lowSpeed.out,30,brushless_lowSpeed.out_max);
    brushless_lowSpeed.lastError = brushless_lowSpeed.error;
    return brushless_lowSpeed.out;
}

float bi;
float angleSpeed_pid(float read_error)
{
//    bi *= 0.7;
//    bi += 0.3*read_error;
    angleSpeed.error = read_error;

    angleSpeed.pout = (angleSpeed.error - angleSpeed.lastError) * angleSpeed.kp;

    //closed_loop.pout = limit_ab(closed_loop.pout,-5,5);

    angleSpeed.iout = angleSpeed.error * angleSpeed.ki;

    angleSpeed.iout = limit_ab_f(angleSpeed.iout,-300,300);

//    if(k_pitch+95.2 - angle.setValue >= 10 || k_pitch+95.2 - angle.setValue <= -10) angleSpeed.iout = 0;

    angleSpeed.dout = angleSpeed.kd * (angleSpeed.error - 2*angleSpeed.lastError + angleSpeed.last2Error);

    angleSpeed.out += angleSpeed.pout + angleSpeed.iout + angleSpeed.dout;
    //输出限幅
    angleSpeed.out = limit_ab(angleSpeed.out,-angleSpeed.out_max,angleSpeed.out_max);
    angleSpeed.last2Error = angleSpeed.lastError;
    angleSpeed.lastError = angleSpeed.error;
    return angleSpeed.out;
}

float ai;

float angle_pid(float read_error)
{
//    ai *= 0.7;
//    ai += 0.3*read_error;
    angle.error = read_error;

    angle.pout = (angle.error) * angle.kp;

    //closed_loop.pout = limit_ab(closed_loop.pout,-5,5);

    angle.iout += angle.error * angle.ki;

    angle.iout = limit_ab_f(angle.iout,-100,100);

    angle.dout = angle.kd * (angle.error - angle.lastError);

    angle.out = angle.pout + angle.iout + angle.dout;

    angle.out = limit_ab_f(angle.out,-angle.out_max,angle.out_max);
//    angle.out = limit_ab(angle.out,-angle.out_max,angle.out_max);
    angle.lastError = angle.error;
    return angle.out;
}

float angle_incremental_pid(float read_error)
{
    angle.error = read_error;

    angle.pout = (angle.error - angle.lastError) * angle.kp;

    //closed_loop.pout = limit_ab(closed_loop.pout,-5,5);

    angle.iout = angle.error * angle.ki;

//    angle.iout = limit_ab_f(angle.iout,-50,50);

    angle.dout = angle.kd * (angle.error - 2*angle.lastError + angle.last2Error);

    angle.out += angle.pout + angle.iout + angle.dout;

    angle.out = limit_ab_f(angle.out,-angle.out_max,angle.out_max);
//    angle.out = limit_ab(angle.out,-angle.out_max,angle.out_max);
    angle.last2Error = angle.lastError;
    angle.lastError = angle.error;
    return angle.out;
}

float angle_pid_gyro(float read_error, float gyro)
{
    angle.error = read_error;

    angle.pout = angle.error * angle.kp;

    //closed_loop.pout = limit_ab(closed_loop.pout,-5,5);

    angle.iout += angle.error * angle.ki;

    angle.iout = limit_ab_f(angle.iout,-500,500);

    if(k_pitch+95.2 - angle.setValue >= 15 || k_pitch+95.2 - angle.setValue <= -15) angle.iout = 0;

    angle.dout = angle.kd * gyro;

    angle.out = angle.pout + angle.iout + angle.dout;

    angle.out = limit_ab(angle.out,-angle.out_max,angle.out_max);

    angle.lastError = angle.error;
    return angle.out;
}

float brushlessSpeed_pid(float read_error)
{
    brushlessSpeed.error = read_error;

    brushlessSpeed.pout = brushlessSpeed.error * brushlessSpeed.kp;

    brushlessSpeed.iout += brushlessSpeed.error * brushlessSpeed.ki;

    brushlessSpeed.iout = limit_ab_f(brushlessSpeed.iout,-brushlessSpeed.out_max,brushlessSpeed.out_max); //1.67

    if(k_pitch+95.2 - angle.setValue >= 20 || k_pitch+95.2 - angle.setValue <= -20) brushlessSpeed.iout = 0;

    brushlessSpeed.dout = brushlessSpeed.kd * (brushlessSpeed.error - brushlessSpeed.lastError);

    brushlessSpeed.out = brushlessSpeed.pout + brushlessSpeed.iout + brushlessSpeed.dout;

    brushlessSpeed.out = limit_ab_f(brushlessSpeed.out,-brushlessSpeed.out_max,brushlessSpeed.out_max);

    brushlessSpeed.lastError = brushlessSpeed.error;
    return brushlessSpeed.out;
}

float brushlessSpeed_incremental_pid(float read_error)
{

//    Encod1 *= 0.7;
//
//    Encod1 += 0.3*read_error;

    brushlessSpeed.error = read_error;

//    printf("%f\n", read_error);

    brushlessSpeed.pout = (brushlessSpeed.error - brushlessSpeed.lastError) * brushlessSpeed.kp;

    brushlessSpeed.iout = brushlessSpeed.error * brushlessSpeed.ki;
//
//    brushlessSpeed.iout = limit_ab_f(brushlessSpeed.iout,-167,167); //1.67
//
//    if(k_pitch+95.2 - angle.setValue >= 10 || k_pitch+95.2 - angle.setValue <= -10) brushlessSpeed.iout = 0;
//
    brushlessSpeed.dout = brushlessSpeed.kd * (brushlessSpeed.error - 2*brushlessSpeed.lastError + brushlessSpeed.last2Error);

    brushlessSpeed.out += (brushlessSpeed.pout + brushlessSpeed.iout + brushlessSpeed.dout);
//
    brushlessSpeed.out = limit_ab_f(brushlessSpeed.out,-brushlessSpeed.out_max,brushlessSpeed.out_max);
//
    brushlessSpeed.last2Error = brushlessSpeed.lastError;

    brushlessSpeed.lastError = brushlessSpeed.error;

    return brushlessSpeed.out;
}

int32 LQR_control(int angle, float gyro, int32 speed)
{
    lqr.speed = lqr.k1*angle + lqr.k2*gyro + lqr.k3*speed;
    return lqr.speed;
}

int32 motor_pid(int32 error)
{
    Encod *=0.65;
    Encod += error * 0.35;
    motor.error = Encod;

    motor.pout = motor.error * motor.kp;

    motor.iout += motor.error * motor.ki;

    motor.iout = limit_ab_f(motor.iout,-1000,1000);

    motor.dout = motor.kd * (motor.error - motor.lastError);

    motor.out = motor.pout + motor.iout + motor.dout;
    //输出限幅
    motor.out = limit_ab(motor.out,-motor.out_max,motor.out_max);

    motor.lastError = motor.error;
    return motor.out;
}

int32 servo_pid(float error)
{
    servo.error = error;

    servo.pout = servo.error * servo.kp;

    servo.iout += servo.error * servo.ki;

    servo.iout = limit_ab(servo.iout,620,820);

    servo.dout = servo.kd * (servo.error - servo.lastError);

    servo.out = servo.pout + servo.iout + servo.dout;
    //输出限幅
    servo.out = limit_ab(servo.out, 620, 820);

    servo.lastError = servo.error;
    return servo.out;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PI闭环计算初始化
//  @param      void
//  @return     void
//  @since
//-------------------------------------------------------------------------------------------------------------------
void brushless_highSpeed_pid_init(void)
{
    brushless_highSpeed.out_max = 300;
    brushless_highSpeed.kp = 0.5;  //0.7
    brushless_highSpeed.ki = 0.01; //0.01
    brushless_highSpeed.kd = 2;    //2
    brushless_highSpeed.out = 0;
    brushless_highSpeed.real_speed = 0;
    brushless_highSpeed.lastError = 0;
    brushless_highSpeed.last2Error = 0;
    brushless_highSpeed.error = 0;
}

void brushless_lowSpeed_pid_init(void)
{
    brushless_lowSpeed.out_max = 300;
    brushless_lowSpeed.kp = 0.5;  //0.7
    brushless_lowSpeed.ki = 0.01; //0.01
    brushless_lowSpeed.kd = 0.04;    //2
    brushless_lowSpeed.out = 0;
    brushless_lowSpeed.real_speed = 0;
    brushless_lowSpeed.lastError = 0;
    brushless_lowSpeed.last2Error = 0;
    brushless_lowSpeed.error = 0;
}

void angleSpeed_pid_init(void)
{
    angleSpeed.out_max = 500;//200
    angleSpeed.kp = 25;     //55 110 80 15.5 120 60 50 55  30 25
    angleSpeed.ki = 3;   //4; 3      //6 8
    angleSpeed.kd = 8;      //15  0.1 0.6 3 5 2
    angleSpeed.out = 0;
    angleSpeed.realValue = 0;
    angleSpeed.lastError = 0;
    angleSpeed.error = 0;
    angleSpeed.setValue = 0;
}

void angle_pid_init(void)
{
    angle.out_max = 500;
    angle.kp = 1.5;//5*0.6;//100;//3; //2.5   2 //5.6 210 108 180 2 1.52   1.47
    angle.ki = 0;
    angle.kd = 0.01;//1*0.6;//20; //2 135 78 80
    angle.out = 0;
    angle.realValue = 0;
    angle.lastError = 0;
    angle.error = 0;
    angle.setValue = 0;
}

//void brushlessSpeed_pid_init(void)
//{
//    brushlessSpeed.out_max = 200;
//    brushlessSpeed.kp = 0.085;//0.0000085;   //0.0002 0.0007985 0.008
//    brushlessSpeed.ki = 0.008;//brushlessSpeed.kp/200; //brushlessSpeed.kp/200
//    brushlessSpeed.kd = 0.5;//0.004 0.05
//    brushlessSpeed.out = 0;
//    brushlessSpeed.realValue = 0;
//    brushlessSpeed.lastError = 0;
//    brushlessSpeed.error = 0;
//    brushlessSpeed.setValue = 0;
//}
//0.0001
void brushlessSpeed_pid_init(void) //19 0.02 0.1
{
    brushlessSpeed.out_max = 20000;
    brushlessSpeed.kp = 45;//2.5;//50;//0.8;//0.0000085;   //0.0002 0.0007985 0.008 0.085 0.56
    brushlessSpeed.ki = 0.001;
    brushlessSpeed.kd = 0.10;//1;//0.004 0.5
    brushlessSpeed.out = 0;
    brushlessSpeed.realValue = 0;
    brushlessSpeed.lastError = 0;
    brushlessSpeed.error = 0;
    brushlessSpeed.setValue = 0;
}

void lqr_init(void)
{
    lqr.k1 = 250;  //1.6
    lqr.k2 = 0;  //10
    lqr.k3 = 0; //1
    lqr.speed = 0;
}

void motor_pid_init(void)
{
    Encod = 0;
    motor.out_max = 2500;
    motor.kp = 1;
    motor.ki = 0.08;
    motor.kd = 0;
}

void servo_pid_init(void) // 820 720 620
{
    servo.kp = 1;
    servo.ki = 0;
    servo.kd = 0;
}

float DiffRSum(uint16 data1, uint16 data2)
{
    float result;
    result = (sqrt(data1) - sqrt(data2)) / data1 + data2;
    return result;
}


