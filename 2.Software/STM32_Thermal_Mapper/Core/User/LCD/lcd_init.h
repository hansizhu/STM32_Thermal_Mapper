#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "main.h"

#define USE_HORIZONTAL 3  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 320

#else
#define LCD_W 320
#define LCD_H 240
#endif




//-----------------LCD端口定义---------------- 

#define		DATAOUT(x)			GPIOA->BSRR=(((uint32_t)(uint8_t)~(x))<<16)|((uint32_t)(uint8_t)(x))		//数据输出
#define 	DATAIN					GPIOA->IDR&0xFF;																												//数据输入

#define		LCD_DATA_IN()		{GPIOA->PUPDR&=0xFFFF0000;\
													 GPIOA->PUPDR|=0x00005555;\
													 GPIOA->MODER&=0xFFFF0000;\
													 GPIOA->MODER|=0x00000000;}		//PA0-7设置 上拉 输入

#define		LCD_DATA_OUT()	{GPIOA->PUPDR&=0xFFFF0000;\
													 GPIOA->PUPDR|=0x00000000;\
													 GPIOA->MODER&=0xFFFF0000;\
													 GPIOA->MODER|=0x00005555;}		//PA0-7设置 推挽 输出

#define		LCD_RES_Set()		PCout(4)=1		//复位				PC4
#define		LCD_CS_Set()		PCout(5)=1		//片选端口			PC5
#define		LCD_DC_Set()		PBout(0)=1		//数据/命令		PB0
#define		LCD_WR_Set()		PBout(1)=1		//写数据				PB1
#define		LCD_RD_Set()		PBout(2)=1		//读数据				PB2

#define		LCD_RES_Clr()		PCout(4)=0		//复位				PC4
#define		LCD_CS_Clr()		PCout(5)=0		//片选端口			PC5
#define		LCD_DC_Clr()		PBout(0)=0		//数据/命令		PB0
#define		LCD_WR_Clr()		PBout(1)=0		//写数据				PB1
#define		LCD_RD_Clr()		PBout(2)=0		//读数据				PB2

#define		LCD_START_LIGHT		700




void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(u8 dat);//模拟SPI时序
void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif




