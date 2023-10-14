/**
  ******************************************************************************
	*
  * File Name          : z_calculation.c
  * Description        : 算法相关函数
  * author						 : 张公子
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "zfy_library.h"

/*******************************************************************************
	* @name:						Filtering1_Recursive_Average_Z
	* @instructions:		算法1_递推平均滤波		平均求和递推
	*
	* @input:						in_data:输入得值
	*										flag:滤波的序号
	* @output:					滤波计算后的数据
	*****************************************************************************/
#define		Filtering1_NUM		10		//递推平均数量
uint16_t Filtering1_Recursive_Average_Z(uint16_t in_data,uint8_t flag)
{
		uint16_t out_data = 0;
		uint8_t i;
	
		if(flag == 1)
		{
				static uint16_t Filtering_buf1[Filtering1_NUM] = {0};		//数据数组
				static uint32_t filtering_sub1 = 0;											//数据的和
			
				for(i=0; i < Filtering1_NUM-1; i++) 
				{
						Filtering_buf1[i] = Filtering_buf1[i+1];
						filtering_sub1 += Filtering_buf1[i];
				}
				Filtering_buf1[Filtering1_NUM - 1] = in_data;
				filtering_sub1 += in_data;
				
				out_data = filtering_sub1/Filtering1_NUM;
				
				filtering_sub1 = 0;
		}
		else if(flag == 2)
		{
				static uint16_t Filtering_buf2[Filtering1_NUM] = {0};		//数据数组
				static uint32_t filtering_sub2 = 0;											//数据的和
			
				for(i=0; i < Filtering1_NUM-1; i++) 
				{
						Filtering_buf2[i] = Filtering_buf2[i+1];
						filtering_sub2 += Filtering_buf2[i];
				}
				Filtering_buf2[Filtering1_NUM - 1] = in_data;
				filtering_sub2 += in_data;
				
				out_data = filtering_sub2/Filtering1_NUM;
				
				filtering_sub2 = 0;		
		}
		
		return out_data;	
}

/*******************************************************************************
	* @name:						Filtering2_Median_Average_Z
	* @instructions:		算法2_中位数平均滤波		删去若干个最高和最低的数据后 求平均值
	*
	* @input:						in_data:输入得值
	*										flag:滤波的序号
	* @output:					滤波计算后的数据
	*****************************************************************************/
#define		Filtering2_NUM		20		//递推平均数量
#define		Filtering2_DEL		5			//单侧删去得值

uint16_t Filtering2_Median_Average_Z(uint16_t in_data,uint8_t flag)
{
		uint16_t out_data = 0;
		uint8_t i,j;
	
		if(flag == 1)
		{
				static uint16_t filtering2_buf_loop1[Filtering2_NUM] = {0};		//数据循环记录数组
				static uint16_t filtering2_buf_cal1[Filtering2_NUM] = {0};		//数据计算数组
				static uint16_t filtering2_temp1 = 0;													//数据临时存储
				static uint32_t filtering2_sub1 = 0;													//数据的和
			
				for(i=0; i < Filtering2_NUM-1; i++) 
				{
						filtering2_buf_loop1[i] = filtering2_buf_loop1[i+1];
						filtering2_buf_cal1[i] = filtering2_buf_loop1[i];
				}
				filtering2_buf_loop1[Filtering2_NUM - 1] = in_data;
				filtering2_buf_cal1[Filtering2_NUM - 1] = in_data;
				
				
				for(i=0; i < Filtering2_NUM-1; i++)
				{
						for(j=0;j<Filtering2_NUM-1;j++)
						{
								if(filtering2_buf_cal1[j] > filtering2_buf_cal1[j+1])
								{
										filtering2_temp1 = filtering2_buf_cal1[j+1];
										filtering2_buf_cal1[j+1] = filtering2_buf_cal1[j];
										filtering2_buf_cal1[j] = filtering2_temp1;
								}
						}
				}
				
				for(i=Filtering2_DEL;i<(Filtering2_NUM-Filtering2_DEL);i++)		filtering2_sub1 += filtering2_buf_cal1[i];
				
				out_data = filtering2_sub1/(Filtering2_NUM-2*Filtering2_DEL);
				
				filtering2_sub1 = 0;
		}
		else if(flag == 2)
		{
				static uint16_t filtering2_buf_loop2[Filtering2_NUM] = {0};		//数据循环记录数组
				static uint16_t filtering2_buf_cal2[Filtering2_NUM] = {0};		//数据计算数组
				static uint16_t filtering2_temp2 = 0;													//数据临时存储
				static uint32_t filtering2_sub2 = 0;													//数据的和
			
				for(i=0; i < Filtering2_NUM-1; i++) 
				{
						filtering2_buf_loop2[i] = filtering2_buf_loop2[i+1];
						filtering2_buf_cal2[i] = filtering2_buf_loop2[i];
				}
				filtering2_buf_loop2[Filtering2_NUM - 1] = in_data;
				filtering2_buf_cal2[Filtering2_NUM - 1] = in_data;
				
				
				for(i=0; i < Filtering2_NUM-1; i++)
				{
						for(j=0;j<Filtering2_NUM-1;j++)
						{
								if(filtering2_buf_cal2[j] > filtering2_buf_cal2[j+1])
								{
										filtering2_temp2 = filtering2_buf_cal2[j+1];
										filtering2_buf_cal2[j+1] = filtering2_buf_cal2[j];
										filtering2_buf_cal2[j] = filtering2_temp2;
								}
						}
				}
				
				for(i=Filtering2_DEL;i<(Filtering2_NUM-Filtering2_DEL);i++)		filtering2_sub2 += filtering2_buf_cal2[i];
				
				out_data = filtering2_sub2/(Filtering2_NUM-2*Filtering2_DEL);
				
				filtering2_sub2 = 0;		
		}
		
		return out_data;	
}


/*******************************************************************************
	* @name:						Filtering3_Weighting_Median_Average_Z
	* @instructions:		算法3_加权中位数平均滤波			删去若干个最高和最低的数据后 将平均值和中位数加权求和
	*
	* @input:						in_data:输入得值
	*										flag:滤波的序号
	* @output:					滤波计算后的数据
	*****************************************************************************/
#define		Filtering3_NUM			10		//递推平均数量
#define		Filtering3_DEL			3			//单侧删去得值
#define		Filtering3_MED_VAL	50		//中位数权重/100	
#define		Filtering3_AVE_VAL	50		//平均数权重/100	

uint16_t Filtering3_Weighting_Median_Average_Z(uint16_t in_data,uint8_t flag)
{
		uint16_t out_data = 0;
		uint8_t i,j;
	
		if(flag == 1)
		{
				static uint16_t filtering3_buf_loop1[Filtering3_NUM] = {0};		//数据循环记录数组
				static uint16_t filtering3_buf_cal1[Filtering3_NUM] = {0};		//数据计算数组
				static uint16_t filtering3_temp1 = 0;													//数据临时存储
				static uint16_t filtering3_med1 = 0;													//数据中位数
				static uint16_t filtering3_ave1 = 0;													//数据的平均数		 
				static uint32_t filtering3_sub1 = 0;													//数据的和
				
				for(i=0; i < Filtering3_NUM-1; i++) 
				{
						filtering3_buf_loop1[i] = filtering3_buf_loop1[i+1];
						filtering3_buf_cal1[i] = filtering3_buf_loop1[i];
				}
				filtering3_buf_loop1[Filtering3_NUM - 1] = in_data;
				filtering3_buf_cal1[Filtering3_NUM - 1] = in_data;
				
				
				for(i=0; i < Filtering3_NUM-1; i++)
				{
						for(j=0;j<Filtering3_NUM-1;j++)
						{
								if(filtering3_buf_cal1[j] > filtering3_buf_cal1[j+1])
								{
										filtering3_temp1 = filtering3_buf_cal1[j+1];
										filtering3_buf_cal1[j+1] = filtering3_buf_cal1[j];
										filtering3_buf_cal1[j] = filtering3_temp1;
								}
						}
				}
				
				for(i=Filtering3_DEL;i<(Filtering3_NUM-Filtering3_DEL);i++)		filtering3_sub1 += filtering3_buf_cal1[i];
				filtering3_ave1 = filtering3_sub1/(Filtering3_NUM-2*Filtering3_DEL);
				
				filtering3_med1 = filtering3_buf_cal1[Filtering3_NUM/2];
				
				out_data = (filtering3_ave1*Filtering3_MED_VAL + filtering3_med1*Filtering3_MED_VAL)/100;
				
				filtering3_sub1 = 0;
		}
		else if(flag == 2)
		{
				static uint16_t filtering3_buf_loop2[Filtering3_NUM] = {0};		//数据循环记录数组
				static uint16_t filtering3_buf_cal2[Filtering3_NUM] = {0};		//数据计算数组
				static uint16_t filtering3_temp2 = 0;													//数据临时存储
				static uint16_t filtering3_med2 = 0;													//数据中位数
				static uint16_t filtering3_ave2 = 0;													//数据的平均数		 
				static uint32_t filtering3_sub2 = 0;													//数据的和
				
				for(i=0; i < Filtering3_NUM-1; i++) 
				{
						filtering3_buf_loop2[i] = filtering3_buf_loop2[i+1];
						filtering3_buf_cal2[i] = filtering3_buf_loop2[i];
				}
				filtering3_buf_loop2[Filtering3_NUM - 1] = in_data;
				filtering3_buf_cal2[Filtering3_NUM - 1] = in_data;
				
				
				for(i=0; i < Filtering3_NUM-1; i++)
				{
						for(j=0;j<Filtering3_NUM-1;j++)
						{
								if(filtering3_buf_cal2[j] > filtering3_buf_cal2[j+1])
								{
										filtering3_temp2 = filtering3_buf_cal2[j+1];
										filtering3_buf_cal2[j+1] = filtering3_buf_cal2[j];
										filtering3_buf_cal2[j] = filtering3_temp2;
								}
						}
				}
				
				for(i=Filtering3_DEL;i<(Filtering3_NUM-Filtering3_DEL);i++)		filtering3_sub2 += filtering3_buf_cal2[i];
				filtering3_ave2 = filtering3_sub2/(Filtering3_NUM-2*Filtering3_DEL);
				
				filtering3_med2 = filtering3_buf_cal2[Filtering3_NUM/2];
				
				out_data = (filtering3_ave2*Filtering3_MED_VAL + filtering3_med2*Filtering3_MED_VAL)/100;
				
				filtering3_sub2 = 0;
		}
		
		return out_data;	
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
