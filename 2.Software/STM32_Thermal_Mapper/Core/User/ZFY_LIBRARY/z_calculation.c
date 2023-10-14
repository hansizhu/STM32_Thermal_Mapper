/**
  ******************************************************************************
	*
  * File Name          : z_calculation.c
  * Description        : �㷨��غ���
  * author						 : �Ź���
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "zfy_library.h"

/*******************************************************************************
	* @name:						Filtering1_Recursive_Average_Z
	* @instructions:		�㷨1_����ƽ���˲�		ƽ����͵���
	*
	* @input:						in_data:�����ֵ
	*										flag:�˲������
	* @output:					�˲�����������
	*****************************************************************************/
#define		Filtering1_NUM		10		//����ƽ������
uint16_t Filtering1_Recursive_Average_Z(uint16_t in_data,uint8_t flag)
{
		uint16_t out_data = 0;
		uint8_t i;
	
		if(flag == 1)
		{
				static uint16_t Filtering_buf1[Filtering1_NUM] = {0};		//��������
				static uint32_t filtering_sub1 = 0;											//���ݵĺ�
			
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
				static uint16_t Filtering_buf2[Filtering1_NUM] = {0};		//��������
				static uint32_t filtering_sub2 = 0;											//���ݵĺ�
			
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
	* @instructions:		�㷨2_��λ��ƽ���˲�		ɾȥ���ɸ���ߺ���͵����ݺ� ��ƽ��ֵ
	*
	* @input:						in_data:�����ֵ
	*										flag:�˲������
	* @output:					�˲�����������
	*****************************************************************************/
#define		Filtering2_NUM		20		//����ƽ������
#define		Filtering2_DEL		5			//����ɾȥ��ֵ

uint16_t Filtering2_Median_Average_Z(uint16_t in_data,uint8_t flag)
{
		uint16_t out_data = 0;
		uint8_t i,j;
	
		if(flag == 1)
		{
				static uint16_t filtering2_buf_loop1[Filtering2_NUM] = {0};		//����ѭ����¼����
				static uint16_t filtering2_buf_cal1[Filtering2_NUM] = {0};		//���ݼ�������
				static uint16_t filtering2_temp1 = 0;													//������ʱ�洢
				static uint32_t filtering2_sub1 = 0;													//���ݵĺ�
			
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
				static uint16_t filtering2_buf_loop2[Filtering2_NUM] = {0};		//����ѭ����¼����
				static uint16_t filtering2_buf_cal2[Filtering2_NUM] = {0};		//���ݼ�������
				static uint16_t filtering2_temp2 = 0;													//������ʱ�洢
				static uint32_t filtering2_sub2 = 0;													//���ݵĺ�
			
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
	* @instructions:		�㷨3_��Ȩ��λ��ƽ���˲�			ɾȥ���ɸ���ߺ���͵����ݺ� ��ƽ��ֵ����λ����Ȩ���
	*
	* @input:						in_data:�����ֵ
	*										flag:�˲������
	* @output:					�˲�����������
	*****************************************************************************/
#define		Filtering3_NUM			10		//����ƽ������
#define		Filtering3_DEL			3			//����ɾȥ��ֵ
#define		Filtering3_MED_VAL	50		//��λ��Ȩ��/100	
#define		Filtering3_AVE_VAL	50		//ƽ����Ȩ��/100	

uint16_t Filtering3_Weighting_Median_Average_Z(uint16_t in_data,uint8_t flag)
{
		uint16_t out_data = 0;
		uint8_t i,j;
	
		if(flag == 1)
		{
				static uint16_t filtering3_buf_loop1[Filtering3_NUM] = {0};		//����ѭ����¼����
				static uint16_t filtering3_buf_cal1[Filtering3_NUM] = {0};		//���ݼ�������
				static uint16_t filtering3_temp1 = 0;													//������ʱ�洢
				static uint16_t filtering3_med1 = 0;													//������λ��
				static uint16_t filtering3_ave1 = 0;													//���ݵ�ƽ����		 
				static uint32_t filtering3_sub1 = 0;													//���ݵĺ�
				
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
				static uint16_t filtering3_buf_loop2[Filtering3_NUM] = {0};		//����ѭ����¼����
				static uint16_t filtering3_buf_cal2[Filtering3_NUM] = {0};		//���ݼ�������
				static uint16_t filtering3_temp2 = 0;													//������ʱ�洢
				static uint16_t filtering3_med2 = 0;													//������λ��
				static uint16_t filtering3_ave2 = 0;													//���ݵ�ƽ����		 
				static uint32_t filtering3_sub2 = 0;													//���ݵĺ�
				
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
