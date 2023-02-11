#include "imu.h"
#include "headfile.h"
#include "math.h"
#include "common.h"

#define delta_T      0.01f  //25ms����һ��
#define dt 0.01f
#define M_PI 3.1425f

float I_ex, I_ey, I_ez;  // ������

extern short gyroX,gyroY,gyroZ;
extern short accX,accY,accZ;

quaterInfo_t Q_info = {1, 0, 0, 0};  // ȫ����Ԫ��
eulerianAngles_t eulerAngle; //ŷ����
icm_t icm;

float param_Kp = 50;   // ���ٶȼ�(������)���������ʱ�������50
float param_Ki = 0.2;   //�������������ʵĻ������� 0.2

//-------------------------------------------------------------------------------------------------------------------
//  @brief      invSqrt����
//  @param      void
//  @param      void
//  @return     void
//  Sample usage:               �����Ԫ������е�norm ������� Ƕ��������������
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
//  @brief      ���ɼ�����ֵת��Ϊʵ������ֵ, ���������ǽ���ȥ��Ư����
//  @param      void
//  @param      void
//  @return     void
//  Sample usage:               �ж� ֱ�ӵ���
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
//  @brief      ��Ԫ������
//  @param      void
//  @param      void
//  @return     void
//  Sample usage:   �ж�ֱ�ӵ���
//-------------------------------------------------------------------------------------------------------------------
static void IMU_AHRSupdate_noMagnetic(float gx, float gy, float gz, float ax, float ay, float az)
{
    float halfT = 0.5 * delta_T;
    float vx, vy, vz;    //��ǰ�Ļ�������ϵ�ϵ�������λ����
    float ex, ey, ez;    //��Ԫ������ֵ����ٶȼƲ���ֵ�����
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

    //�Լ��ٶ����ݽ��й�һ�� �õ���λ���ٶ�
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

    //�ò���������PI����������ƫ��
    //ͨ������ param_Kp��param_Ki ����������
    //���Կ��Ƽ��ٶȼ����������ǻ�����̬���ٶȡ�
    I_ex += delta_T * ex;   // integral error scaled by Ki
    I_ey += delta_T * ey;
    I_ez += delta_T * ez;

    gx = gx+ param_Kp*ex + param_Ki*I_ex;
    gy = gy+ param_Kp*ey + param_Ki*I_ey;
    gz = gz+ param_Kp*ez + param_Ki*I_ez;


    /*����������ɣ���������Ԫ��΢�ַ���*/


    //��Ԫ��΢�ַ��̣�����halfTΪ�������ڵ�1/2��gx gy gzΪ�����ǽ��ٶȣ����¶�����֪��������ʹ����һ��������������Ԫ��΢�ַ���
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    q1 = q1 + ( q0*gx + q2*gz - q3*gy)*halfT;
    q2 = q2 + ( q0*gy - q1*gz + q3*gx)*halfT;
    q3 = q3 + ( q0*gz + q1*gy - q2*gx)*halfT;
    //    delta_2=(2*halfT*gx)*(2*halfT*gx)+(2*halfT*gy)*(2*halfT*gy)+(2*halfT*gz)*(2*halfT*gz);
    // ������Ԫ����    ��Ԫ��΢�ַ���  ��Ԫ�������㷨�����ױϿ���
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
//  @brief      ��Ԫ��ת��Ϊŷ����
//  @param      void
//  @param      void
//  @return     void
//  Sample usage: �ж�ֱ�ӵ���
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
///*   ���Բ�������̬�޶ȵ�����*/
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


float ax_offset = 0, ay_offset = 0;         //x,y��ļ��ٶ�ƫ����
float gx_offset = 0, gy_offset = 0;         //x,y��Ľ��ٶ�ƫ����

/*����*/
float rad2deg = 57.29578f;                  //���ȵ��ǶȵĻ���ϵ��
float roll_v = 0, pitch_v = 0;              //���㵽x,y���ϵĽ��ٶ�

/*����״̬������״̬���۲�״̬�����Ź���״̬*/
float gyro_roll = 0, gyro_pitch = 0;        //�����ǻ��ּ�����ĽǶȣ�����״̬
float acc_roll = 0, acc_pitch = 0;          //���ٶȼƹ۲���ĽǶȣ��۲�״̬
float k_roll = 0, k_pitch = 0;              //�������˲�����Ƴ����ŽǶȣ����Ź���״̬

/*���Э�������P*/
float e_P[2][2] ={{1,0},{0,1}};             //���Э������������e_P����������Ƶ�P��Ҳ�������µ�P

/*����������K*/
float k_k[2][2] ={{0,0},{0,0}};             //����Ŀ������������K��һ��2X2�ķ���


void Kalman_Filter()
{
    /*��ȡ���ٶȺͼ��ٶ� */
    get_icm42605_accdata_spi();
    get_icm42605_gyro_spi();

    IMU_getValues();

    /*step1:��������״̬*/
    /*����x,y���ϵĽ��ٶ�*/
    roll_v = (icm.gyroX-gx_offset) + ((sin(k_pitch)*sin(k_roll))/cos(k_pitch))*(icm.gyroY-gy_offset) + ((sin(k_pitch)*cos(k_roll))/cos(k_pitch))*icm.gyroZ;//roll��Ľ��ٶ�
    pitch_v = cos(k_roll)*(icm.gyroY-gy_offset) - sin(k_roll)*icm.gyroZ;//pitch��Ľ��ٶ�
    gyro_roll = k_roll + dt*roll_v;//����roll�Ƕ�
    gyro_pitch = k_pitch + dt*pitch_v;//����pitch�Ƕ�

    /*step2:�����������Э�������P*/
    e_P[0][0] = e_P[0][0] + 0.0025;//�����Q������һ���Խ����ҶԽ�Ԫ��Ϊ0.0025
    e_P[0][1] = e_P[0][1] + 0;
    e_P[1][0] = e_P[1][0] + 0;
    e_P[1][1] = e_P[1][1] + 0.0025; //ϵͳ����Ԥ������Q 0.0025

    /*step3:���¿���������K*/
    k_k[0][0] = e_P[0][0]/(e_P[0][0]+0.3); //ϵͳ���������R 0.3
    k_k[0][1] = 0;
    k_k[1][0] = 0;
    k_k[1][1] = e_P[1][1]/(e_P[1][1]+0.3);

    /*step4:�������Ź���״̬*/
    /*�۲�״̬*/
    //roll�Ƕ�
    acc_roll = atan((icm.accY - ay_offset) / (icm.accZ))*rad2deg;
    //pitch�Ƕ�
    acc_pitch = rad2deg * atan2(icm.accX, -icm.accZ);
    /*���Ź���״̬*/
    k_roll = gyro_roll + k_k[0][0]*(acc_roll - gyro_roll); //�����ں�
    k_pitch = gyro_pitch + k_k[1][1]*(acc_pitch - gyro_pitch);

    /*step5:����Э�������P*/
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
        ax_offset = ax_offset + icm.accX;//����x����ٶȵ�ƫ������
        ay_offset = ay_offset + icm.accY;//����y����ٶȵ�ƫ������
        gx_offset = gx_offset + icm.gyroX;
        gy_offset = gy_offset + icm.gyroY;
        systick_delay_ms(STM0, 10);
    }
    ax_offset = ax_offset / 100; //����x����ٶȵ�ƫ����
    ay_offset = ay_offset / 100; //����y����ٶȵ�ƫ����
    gx_offset = gx_offset / 100; //����x����ٶȵ�ƫ����
    gy_offset = gy_offset / 100; //����y����ٶȵ�ƫ����
}

