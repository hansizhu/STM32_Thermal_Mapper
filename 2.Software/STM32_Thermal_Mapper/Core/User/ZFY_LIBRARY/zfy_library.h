/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __zfy_library_H
#define __zfy_library_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** 
  * @brief  ZFY�ⷵ��ֵ
  */
typedef enum
{
  ZFY_UNPASS = 0,
  ZFY_PASS,
	ZFY_ERROR = -1
}ZFY_Return;



/* z_application.c -----------------------------------------------------------*/

ZFY_Return Historical_Differences_Z(int data, uint8_t sign);		//��ʷ���ݱȽϺ���

uint32_t Array_Return_Num_Z(uint16_t array[],uint16_t len);		//��ȡ�����
uint16_t Array_Return_Max_Z(uint16_t array[],uint16_t len);		//��ȡ�������ֵ
uint16_t Array_Return_Min_Z(uint16_t array[],uint16_t len);		//��ȡ������Сֵ

uint16_t Array_Return_Max_Sub_Z(uint16_t array[],uint16_t len);		//��ȡ���ֵ�±�

/* z_calculation.c -----------------------------------------------------------*/

uint16_t Filtering1_Recursive_Average_Z(uint16_t in_data,uint8_t flag);			//�㷨1_����ƽ���˲�
uint16_t Filtering2_Median_Average_Z(uint16_t in_data,uint8_t flag);				//�㷨2_��λ��ƽ���˲�
uint16_t Filtering3_Weighting_Median_Average_Z(uint16_t in_data,uint8_t flag);		//�㷨3_��Ȩ��λ��ƽ���˲�

/* z_system.c ----------------------------------------------------------------*/

void Delay_us_Z(__IO uint32_t delay);							//΢����ʱ
void APP_Jump_Z(uint32_t appxaddr);								//��ת��ָ���ڴ�����
void APP_Jump_Run_Z(uint32_t app_address);				//������ת������


#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
