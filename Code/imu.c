#include "imu.h"
#include "headfile.h"
#include "math.h"
#include "common.h"

#define delta_T      0.01f  //25ms计算一次
#define dt 0.01f
#define M_PI 3.1425f

float I_ex, I_ey, I_ez;  // 误差积分

extern short gyroX,gyroY,gyroZ;
extern short accX,accY,accZ;

quaterInfo_t Q_info = {1, 0, 0, 0};  // 全局四元数
eulerianAngles_t eulerAngle; //欧拉角
icm_t icm;

float param_Kp = 50;   // 加速度计(磁力计)的收敛速率比例增益50
float param_Ki = 0.2;   //陀螺仪收敛速率的积分增益 0.2

//-------------------------------------------------------------------------------------------------------------------
//  @brief      invSqrt函数
//  @param      void
//  @param      void
//  @return     void
//  Sample usage:               求解四元数相关中的norm 无需调用 嵌套在其他函数里
//-------------------------------------------------------------------------------------------------------------------
float invSqrt(float x) {
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;
    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      将采集的数值转化为实际物理值, 并对陀螺仪进行去零漂处理
//  @param      void
//  @param      void
//  @return     void
//  Sample usage:               中断 直接调用
//-------------------------------------------------------------------------------------------------------------------

void IMU_getValues()
{
    float alpha = 0.3;

    icm.accX = ((float)accX) * alpha / 4096 + icm.accX * (1-alpha); // 12bits
    icm.accY = ((float)accY) * alpha / 4096 + icm.accY * (1-alpha);
    icm.accZ = ((float)accZ) * alpha / 4096 + icm.accZ * (1-alpha);

    icm.gyroX = ((float)gyroX) / 16.4f * alpha + icm.gyroX * (1-alpha);// LSB 16.4 16bits
    icm.gyroY = ((float)gyroY) / 16.4f * alpha + icm.gyroY * (1-alpha);
    icm.gyroZ = ((float)gyroZ) / 16.4f * alpha + icm.gyroZ * (1-alpha);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      四元数处理
//  @param      void
//  @param      void
//  @return     void
//  Sample usage:   中断直接调用
//-------------------------------------------------------------------------------------------------------------------
static void IMU_AHRSupdate_noMagnetic(float gx, float gy, float gz, float ax, float ay, float az)
{
    float halfT = 0.5 * delta_T;
    float vx, vy, vz;    //当前的机体坐标系上的重力单位向量
    float ex, ey, ez;    //四元数计算值与加速度计测量值的误差
    float q0 = Q_info.q0;
    float q1 = Q_info.q1;
    float q2 = Q_info.q2;
    float q3 = Q_info.q3;
    float q0q0 = q0 * q0;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;
    // float delta_2 = 0;

    //对加速度数据进行归一化 得到单位加速度
    float norm = invSqrt(ax*ax + ay*ay + az*az);
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;
    vx = 2*(q1q3 - q0q2);
    vy = 2*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    ex = ay * vz - az * vy;
    ey = az * vx - ax * vz;
    ez = ax * vy - ay * vx;

    //用叉乘误差来做PI修正陀螺零偏，
    //通过调节 param_Kp，param_Ki 两个参数，
    //可以控制加速度计修正陀螺仪积分姿态的速度。
    I_ex += delta_T * ex;   // integral error scaled by Ki
    I_ey += delta_T * ey;
    I_ez += delta_T * ez;

    gx = gx+ param_Kp*ex + param_Ki*I_ex;
    gy = gy+ param_Kp*ey + param_Ki*I_ey;
    gz = gz+ param_Kp*ez + param_Ki*I_ez;


    /*数据修正完成，下面是四元数微分方程*/


    //四元数微分方程，其中halfT为测量周期的1/2，gx gy gz为陀螺仪角速度，以下都是已知量，这里使用了一阶龙哥库塔求解四元数微分方程
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    q1 = q1 + ( q0*gx + q2*gz - q3*gy)*halfT;
    q2 = q2 + ( q0*gy - q1*gz + q3*gx)*halfT;
    q3 = q3 + ( q0*gz + q1*gy - q2*gx)*halfT;
    //    delta_2=(2*halfT*gx)*(2*halfT*gx)+(2*halfT*gy)*(2*halfT*gy)+(2*halfT*gz)*(2*halfT*gz);
    // 整合四元数率    四元数微分方程  四元数更新算法，二阶毕卡法
    //    q0 = (1-delta_2/8)*q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    //    q1 = (1-delta_2/8)*q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    //    q2 = (1-delta_2/8)*q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    //    q3 = (1-delta_2/8)*q3 + (q0*gz + q1*gy - q2*gx)*halfT;
    // normalise quaternion
    norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    Q_info.q0 = q0 * norm;
    Q_info.q1 = q1 * norm;
    Q_info.q2 = q2 * norm;
    Q_info.q3 = q3 * norm;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      四元数转化为欧拉角
//  @param      void
//  @param      void
//  @return     void
//  Sample usage: 中断直接调用
//-------------------------------------------------------------------------------------------------------------------
//void IMU_quaterToEulerianAngles(void)
//{
//    IMU_getValues(values);
//    IMU_AHRSupdate_noMagnetic(values[3], values[4], values[5], values[0], values[1], values[2]);
//    float q0 = Q_info.q0;
//    float q1 = Q_info.q1;
//    float q2 = Q_info.q2;
//    float q3 = Q_info.q3;
//    eulerAngle.pitch = asin(-2*q1*q3 + 2*q0*q2) * 180/M_PI; // pitch
//    eulerAngle.roll = atan2(2*q2*q3 + 2*q0*q1, -2*q1*q1 - 2*q2*q2 + 1) * 180/M_PI; // roll
//    eulerAngle.yaw = atan2(2*q1*q2 + 2*q0*q3, -2*q2*q2 - 2*q3*q3 + 1) * 180/M_PI; // yaw
//
///*   可以不用作姿态限度的限制*/
//    if(eulerAngle.roll>90 || eulerAngle.roll<-90)
//    {
//        if(eulerAngle.pitch > 0)
//        {
//            eulerAngle.pitch = 168-eulerAngle.pitch;//180
//        }
//        if(eulerAngle.pitch < 0)
//        {
//            eulerAngle.pitch = -(168+eulerAngle.pitch);//180
//        }
//    }
//    if(eulerAngle.yaw > 180)
//    {
//        eulerAngle.yaw -=360;
//    }
//    else if(eulerAngle.yaw <-180)
//    {
//        eulerAngle.yaw +=360;
//    }
//}


float ax_offset = 0, ay_offset = 0;         //x,y轴的加速度偏移量
float gx_offset = 0, gy_offset = 0;         //x,y轴的角速度偏移量

/*参数*/
float rad2deg = 57.29578f;                  //弧度到角度的换算系数
float roll_v = 0, pitch_v = 0;              //换算到x,y轴上的角速度

/*三个状态，先验状态，观测状态，最优估计状态*/
float gyro_roll = 0, gyro_pitch = 0;        //陀螺仪积分计算出的角度，先验状态
float acc_roll = 0, acc_pitch = 0;          //加速度计观测出的角度，观测状态
float k_roll = 0, k_pitch = 0;              //卡尔曼滤波后估计出最优角度，最优估计状态

/*误差协方差矩阵P*/
float e_P[2][2] ={{1,0},{0,1}};             //误差协方差矩阵，这里的e_P既是先验估计的P，也是最后更新的P

/*卡尔曼增益K*/
float k_k[2][2] ={{0,0},{0,0}};             //这里的卡尔曼增益矩阵K是一个2X2的方阵


void Kalman_Filter()
{
    /*获取角速度和加速度 */
    get_icm42605_accdata_spi();
    get_icm42605_gyro_spi();

    IMU_getValues();

    /*step1:计算先验状态*/
    /*计算x,y轴上的角速度*/
    roll_v = (icm.gyroX-gx_offset) + ((sin(k_pitch)*sin(k_roll))/cos(k_pitch))*(icm.gyroY-gy_offset) + ((sin(k_pitch)*cos(k_roll))/cos(k_pitch))*icm.gyroZ;//roll轴的角速度
    pitch_v = cos(k_roll)*(icm.gyroY-gy_offset) - sin(k_roll)*icm.gyroZ;//pitch轴的角速度
    gyro_roll = k_roll + dt*roll_v;//先验roll角度
    gyro_pitch = k_pitch + dt*pitch_v;//先验pitch角度

    /*step2:计算先验误差协方差矩阵P*/
    e_P[0][0] = e_P[0][0] + 0.0025;//这里的Q矩阵是一个对角阵且对角元均为0.0025
    e_P[0][1] = e_P[0][1] + 0;
    e_P[1][0] = e_P[1][0] + 0;
    e_P[1][1] = e_P[1][1] + 0.0025; //系统过程预测的误差Q 0.0025

    /*step3:更新卡尔曼增益K*/
    k_k[0][0] = e_P[0][0]/(e_P[0][0]+0.3); //系统测量的误差R 0.3
    k_k[0][1] = 0;
    k_k[1][0] = 0;
    k_k[1][1] = e_P[1][1]/(e_P[1][1]+0.3);

    /*step4:计算最优估计状态*/
    /*观测状态*/
    //roll角度
    acc_roll = atan((icm.accY - ay_offset) / (icm.accZ))*rad2deg;
    //pitch角度
    acc_pitch = rad2deg * atan2(icm.accX, -icm.accZ);
    /*最优估计状态*/
    k_roll = gyro_roll + k_k[0][0]*(acc_roll - gyro_roll); //数据融合
    k_pitch = gyro_pitch + k_k[1][1]*(acc_pitch - gyro_pitch);

    /*step5:更新协方差矩阵P*/
    e_P[0][0] = (1 - k_k[0][0])*e_P[0][0];
    e_P[0][1] = 0;
    e_P[1][0] = 0;
    e_P[1][1] = (1 - k_k[1][1])*e_P[1][1];
}

float K1 = 0.1;
float angleXlast;

float ComplementaryFilter(float angle_m, float gyro_m)
{
    float angle;

    angle = K1 * angle_m + (1-K1) * (angleXlast + gyro_m * dt);
    angleXlast = angle;
    return angle;
}


void imu_offset()
{
    for (int i = 1; i <= 100; i++) {
        get_icm42605_accdata_spi();
        get_icm42605_gyro_spi();
        IMU_getValues();
        ax_offset = ax_offset + icm.accX;//计算x轴加速度的偏移总量
        ay_offset = ay_offset + icm.accY;//计算y轴加速度的偏移总量
        gx_offset = gx_offset + icm.gyroX;
        gy_offset = gy_offset + icm.gyroY;
        systick_delay_ms(STM0, 10);
    }
    ax_offset = ax_offset / 100; //计算x轴加速度的偏移量
    ay_offset = ay_offset / 100; //计算y轴加速度的偏移量
    gx_offset = gx_offset / 100; //计算x轴角速度的偏移量
    gy_offset = gy_offset / 100; //计算y轴角速度的偏移量
}

