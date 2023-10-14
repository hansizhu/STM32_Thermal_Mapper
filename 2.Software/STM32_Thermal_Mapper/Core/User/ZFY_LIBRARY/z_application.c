/**
  ******************************************************************************
	*
  * File Name          : z_application.c
  * Description        : 应用相关函数
  * author						 : 张公子
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "zfy_library.h"


/*******************************************************************************
	* @name:						Historical_Differences_Z
	* @instructions:		历史数据比较函数
	*
	* @input:						data					:比较的数据
	*										sign					:比较数据的编号
	*
	* @output:					return_data		:ZFY_PASS		和上一次相同
	*																	 ZFY_UNPASS	和上一次不同
	*****************************************************************************/
#define		HDZ_NUM		10
ZFY_Return Historical_Differences_Z(int data, uint8_t sign)
{
		ZFY_Return return_data;
		static int save[HDZ_NUM];
	
		if(data == save[sign])		return_data = ZFY_PASS;
		else 											return_data = ZFY_UNPASS;
	
		save[sign] = data;
		
		return return_data;
}
/*sign数字说明
		0.电池比例值
		1.
		2.
		3.
		4.
		5.
		6.
		7.
		8.
		9.
*/

/*******************************************************************************
	* @name:						Array_Return_Num
	* @instructions:		读取数组和
	*
	* @input:						array				数组
	*										len					数组长度
	*
	* @output:					out_data		数组中最大值
	*****************************************************************************/
uint32_t Array_Return_Num_Z(uint16_t array[],uint16_t len)
{
		uint32_t out_data = 0;
	
		for(int i=0;i<len;i++)		out_data += array[i];
		
		return out_data;
}

/*******************************************************************************
	* @name:						Array_Return_Max
	* @instructions:		读取数组最大值
	*
	* @input:						array				数组
	*										len					数组长度
	*
	* @output:					out_data		数组中最大值
	*****************************************************************************/
uint16_t Array_Return_Max_Z(uint16_t array[],uint16_t len)
{
		uint16_t out_data = array[0];
	
		for(int i=1;i<len;i++)
		{
				if(array[i] > out_data)		out_data = array[i];
		}
		
		return out_data;
}


/*******************************************************************************
	* @name:						Array_Return_Max_Sub
	* @instructions:		读取数组最大值下标
	*
	* @input:						array				数组
	*										len					数组长度
	*
	* @output:					out_data		数组中最大值
	*****************************************************************************/
uint16_t Array_Return_Max_Sub_Z(uint16_t array[],uint16_t len)
{
		uint16_t out_data = 0;
		uint16_t max = array[0];
	
		for(int i=1;i<len;i++)
		{
				if(array[i] > max)
				{
						max = array[i];
						out_data = i;
				}
		}
		
		return out_data;
}


/*******************************************************************************
	* @name:						Array_Return_Min
	* @instructions:		读取数组最小值
	*
	* @input:						array				数组
	*										len					数组长度
	*
	* @output:					out_data		数组中最小值
	*****************************************************************************/
uint16_t Array_Return_Min_Z(uint16_t array[],uint16_t len)
{
		uint16_t out_data = array[0];
	
		for(int i=1;i<len;i++)
		{
				if(array[i] < out_data)		out_data = array[i];
		}
		
		return out_data;		
}





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
