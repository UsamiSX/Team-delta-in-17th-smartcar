#include "zf_gpio.h"
#include "ccu6_pwm.h"
#include "move_filter.h"
#include "motor.h"
#include "SEEKFREE_FUN.h"
#include "hall.h"


uint8 control_hall[6]={6,2,3,1,5,4};        //����ʱ�����


uint8 hall_steps_normal[2][8]={             //����ת���Ļ���ʱ��
        {0,2,3,4,5,6,1,0},                    //0������ռλ ������
        {0,6,1,2,3,4,5,0},
};

uint8 hall_steps_advance[2][8]={            //��ǰ1�໻��Ļ���ʱ��
        {0,3,4,5,6,1,2,0},                    //0������ռλ ������
        {0,5,6,1,2,3,4,0},
};

int8 hall_value_now;     //��ǰ������ֵ
int8 hall_value_last;    //�ϴλ�����ֵ
int8 next_hall_value;    //��һ�λ�����ֵ
uint8  hall_index = 0;   //����ʱ����±�
uint16 commutation_time_save[6] = {COMMUTATION_TIMEOUT, COMMUTATION_TIMEOUT, COMMUTATION_TIMEOUT, COMMUTATION_TIMEOUT, COMMUTATION_TIMEOUT, COMMUTATION_TIMEOUT};  //����ʱ�� ����6��
uint32 commutation_time_sum = 0;    //���λ���ʱ���ܺ�
uint16 commutation_time = COMMUTATION_TIMEOUT;    //ͳ�Ʊ��λ�������ʱ��
uint32 commutation_delay = COMMUTATION_TIMEOUT;   //�ӳٻ�����ʱʱ��
uint16 commutation_delay_ratio = 6; //������ʱʱ�� = commutation_delay_ratio*commutation_time_sum>>7 ��ֵԽС����Խ��ǰ

uint8 motor_state;
uint8 motor_dir;


void hall_init(void)
{
    gpio_init(HALLA_PIN, GPI, 0, PULLUP);
    gpio_init(HALLB_PIN, GPI, 0, PULLUP);
    gpio_init(HALLC_PIN, GPI, 0, PULLUP);
    read_hall_value();
}

void read_hall_value(void)
{
    uint8 ha, hb, hc;

    ha = gpio_get(HALLA_PIN);
    hb = gpio_get(HALLB_PIN);
    hc = gpio_get(HALLC_PIN);

    gpio_set(P21_2, ha);
    gpio_set(P21_3, hb);
    gpio_set(P21_4, hc);

    hall_value_now = ha + hb*2 + hc*4;

    //����ʱ��ת�� ��ԭʼ�Ļ���ʱ����������
    for(int i=0; i<6; i++)
    {
        if(hall_value_now == control_hall[i])
        {
            hall_value_now = i+1;
            break;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���㵱ǰ���ٶ�
//  @param      void
//  @return     void
//  @since
//-------------------------------------------------------------------------------------------------------------------
inline void calc_speed(void)
{
    //ת�ټ���
    uint8 i;
    int32 speed;

    commutation_time_sum = 0;
    for(i=0; i<6; i++)
    {
        commutation_time_sum += commutation_time_save[i];
    }

    //���ת�ټ���˵��
    //2.commutation_time_sum��ͳ�Ƶ������6�λ������ٴ�T12�����ж�
        //2.1 T12�����ж������жϵ�Ƶ��Ϊ20KHz
        //2.2 �������ù�ϵccu6_t12_pwm->scan_hall_status->calc_speed
        //2.3 commutation_timeΪͳ�ƻ���ʱ��ı���
    //3.ͨ�����ת�����Ƕ���RPM��ʾ��RPM��ʾÿ���ӵ����ת��
    //3.���תһȦ��Ҫ����Ĵ������� ���������*6
    //4.��˵��ת�ٵ���60*ADC�ж�Ƶ��/���������/commutation_time_sum���������Եõ����ÿ���ӵ�ת��

    if(commutation_time_sum == 0) commutation_time_sum = 1;

    speed = PWM_PIT_NUM/POLEPAIRS/commutation_time_sum;

    if(speed >= 10000) speed = 0;


//    if((hall_value_now == 6 && (hall_value_last == 1 || hall_value_last == 2)) || (hall_value_now == 1 && (hall_value_last == 2 || hall_value_last == 3)) || (hall_value_now == 2 && (hall_value_last == 3 || hall_value_last == 4)) || (hall_value_now == 3 && (hall_value_last == 4 || hall_value_last == 5)) || (hall_value_now == 4 && (hall_value_last == 5 || hall_value_last == 6)) || (hall_value_now == 5 && (hall_value_last == 6 || hall_value_last == 1)))//�����ת��ʱ����Ҫ���ٶ�ȡ��
//    {
//        speed = -speed;
//    }

    switch(hall_value_last)
    {//                                   ��ת                 ��ת
        case 1:if(hall_value_now!=2){motor_dir=1;}else if(hall_value_now==2){motor_dir=0;}break;
        case 2:if(hall_value_now!=3){motor_dir=1;}else if(hall_value_now==3){motor_dir=0;}break;
        case 3:if(hall_value_now!=4){motor_dir=1;}else if(hall_value_now==4){motor_dir=0;}break;
        case 4:if(hall_value_now!=5){motor_dir=1;}else if(hall_value_now==5){motor_dir=0;}break;
        case 5:if(hall_value_now!=6){motor_dir=1;}else if(hall_value_now==6){motor_dir=0;}break;
        case 6:if(hall_value_now!=1){motor_dir=1;}else if(hall_value_now==1){motor_dir=0;}break;
        default:break;
    }
    if(motor_dir == 1) speed = -speed;

    move_filter_calc(&speed_filter, speed);
}

void scan_hall_status(void)
{
    read_hall_value();      //��ȡ��ǰ����ֵ

    commutation_time++;

    //���೬ʱ����
    //������೬����250ms����Ϊ���ֶ�ת��Ӧ�ü�ʱ���ٶ�����Ϊ0
    if(commutation_time >= COMMUTATION_TIMEOUT)
    {
        commutation_time = COMMUTATION_TIMEOUT;
        commutation_time_save[0] = COMMUTATION_TIMEOUT;
        commutation_time_save[1] = COMMUTATION_TIMEOUT;
        commutation_time_save[2] = COMMUTATION_TIMEOUT;
        commutation_time_save[3] = COMMUTATION_TIMEOUT;
        commutation_time_save[4] = COMMUTATION_TIMEOUT;
        commutation_time_save[5] = COMMUTATION_TIMEOUT;

        //����ƽ���˲���ʼ��
        move_filter_init(&speed_filter);
        motor_state=MOTOR_STOP;
    }
    else
    {
        motor_state=MOTOR_RUN;
    }

    if(hall_value_now==7 || hall_value_now==0) return;      //�������ݶ�ȡ���� ��ִ�л��ಿ��

    if( ((hall_value_now != hall_value_last) && !commutation_delay) || !next_hall_value)        //��ʼ����
    {
        hall_index++;
        if(hall_index >= 6)
        {
            hall_index = 0;
        }
        commutation_time_save[hall_index] = commutation_time;       //�ٶ���Ϣ����

        commutation_time=0;

        //ת���ٶȼ���
        calc_speed();

        if((speed_filter.data_average > 4000) || (speed_filter.data_average < -4000))
        {
//            �ٶȴ���һ����ʱ������ӳٽϴ�
//            ��˲��ó�ǰ�������ʱ�ķ�ʽȥƥ����ѵĻ����
            next_hall_value = hall_steps_advance[motor_control.dir][hall_value_now];
            commutation_delay = limit_ab((commutation_delay_ratio*commutation_time_sum)>>7,0,DELAY_MAX);
        }
        else
        {
            //�ٶȽϵ����賬ǰ����
            next_hall_value = hall_steps_normal[motor_control.dir][hall_value_now];
            commutation_delay = 0;
        }
        hall_value_last = hall_value_now;
    }
}
