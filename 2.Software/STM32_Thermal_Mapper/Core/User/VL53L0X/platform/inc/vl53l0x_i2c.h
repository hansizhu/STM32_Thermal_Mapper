#ifndef __VL53L0_I2C_H
#define __VL53L0_I2C_H

#include "main.h"


//IO方向设置
#define VL_SDA_IN()  {GPIOC->MODER&=~(3<<(11*2));GPIOC->MODER|=0<<11*2;}			//PC11设置输入
#define VL_SDA_OUT() {GPIOC->MODER&=~(3<<(11*2));GPIOC->MODER|=1<<11*2;}			//PC11设置输出


//IO操作函数	 
#define VL_IIC_SCL    PCout(12)   //SCL
#define VL_IIC_SDA    PCout(11) 	//SDA输出
#define VL_READ_SDA   PCin(11) 		//SDA输入

//状态
#define STATUS_OK       0x00
#define STATUS_FAIL     0x01

//IIC操作函数
void VL53L0X_i2c_init(void);//初始化IIC的IO口

u8 VL53L0X_write_byte(u8 address,u8 index,u8 data);              //IIC写一个8位数据
u8 VL53L0X_write_word(u8 address,u8 index,u16 data);             //IIC写一个16位数据
u8 VL53L0X_write_dword(u8 address,u8 index,u32 data);            //IIC写一个32位数据
u8 VL53L0X_write_multi(u8 address, u8 index,u8 *pdata,u16 count);//IIC连续写

u8 VL53L0X_read_byte(u8 address,u8 index,u8 *pdata);             //IIC读一个8位数据
u8 VL53L0X_read_word(u8 address,u8 index,u16 *pdata);            //IIC读一个16位数据
u8 VL53L0X_read_dword(u8 address,u8 index,u32 *pdata);           //IIC读一个32位数据
u8 VL53L0X_read_multi(u8 address,u8 index,u8 *pdata,u16 count);  //IIC连续读


void Delay_us(__IO uint32_t delay);

#endif 


