#ifndef __ICM42605_H
#define __ICM42605_H

#include "headfile.h"


//
////====================================================硬件 SPI 驱动====================================================
//#define ICM42605_SPI_SPEED          system_clock/8                              // 硬件 SPI 速率
//#define ICM42605_SPI                SPI_0                                       // 硬件 SPI 号
//#define ICM42605_SPC_PIN            SPI0_SCLK_P20_11                            //SPI3_SCK_B3    // 硬件 SPI SCK 引脚
//#define ICM42605_SDI_PIN            SPI0_MOSI_P20_14                            //SPI3_MOSI_B5                                // 硬件 SPI MOSI 引脚
//#define ICM42605_SDO_PIN            SPI0_MOSI_P20_12                            //SPI3_MISO_B4                                // 硬件 SPI MISO 引脚
////====================================================硬件 SPI 驱动====================================================
//#define ICM42605_CS_PIN             SPI0_CS2_P20_13 //C10                                          // CS 片选引脚

#define ICM42605_TIMEOUT_COUNT      0x00FF                                      // ICM42605 超时计数

#define ICM42605_SPI_W              0x00
#define ICM42605_SPI_R              0x80





#define fifo_packet_structure_size   16
#define fifo_packet_num              32
#define fifo_packet_size             fifo_packet_structure_size*fifo_packet_num

#define who_am_i                0x75
#define ICM42605_ID             0x42

#define ICM_TEMP_OUTH_REG       0X1D    //温度值高八位寄存器
#define ICM_TEMP_OUTL_REG       0X1E    //温度值低8位寄存器

#define ICM_ACCEL_XOUTH_REG     0X1F    //加速度值,X轴高8位寄存器
#define ICM_ACCEL_XOUTL_REG     0X20    //加速度值,X轴低8位寄存器
#define ICM_ACCEL_YOUTH_REG     0X21    //加速度值,Y轴高8位寄存器
#define ICM_ACCEL_YOUTL_REG     0X22    //加速度值,Y轴低8位寄存器
#define ICM_ACCEL_ZOUTH_REG     0X23    //加速度值,Z轴高8位寄存器
#define ICM_ACCEL_ZOUTL_REG     0X24    //加速度值,Z轴低8位寄存器

#define ICM_GYRO_XOUTH_REG      0X25    //陀螺仪值,X轴高8位寄存器
#define ICM_GYRO_XOUTL_REG      0X26    //陀螺仪值,X轴低8位寄存器
#define ICM_GYRO_YOUTH_REG      0X27    //陀螺仪值,Y轴高8位寄存器
#define ICM_GYRO_YOUTL_REG      0X28    //陀螺仪值,Y轴低8位寄存器
#define ICM_GYRO_ZOUTH_REG      0X29    //陀螺仪值,Z轴高8位寄存器
#define ICM_GYRO_ZOUTL_REG      0X2A    //陀螺仪值,Z轴低8位寄存器


#define reg_bank_sel                 0x76
#define device_config_reg            0x11
#define bit_spi_mode                 0x10
#define bit_soft_reset_chip_config   0x01

#define intf_config4                 0x7A
#define pwr_mgmt0_reg                0x4E
//#define bit_temp_dis               0x20
#define bit_idle                     0x10
#define bit_gyro_mode_mask           ((0x03)<<2)
#define bit_gyro_mode_off            ((0x00)<<2)
#define bit_gyro_mode_standby        ((0x01)<<2)
//#define bit_gyro_mode_lp           ((0x02)<<2)
#define bit_gyro_mode_ln             ((0x03)<<2)
#define bit_accel_mode_mask          ((0x03)<<0)
#define bit_accel_mode_off           0x00
#define bit_accel_mode_lp            0x02
#define bit_accel_mode_ln            0x03

#define gyro_config0_reg             0x4F
#define bit_gyro_ui_fs_sel_shift     5
#define bit_gyro_ui_fs_sel_2000dps   ((0x00)<<bit_gyro_ui_fs_sel_shift)
#define bit_gyro_ui_fs_sel_1000dps   ((0x01)<<bit_gyro_ui_fs_sel_shift)
#define bit_gyro_ui_fs_sel_mask      ((0x07)<<bit_gyro_ui_fs_sel_shift)
#define bit_gyro_odr_100hz           ((0x08)<<0)
#define bit_gyro_odr_50hz            ((0x09)<<0)
#define bit_gyro_odr_nonflame_mask   ((0x0F)<<0)

#define accel_config0_reg            0x50
#define bit_accel_ui_fs_sel_shift    5
#define bit_accel_ui_fs_sel_8g       ((0x01)<<bit_accel_ui_fs_sel_shift)
#define bit_accel_ui_fs_sel_mask     ((0x07)<<bit_accel_ui_fs_sel_shift)
#define bit_accel_odr_100hz          ((0x08)<<0)
#define bit_accel_odr_50hz           ((0x09)<<0)
#define bit_accel_odr_nonflame_mask  ((0x0F)<<0)

#define int_source0_reg               0x65
#define bit_int_ui_fsync_int1_en      0x40
#define bit_int_pll_rdy_int1_en       0x20
#define bit_int_reset_done_int1_en    0x10
#define bit_int_ui_drdy_int1_en       0x08
#define bit_int_fifo_ths_int1_en      0x04//FIFO threshold interrupt
#define bit_int_fifo_full_int1_en     0x02
#define bit_int_ui_agc_rdy_int1_en    0x01

#define sensor_selftest_reg           0x6B
#define bit_accel_st_result           0x08
#define bit_gyro_st_result            0x04
#define bit_accel_st_status           0x02
#define bit_gyro_st_status            0x01

#define int_config_reg                0x14
#define bit_int2_mode                 0x20
#define bit_int2_drive_circuit        0x10
#define bit_int2_polarity             0x08
#define bit_int1_mode                 0x04
#define bit_int1_drive_circuit        0x02
#define bit_int1_polarity             0x01

#define fifo_config_reg               0x16
#define bit_fifo_mode_ctrl_mask       ((0x03)<<6)
#define bit_fifo_mode_ctrl_bypass     ((0x00)<<6)
#define bit_fifo_mode_ctrl_stream     ((0x01)<<6)
#define bit_fifo_mode_ctrl_snapshot   ((0x02)<<6)

#define tmst_config_reg                0x54
#define bit_fifo_ram_iso_ena           0x40
#define bit_en_dreg_fifo_d2a           0x20
#define bit_tmst_to_regs_en            0x10
#define bit_tmst_resol                 0x08
#define bit_tmst_delta_en              0x04
#define bit_tmst_fsync_en              0x02
#define bit_tmst_en                    0x01

#define fifo_config2_reg               0x60
#define fifo_config3_reg               0x61

#define fsync_config_reg               0x62
#define bit_fsync_ui_sel_mask          ((0x07)<<4)
#define bit_fsync_ui_sel_tag_temp      ((0x01)<<4)
#define bit_fsync_ui_flag_clear_sel    0x02

#define fifo_config1_reg               0x5F
#define bit_fifo_resume_partial        0x40
#define bit_fifo_wm_gt_th              0x20
#define bit_fifo_hires_en              0x10
#define bit_fifo_tmst_fsync_en         0x08
#define bit_fifo_temp_en               0x04
#define bit_fifo_gyro_en               0x02
#define bit_fifo_accel_en              0x01

#define int_config0_reg                0x63
#define int_config1_reg                0x64
#define bit_int_asy_rst_disable        0x10

#define fifo_byte_count_h_res          0x2E
#define fifo_byte_count_l_res          0x2F

#define fifo_accel_en                  0x40
#define fifo_gyro_en                   0x20

#define fifo_data_port                 0x30


#define gyr_ssl                        32.8f
#define acc_ssl                        0.244f

extern int16 icm_gyro_x,icm_gyro_y,icm_gyro_z;
extern int16 icm_acc_x,icm_acc_y,icm_acc_z;

typedef struct
{
    int16 Sample_accdata[3*fifo_packet_num];
    int16 Sample_gyrdata[3*fifo_packet_num];
}Sample_data_type_t;

//
////void ICM_Read_Len(uint8_t reg,uint8_t len,uint8_t *buf);
////uint8_t icm42605_read_reg(uint8_t reg);
////uint8_t icm42605_write_reg(uint8_t reg,uint8_t value);
////uint8_t icm42605_read_regs(uint8_t reg,uint8_t *buf,uint16_t len);
//void ICM_Get_Gyroscope(short *gx,short *gy,short *gz);
//void ICM_Get_Accelerometer(short *ax,short *ay,short *az);
//void ICM_Get_Raw_data(short *ax,short *ay,short *az,short *gx,short *gy,short *gz);
//uint8 icm42605_init(void);
//void icm42605_spi_w_reg_byte(uint8 cmd, uint8 val);
//void icm42605_spi_r_reg_byte(uint8 cmd, uint8 *val);
////void icm42605_read_fifo(Sample_data_type_t *data,uint16_t len);
////void icm42605_stop(void);
////void Test_ICM42605FIFO(void);

void icm42605_spi_w_reg_byte(uint8 cmd, uint8 val);
void icm42605_spi_r_reg_byte(uint8 cmd, uint8 *val);
void icm42605_spi_r_reg_bytes(uint8 * val, uint8 num);
void icm42605_self3_check(void);
uint8 icm42605_init(void);
void get_icm42605_accdata_spi(void);
void get_icm42605_gyro_spi(void);

static uint8 icm_spi_r_reg_bytes(uint8 reg);


#endif
