/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            ICM20602
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ3184284598)
 * @version         查看doc内version文件 版本说明
 * @Software        ADS v1.2.2
 * @Target core     TC377TP
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-11-23
 * @note
                    接线定义：
                    ------------------------------------
                        SCL                 查看SEEKFREE_IIC文件内的SEEKFREE_SCL宏定义
                        SDA                 查看SEEKFREE_IIC文件内的SEEKFREE_SDA宏定义
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
//  以下函数是使用硬件SPI通信，相比较IIC，速度比IIC快非常多。
//-------------------------------------------------------------------------------------------------------------------
#define SPI_NUM        SPI_0//SPI_4//SPI_4//SPI_0////
#define SPI_SCK_PIN    SPI0_SCLK_P20_11////SPI4_SCLK_P22_3 //  //////                   //接模块SPC
#define SPI_MOSI_PIN   SPI0_MOSI_P20_14//SPI4_MOSI_P22_0////////                    //接模块SDI
#define SPI_MISO_PIN   SPI0_MISO_P20_12//SPI4_MISO_P22_1////////         //接模块SDO
#define SPI_CS_PIN     SPI0_CS2_P20_13//SPI0_CS6_P20_10////SPI4_CS6_P23_1// SPI0_CS13_P15_0//SPI3_CS0_P02_4//// ////                         //接模块CS
////#define ICM42605_CS(x)              (x? (gpio_set(SPI_CS_PIN,1)): (gpio_set(SPI_CS_PIN,0))) //驱动宏定义


//#define SPI_NUM        SPI_4//SPI_4//SPI_0////
//#define SPI_SCK_PIN    SPI4_SCLK_P22_3 //  //////                   //接模块SPC
//#define SPI_MOSI_PIN   SPI4_MOSI_P22_0////////                    //接模块SDI
//#define SPI_MISO_PIN   SPI4_MISO_P22_1////////         //接模块SDO
//#define SPI_CS_PIN     SPI4_CS6_P23_1// SPI0_CS13_P15_0//SPI3_CS0_P02_4//// ////                         //接模块CS
//#define ICM42605_CS(x)              (x? (gpio_set(SPI_CS_PIN,1)): (gpio_set(SPI_CS_PIN,0))) //驱动宏定义


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM42605 SPI写寄存器
//  @param      cmd     寄存器地址
//  @param      val     需要写入的数据
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
//  @brief      ICM42605 SPI读寄存器
//  @param      cmd     寄存器地址
//  @param      *val    接收数据的地址
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
//*  函数名称：uint8_t icm42605_spi_r_reg_bytes(uint8_t reg)
//*  功能说明：读取数据
//*  参数说明：uint8_t reg寄存器地址
//*  函数返回：无
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
//  @brief      ICM42605 SPI多字节读寄存器
//  @param      cmd     寄存器地址
//  @param      *val    接收数据的地址
//  @param      num     读取数量
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm42605_spi_r_reg_bytes(uint8 * val, uint8 num)
{
    spi_mosi(SPI_NUM,SPI_CS_PIN,val,val,num,1);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM42605自检函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm42605_self3_check(void)
{
    uint8 dat=0;
    while(ICM42605_ID != dat)   //读取ICM20602 ID
    {
        icm42605_spi_r_reg_byte(who_am_i,&dat);
        printf("%d/n",dat);
        systick_delay_ms(STM0, 10);
        //卡在这里原因有以下几点
        //1 ICM20602坏了，如果是新的这样的概率极低
        //2 接线错误或者没有接好
        //3 可能你需要外接上拉电阻，上拉到3.3V
    }

}

/*************************************************************************
*  函数名称：uint8_t icm42605_init(void)
*  功能说明：ICM42605初始化
*  参数说明：无
*  函数返回：0 成功；1失败
*************************************************************************/
uint8 icm42605_init(void)
{
    uint8 reg_val =0x0;
    systick_delay_ms(STM0,10);  //上电延时

    (void)spi_init(SPI_NUM, SPI_SCK_PIN, SPI_MOSI_PIN,  SPI_MISO_PIN , SPI_CS_PIN , 3, 10*1000*1000);

    icm42605_self3_check();
    reg_val = icm_spi_r_reg_bytes(who_am_i);  //who_am_i
    printf("ID:0x%02X",reg_val);

    icm42605_spi_w_reg_byte(reg_bank_sel,0x00);  //设置bank 0区域寄存器
    icm42605_spi_w_reg_byte(device_config_reg,bit_soft_reset_chip_config);  //软复位传感器
    systick_delay_ms(STM0,50);
  
    if(reg_val == 0x42) {
        icm42605_spi_w_reg_byte(reg_bank_sel,0x01);  //设置bank 1区域寄存器
        icm42605_spi_w_reg_byte(intf_config4,0x02);  //设置为4线SPI通信

        icm42605_spi_w_reg_byte(reg_bank_sel,0x00);  //设置bank 0区域寄存器
        icm42605_spi_w_reg_byte(fifo_config_reg,0x40);  //Stream-to-FIFO Mode

        reg_val = icm_spi_r_reg_bytes(int_source0_reg);  //保存寄存器的值
        icm42605_spi_w_reg_byte(int_source0_reg,0x00);  //失能中断
        icm42605_spi_w_reg_byte(fifo_config2_reg,0x00);// Lower bits of FIFO watermark
        icm42605_spi_w_reg_byte(fifo_config3_reg,0x02);// Upper bits of FIFO watermark
        icm42605_spi_w_reg_byte(int_source0_reg, reg_val);  //重新写回寄存器的值
        icm42605_spi_w_reg_byte(fifo_config1_reg,0x63);// 使能加速度和角速度的FIFO

        icm42605_spi_w_reg_byte(reg_bank_sel,0x00);
        icm42605_spi_w_reg_byte(int_config_reg,0x36);  //设置INT1和INT2为推挽锁存模式

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
//  @brief      获取ICM42605加速度计数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:               执行该函数后，直接查看对应的变量即可
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
//  @brief      获取ICM42605陀螺仪数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:               执行该函数后，直接查看对应的变量即可
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
