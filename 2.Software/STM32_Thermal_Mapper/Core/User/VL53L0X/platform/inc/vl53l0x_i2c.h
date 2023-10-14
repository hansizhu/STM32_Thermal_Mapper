#ifndef __VL53L0_I2C_H
#define __VL53L0_I2C_H

#include "main.h"


//IO��������
#define VL_SDA_IN()  {GPIOC->MODER&=~(3<<(11*2));GPIOC->MODER|=0<<11*2;}			//PC11��������
#define VL_SDA_OUT() {GPIOC->MODER&=~(3<<(11*2));GPIOC->MODER|=1<<11*2;}			//PC11�������


//IO��������	 
#define VL_IIC_SCL    PCout(12)   //SCL
#define VL_IIC_SDA    PCout(11) 	//SDA���
#define VL_READ_SDA   PCin(11) 		//SDA����

//״̬
#define STATUS_OK       0x00
#define STATUS_FAIL     0x01

//IIC��������
void VL53L0X_i2c_init(void);//��ʼ��IIC��IO��

u8 VL53L0X_write_byte(u8 address,u8 index,u8 data);              //IICдһ��8λ����
u8 VL53L0X_write_word(u8 address,u8 index,u16 data);             //IICдһ��16λ����
u8 VL53L0X_write_dword(u8 address,u8 index,u32 data);            //IICдһ��32λ����
u8 VL53L0X_write_multi(u8 address, u8 index,u8 *pdata,u16 count);//IIC����д

u8 VL53L0X_read_byte(u8 address,u8 index,u8 *pdata);             //IIC��һ��8λ����
u8 VL53L0X_read_word(u8 address,u8 index,u16 *pdata);            //IIC��һ��16λ����
u8 VL53L0X_read_dword(u8 address,u8 index,u32 *pdata);           //IIC��һ��32λ����
u8 VL53L0X_read_multi(u8 address,u8 index,u8 *pdata,u16 count);  //IIC������


void Delay_us(__IO uint32_t delay);

#endif 


