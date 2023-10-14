#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "main.h"

#define USE_HORIZONTAL 3  //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 320

#else
#define LCD_W 320
#define LCD_H 240
#endif




//-----------------LCD�˿ڶ���---------------- 

#define		DATAOUT(x)			GPIOA->BSRR=(((uint32_t)(uint8_t)~(x))<<16)|((uint32_t)(uint8_t)(x))		//�������
#define 	DATAIN					GPIOA->IDR&0xFF;																												//��������

#define		LCD_DATA_IN()		{GPIOA->PUPDR&=0xFFFF0000;\
													 GPIOA->PUPDR|=0x00005555;\
													 GPIOA->MODER&=0xFFFF0000;\
													 GPIOA->MODER|=0x00000000;}		//PA0-7���� ���� ����

#define		LCD_DATA_OUT()	{GPIOA->PUPDR&=0xFFFF0000;\
													 GPIOA->PUPDR|=0x00000000;\
													 GPIOA->MODER&=0xFFFF0000;\
													 GPIOA->MODER|=0x00005555;}		//PA0-7���� ���� ���

#define		LCD_RES_Set()		PCout(4)=1		//��λ				PC4
#define		LCD_CS_Set()		PCout(5)=1		//Ƭѡ�˿�			PC5
#define		LCD_DC_Set()		PBout(0)=1		//����/����		PB0
#define		LCD_WR_Set()		PBout(1)=1		//д����				PB1
#define		LCD_RD_Set()		PBout(2)=1		//������				PB2

#define		LCD_RES_Clr()		PCout(4)=0		//��λ				PC4
#define		LCD_CS_Clr()		PCout(5)=0		//Ƭѡ�˿�			PC5
#define		LCD_DC_Clr()		PBout(0)=0		//����/����		PB0
#define		LCD_WR_Clr()		PBout(1)=0		//д����				PB1
#define		LCD_RD_Clr()		PBout(2)=0		//������				PB2

#define		LCD_START_LIGHT		700




void LCD_GPIO_Init(void);//��ʼ��GPIO
void LCD_Writ_Bus(u8 dat);//ģ��SPIʱ��
void LCD_WR_DATA8(u8 dat);//д��һ���ֽ�
void LCD_WR_DATA(u16 dat);//д�������ֽ�
void LCD_WR_REG(u8 dat);//д��һ��ָ��
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//�������꺯��
void LCD_Init(void);//LCD��ʼ��
#endif




