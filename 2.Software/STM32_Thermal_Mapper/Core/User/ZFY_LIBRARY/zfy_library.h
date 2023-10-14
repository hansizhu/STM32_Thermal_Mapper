/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __zfy_library_H
#define __zfy_library_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** 
  * @brief  ZFY库返回值
  */
typedef enum
{
  ZFY_UNPASS = 0,
  ZFY_PASS,
	ZFY_ERROR = -1
}ZFY_Return;



/* z_application.c -----------------------------------------------------------*/

ZFY_Return Historical_Differences_Z(int data, uint8_t sign);		//历史数据比较函数

uint32_t Array_Return_Num_Z(uint16_t array[],uint16_t len);		//读取数组和
uint16_t Array_Return_Max_Z(uint16_t array[],uint16_t len);		//读取数组最大值
uint16_t Array_Return_Min_Z(uint16_t array[],uint16_t len);		//读取数组最小值

uint16_t Array_Return_Max_Sub_Z(uint16_t array[],uint16_t len);		//读取最大值下标

/* z_calculation.c -----------------------------------------------------------*/

uint16_t Filtering1_Recursive_Average_Z(uint16_t in_data,uint8_t flag);			//算法1_递推平均滤波
uint16_t Filtering2_Median_Average_Z(uint16_t in_data,uint8_t flag);				//算法2_中位数平均滤波
uint16_t Filtering3_Weighting_Median_Average_Z(uint16_t in_data,uint8_t flag);		//算法3_加权中位数平均滤波

/* z_system.c ----------------------------------------------------------------*/

void Delay_us_Z(__IO uint32_t delay);							//微秒延时
void APP_Jump_Z(uint32_t appxaddr);								//跳转至指定内存运行
void APP_Jump_Run_Z(uint32_t app_address);				//程序跳转后运行


#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
