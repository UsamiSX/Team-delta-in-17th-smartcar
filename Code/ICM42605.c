/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            ICM20602
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ3184284598)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        ADS v1.2.2
 * @Target core     TC377TP
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-11-23
 * @note
                    ���߶��壺
                    ------------------------------------
                        SCL                 �鿴SEEKFREE_IIC�ļ��ڵ�SEEKFREE_SCL�궨��
                        SDA                 �鿴SEEKFREE_IIC�ļ��ڵ�SEEKFREE_SDA�궨��
                    ------------------------------------
 ********************************************************************************************************************/


#include "zf_stm_systick.h"
#include "zf_gpio.h"
#include "zf_spi.h"
#include "SEEKFREE_IIC.h"
#include "ICM42605.h"


short gyroX,gyroY,gyroZ;
short accX,accY,accZ;


//-------------------------------------------------------------------------------------------------------------------
//  ���º�����ʹ��Ӳ��SPIͨ�ţ���Ƚ�IIC���ٶȱ�IIC��ǳ��ࡣ
//-------------------------------------------------------------------------------------------------------------------
#define SPI_NUM        SPI_0//SPI_4//SPI_4//SPI_0////
#define SPI_SCK_PIN    SPI0_SCLK_P20_11////SPI4_SCLK_P22_3 //  //////                   //��ģ��SPC
#define SPI_MOSI_PIN   SPI0_MOSI_P20_14//SPI4_MOSI_P22_0////////                    //��ģ��SDI
#define SPI_MISO_PIN   SPI0_MISO_P20_12//SPI4_MISO_P22_1////////         //��ģ��SDO
#define SPI_CS_PIN     SPI0_CS2_P20_13//SPI0_CS6_P20_10////SPI4_CS6_P23_1// SPI0_CS13_P15_0//SPI3_CS0_P02_4//// ////                         //��ģ��CS
////#define ICM42605_CS(x)              (x? (gpio_set(SPI_CS_PIN,1)): (gpio_set(SPI_CS_PIN,0))) //�����궨��


//#define SPI_NUM        SPI_4//SPI_4//SPI_0////
//#define SPI_SCK_PIN    SPI4_SCLK_P22_3 //  //////                   //��ģ��SPC
//#define SPI_MOSI_PIN   SPI4_MOSI_P22_0////////                    //��ģ��SDI
//#define SPI_MISO_PIN   SPI4_MISO_P22_1////////         //��ģ��SDO
//#define SPI_CS_PIN     SPI4_CS6_P23_1// SPI0_CS13_P15_0//SPI3_CS0_P02_4//// ////                         //��ģ��CS
//#define ICM42605_CS(x)              (x? (gpio_set(SPI_CS_PIN,1)): (gpio_set(SPI_CS_PIN,0))) //�����궨��


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM42605 SPIд�Ĵ���
//  @param      cmd     �Ĵ�����ַ
//  @param      val     ��Ҫд�������
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm42605_spi_w_reg_byte(uint8 cmd, uint8 val)
{
    uint8 dat[2];

    dat[0] = cmd | ICM42605_SPI_W;
    dat[1] = val;

    spi_mosi(SPI_NUM,SPI_CS_PIN,dat,dat,2,1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM42605 SPI���Ĵ���
//  @param      cmd     �Ĵ�����ַ
//  @param      *val    �������ݵĵ�ַ
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm42605_spi_r_reg_byte(uint8 cmd, uint8 *val)
{
    uint8 dat[2];

    dat[0] = cmd | ICM42605_SPI_R;
    dat[1] = *val;

    spi_mosi(SPI_NUM,SPI_CS_PIN,dat,dat,2,1);

    *val = dat[1];
}

///*************************************************************************
//*  �������ƣ�uint8_t icm42605_spi_r_reg_bytes(uint8_t reg)
//*  ����˵������ȡ����
//*  ����˵����uint8_t reg�Ĵ�����ַ
//*  �������أ���
//*************************************************************************/

//void icm42605_spi_r_reg_byte(uint8 cmd, uint8 *val)
//{
//    uint8 dat[2];
//
//    dat[0] = cmd | ICM42605_SPI_R;
//    dat[1] = *val;
//
//    spi_mosi(ICM42605_SPI,ICM42605_CS_PIN,dat,dat,2,1);
//
//    *val = dat[1];
//}

static uint8 icm_spi_r_reg_bytes(uint8 reg)
{
    uint8 dat;
    uint8 value[1];
    gpio_set(P20_13,0);
    icm42605_spi_r_reg_byte(reg,value);
    dat = *value;
    gpio_set(P20_13,1);
    return dat;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM42605 SPI���ֽڶ��Ĵ���
//  @param      cmd     �Ĵ�����ַ
//  @param      *val    �������ݵĵ�ַ
//  @param      num     ��ȡ����
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm42605_spi_r_reg_bytes(uint8 * val, uint8 num)
{
    spi_mosi(SPI_NUM,SPI_CS_PIN,val,val,num,1);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM42605�Լ캯��
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm42605_self3_check(void)
{
    uint8 dat=0;
    while(ICM42605_ID != dat)   //��ȡICM20602 ID
    {
        icm42605_spi_r_reg_byte(who_am_i,&dat);
        printf("%d/n",dat);
        systick_delay_ms(STM0, 10);
        //��������ԭ�������¼���
        //1 ICM20602���ˣ�������µ������ĸ��ʼ���
        //2 ���ߴ������û�нӺ�
        //3 ��������Ҫ����������裬������3.3V
    }

}

/*************************************************************************
*  �������ƣ�uint8_t icm42605_init(void)
*  ����˵����ICM42605��ʼ��
*  ����˵������
*  �������أ�0 �ɹ���1ʧ��
*************************************************************************/
uint8 icm42605_init(void)
{
    uint8 reg_val =0x0;
    systick_delay_ms(STM0,10);  //�ϵ���ʱ

    (void)spi_init(SPI_NUM, SPI_SCK_PIN, SPI_MOSI_PIN,  SPI_MISO_PIN , SPI_CS_PIN , 3, 10*1000*1000);

    icm42605_self3_check();
    reg_val = icm_spi_r_reg_bytes(who_am_i);  //who_am_i
    printf("ID:0x%02X",reg_val);

    icm42605_spi_w_reg_byte(reg_bank_sel,0x00);  //����bank 0����Ĵ���
    icm42605_spi_w_reg_byte(device_config_reg,bit_soft_reset_chip_config);  //��λ������
    systick_delay_ms(STM0,50);
  
    if(reg_val == 0x42) {
        icm42605_spi_w_reg_byte(reg_bank_sel,0x01);  //����bank 1����Ĵ���
        icm42605_spi_w_reg_byte(intf_config4,0x02);  //����Ϊ4��SPIͨ��

        icm42605_spi_w_reg_byte(reg_bank_sel,0x00);  //����bank 0����Ĵ���
        icm42605_spi_w_reg_byte(fifo_config_reg,0x40);  //Stream-to-FIFO Mode

        reg_val = icm_spi_r_reg_bytes(int_source0_reg);  //����Ĵ�����ֵ
        icm42605_spi_w_reg_byte(int_source0_reg,0x00);  //ʧ���ж�
        icm42605_spi_w_reg_byte(fifo_config2_reg,0x00);// Lower bits of FIFO watermark
        icm42605_spi_w_reg_byte(fifo_config3_reg,0x02);// Upper bits of FIFO watermark
        icm42605_spi_w_reg_byte(int_source0_reg, reg_val);  //����д�ؼĴ�����ֵ
        icm42605_spi_w_reg_byte(fifo_config1_reg,0x63);// ʹ�ܼ��ٶȺͽ��ٶȵ�FIFO

        icm42605_spi_w_reg_byte(reg_bank_sel,0x00);
        icm42605_spi_w_reg_byte(int_config_reg,0x36);  //����INT1��INT2Ϊ��������ģʽ

        icm42605_spi_w_reg_byte(reg_bank_sel, 0x00);
        reg_val = (icm_spi_r_reg_bytes(int_source0_reg)|bit_int_fifo_ths_int1_en);  //EN FIFO
        icm42605_spi_w_reg_byte(int_source0_reg, reg_val);

        icm42605_spi_w_reg_byte(reg_bank_sel, 0x00);
        reg_val = ((icm_spi_r_reg_bytes(accel_config0_reg)&0x1F)|(bit_accel_ui_fs_sel_8g));//8g
        icm42605_spi_w_reg_byte(accel_config0_reg, reg_val);

        icm42605_spi_w_reg_byte(reg_bank_sel, 0x00);
        reg_val = ((icm_spi_r_reg_bytes(accel_config0_reg)&0xF0)|bit_accel_odr_50hz);  //50Hz
        icm42605_spi_w_reg_byte(accel_config0_reg, reg_val);

        icm42605_spi_w_reg_byte(reg_bank_sel, 0x00);
        reg_val = ((icm_spi_r_reg_bytes(gyro_config0_reg)&0x1F)|(bit_gyro_ui_fs_sel_2000dps));  //2000dps
        icm42605_spi_w_reg_byte(gyro_config0_reg,reg_val);

        icm42605_spi_w_reg_byte(reg_bank_sel, 0x00);
        reg_val = ((icm_spi_r_reg_bytes(gyro_config0_reg)&0xF0)|bit_gyro_odr_50hz);  //50Hz
        icm42605_spi_w_reg_byte(gyro_config0_reg, reg_val);

        icm42605_spi_w_reg_byte(reg_bank_sel, 0x00);
        reg_val = icm_spi_r_reg_bytes(pwr_mgmt0_reg)|(bit_accel_mode_ln); // Accel on in LNM
        icm42605_spi_w_reg_byte(pwr_mgmt0_reg, reg_val);
        systick_delay_ms(STM0,1);

        icm42605_spi_w_reg_byte(reg_bank_sel, 0x00);
        reg_val = icm_spi_r_reg_bytes(pwr_mgmt0_reg)|(bit_gyro_mode_ln); // Gyro on in LNM
        icm42605_spi_w_reg_byte(pwr_mgmt0_reg, reg_val);
        systick_delay_ms(STM0,1);

        printf("[Info] ICM42605.c: ICM42605 Initial is OK\n");
        return 0;
    }
    else {
        return 1;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡICM42605���ٶȼ�����
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:               ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void get_icm42605_accdata_spi(void)
{
    struct
    {
        uint8 reg;
        uint8 dat[6];
    }buf;

    buf.reg = ICM_ACCEL_XOUTH_REG | ICM42605_SPI_R;

    icm42605_spi_r_reg_bytes(&buf.reg, 7);
    accX = (int16)(((uint16)buf.dat[0]<<8 | buf.dat[1]));
    accY = (int16)(((uint16)buf.dat[2]<<8 | buf.dat[3]));
    accZ = (int16)(((uint16)buf.dat[4]<<8 | buf.dat[5]));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡICM42605����������
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:               ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void get_icm42605_gyro_spi(void)
{
    struct
    {
        uint8 reg;
        uint8 dat[6];
    }buf;

    buf.reg = ICM_GYRO_XOUTH_REG | ICM42605_SPI_R;

    icm42605_spi_r_reg_bytes(&buf.reg, 7);
    gyroX = (int16)(((uint16)buf.dat[0]<<8 | buf.dat[1]));
    gyroY = (int16)(((uint16)buf.dat[2]<<8 | buf.dat[3]));
    gyroZ = (int16)(((uint16)buf.dat[4]<<8 | buf.dat[5]));
}
