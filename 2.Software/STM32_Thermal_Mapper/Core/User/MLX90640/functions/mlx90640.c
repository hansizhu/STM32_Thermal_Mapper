#include "mlx90640.h"
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"

#include <zfy_library.h>

#include <stdio.h>
#include <math.h>

#include "lcd.h"
#include "vl53l0x.h"

/*
MLX90640 �Ĳ���������߿��Դﵽ 64Hz����Խ�������ʱ��������Խ�󣬵����������½����ֲ��ϸ�����ָ���� 1Hz ʱ�������ֳ� 0.1�档
���·�Χ��-40~300�档
���¾��Ⱥͳ���������йأ������м�λ���ǡ�0.5~1.0�棬����� 4 �����ǡ�2.0�棬��������Լ�ǡ�1.0�档
�������ϵ���и���ƽ��ʱ�䣬��Լ�� 5 ���ӣ�δ�ﵽ��ƽ��ʱ���Ȼ��һЩ��
*/

paramsMLX90640 mlx90640;

static uint16_t eeMLX90640[834];		//ԭʼ����
static uint16_t mlx90640To[768];		//������ԭʼ��������
static uint16_t mlx90640_disp_buf[768];		//������ʾ����

static uint16_t mlx90640_temp_buf[25];		//˫�����ڲ巨���ԭʼ����
static uint16_t mlx90640_lcd_buf[100];		//�Ŵ�󵥸�������ʾ������
static uint16_t mlx90640_line_buf[3100];	//���е���������

int mlx90640_max_temp = 0;		//����������¶�
int mlx90640_min_temp = 0;		//����������¶�
int mlx90640_med_temp = 0;		//�豸�������¶�

float mlx90640_vdd = 0;			//mlx90640�豸��ѹ
float mlx90640_temp = 0;		//mlx90640�豸�¶�

float emissivity = 0.95;

uint16_t mlx90640_status;

//const uint16_t rainbow3[] = {       
//0x001F,0x003F,0x005F,0x007F,0x009F,0x00DF,0x00FF,0x011F,
//0x013F,0x017F,0x019F,0x01BF,0x01DF,0x01FF,0x023F,0x025F,
//0x027F,0x029F,0x02DF,0x02FF,0x031F,0x033F,0x037F,0x039F,
//0x03BF,0x03DF,0x03FF,0x043F,0x045F,0x047F,0x049F,0x04DF,
//0x04FF,0x051F,0x053F,0x057F,0x059F,0x05BF,0x05DF,0x05FF,
//0x063F,0x065F,0x067F,0x069F,0x06DF,0x06FF,0x071F,0x073F,
//0x077F,0x079F,0x07BF,0x07DF,0x07FE,0x07FD,0x07FD,0x07FC,
//0x07FB,0x07FB,0x07FA,0x07FA,0x07F9,0x07F8,0x07F8,0x07F7,
//0x07F7,0x07F6,0x07F5,0x07F5,0x07F4,0x07F3,0x07F3,0x07F2,
//0x07F2,0x07F1,0x07F0,0x07F0,0x07EF,0x07EF,0x07EE,0x07ED,
//0x07ED,0x07EC,0x07EC,0x07EB,0x07EA,0x07EA,0x07E9,0x07E9,
//0x07E8,0x07E7,0x07E7,0x07E6,0x07E6,0x07E5,0x07E4,0x07E4,
//0x07E3,0x07E3,0x07E2,0x07E1,0x07E1,0x07E0,0x07E0,0x07E0,
//0x0FE0,0x0FE0,0x17E0,0x1FE0,0x1FE0,0x27E0,0x27E0,0x2FE0,
//0x37E0,0x37E0,0x3FE0,0x3FE0,0x47E0,0x4FE0,0x4FE0,0x57E0,
//0x57E0,0x5FE0,0x67E0,0x67E0,0x6FE0,0x6FE0,0x77E0,0x7FE0,
//0x7FE0,0x87E0,0x87E0,0x8FE0,0x97E0,0x97E0,0x9FE0,0x9FE0,
//0xA7E0,0xAFE0,0xAFE0,0xB7E0,0xBFE0,0xBFE0,0xC7E0,0xC7E0,
//0xCFE0,0xD7E0,0xD7E0,0xDFE0,0xDFE0,0xE7E0,0xEFE0,0xEFE0,
//0xF7E0,0xF7E0,0xFFC0,0xFFA0,0xFF80,0xFF80,0xFF60,0xFF60,
//0xFF40,0xFF20,0xFF20,0xFF00,0xFEE0,0xFEE0,0xFEC0,0xFEC0,
//0xFEA0,0xFE80,0xFE80,0xFE60,0xFE60,0xFE40,0xFE20,0xFE20,
//0xFE00,0xFDE0,0xFDE0,0xFDC0,0xFDC0,0xFDA0,0xFD80,0xFD80,
//0xFD60,0xFD40,0xFD40,0xFD20,0xFD20,0xFD00,0xFCE0,0xFCE0,
//0xFCC0,0xFCC0,0xFCA0,0xFC80,0xFC80,0xFC60,0xFC40,0xFC40,
//0xFC20,0xFC20,0xFC00,0xFBE0,0xFBE0,0xFBC0,0xFBC0,0xFBA0,
//0xFB80,0xFB80,0xFB60,0xFB40,0xFB40,0xFB20,0xFB20,0xFB00,
//0xFAE0,0xFAE0,0xFAC0,0xFAC0,0xFAA0,0xFA80,0xFA80,0xFA60,
//0xFA40,0xFA40,0xFA20,0xFA20,0xFA00,0xF9E0,0xF9E0,0xF9C0,
//0xF9C0,0xF9A0,0xF980,0xF980,0xF960,0xF960,0xF940,0xF920,
//0xF920,0xF900,0xF8E0,0xF8E0,0xF8C0,0xF8C0,0xF8A0,0xF880,
//0xF880,0xF860,0xF860,0xF840,0xF820,0xF820,0xF800,0xF800
//};

const uint16_t pseudo2[] = {  
0x0000,0x0000,0x0000,0x0001,0x0001,0x0002,0x0002,0x0003,
0x0003,0x0004,0x0004,0x0005,0x0005,0x0006,0x0006,0x0007,
0x0007,0x0008,0x0008,0x0009,0x0009,0x000A,0x000A,0x000B,
0x000B,0x000C,0x000C,0x000D,0x000D,0x000E,0x000E,0x000F,
0x000F,0x000F,0x0010,0x0010,0x0011,0x0011,0x0012,0x0012,
0x0013,0x0013,0x0014,0x0014,0x0015,0x0015,0x0016,0x0016,
0x0017,0x0017,0x0018,0x0018,0x0019,0x0019,0x001A,0x001A,
0x001B,0x001B,0x001C,0x001C,0x001D,0x001D,0x001E,0x001E,
0x001E,0x001E,0x003D,0x005D,0x007C,0x009C,0x00BB,0x00DB,
0x00FA,0x011A,0x0139,0x0159,0x0178,0x0198,0x01B7,0x01D7,
0x01F6,0x0216,0x0235,0x0255,0x0274,0x0294,0x02B3,0x02D3,
0x02F2,0x0312,0x0331,0x0351,0x0370,0x0390,0x03AF,0x03CF,
0x03EF,0x040E,0x042E,0x044D,0x046D,0x048C,0x04AC,0x04CB,
0x04EB,0x050A,0x052A,0x0549,0x0569,0x0588,0x05A8,0x05C7,
0x05E7,0x0606,0x0626,0x0645,0x0665,0x0684,0x06A4,0x06C3,
0x06E3,0x0702,0x0722,0x0741,0x0761,0x0780,0x07A0,0x07C0,
0x07E0,0x07E0,0x07E0,0x0FE0,0x0FE0,0x17E0,0x17E0,0x1FE0,
0x1FE0,0x27E0,0x27E0,0x2FE0,0x2FE0,0x37E0,0x37E0,0x3FE0,
0x3FE0,0x47E0,0x47E0,0x4FE0,0x4FE0,0x57E0,0x57E0,0x5FE0,
0x5FE0,0x67E0,0x67E0,0x6FE0,0x6FE0,0x77E0,0x77E0,0x7FE0,
0x7FE0,0x7FE0,0x87E0,0x87E0,0x8FE0,0x8FE0,0x97E0,0x97E0,
0x9FE0,0x9FE0,0xA7E0,0xA7E0,0xAFE0,0xAFE0,0xB7E0,0xB7E0,
0xBFE0,0xBFE0,0xC7E0,0xC7E0,0xCFE0,0xCFE0,0xD7E0,0xD7E0,
0xDFE0,0xDFE0,0xE7E0,0xE7E0,0xEFE0,0xEFE0,0xF7E0,0xF7E0,
0xFFC0,0xFFA0,0xFF80,0xFF60,0xFF40,0xFF20,0xFF00,0xFEE0,
0xFEC0,0xFEA0,0xFE80,0xFE60,0xFE40,0xFE20,0xFE00,0xFDE0,
0xFDC0,0xFDA0,0xFD80,0xFD60,0xFD40,0xFD20,0xFD00,0xFCE0,
0xFCC0,0xFCA0,0xFC80,0xFC60,0xFC40,0xFC20,0xFC00,0xFBE0,
0xFBC0,0xFBA0,0xFB80,0xFB60,0xFB40,0xFB20,0xFB00,0xFAE0,
0xFAC0,0xFAA0,0xFA80,0xFA60,0xFA40,0xFA20,0xFA00,0xF9E0,
0xF9C0,0xF9A0,0xF980,0xF960,0xF940,0xF920,0xF900,0xF8E0,
0xF8C0,0xF8A0,0xF880,0xF860,0xF840,0xF820,0xF800,0xF800
};

/*******************************************************************************
	* @name:						Mlx90640_Init	
	* @instructions:		MLX90640��ʼ��
	*
	* @input:						NULL
	* @output:					NULL
	*****************************************************************************/
void Mlx90640_Init(void)
{
		MLX90640_I2CInit();		//��ʼ��IO��
	
		MLX90640_SetRefreshRate(MLX90640_ADDR, RefreshRate);		//����֡��
		MLX90640_SetChessMode(MLX90640_ADDR);		//����ģʽ
	
		mlx90640_status = MLX90640_DumpEE(MLX90640_ADDR, eeMLX90640);		//��ȡ����У������ 
		if (mlx90640_status != 0) printf("load system parameters error with code:%d\r\n",mlx90640_status);
	
		mlx90640_status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);		//����У������
		if (mlx90640_status != 0) printf("Parameter extraction failed with error code:%d\r\n",mlx90640_status);

		for(int i=0;i<3;)
		{
				MLX90640_I2CRead(MLX90640_ADDR, 0x8000, 1, &mlx90640_status);

				if(mlx90640_status & 0x0008)
				{
						int status = MLX90640_GetFrameData(MLX90640_ADDR, eeMLX90640);  //��ȡһ֡ԭʼ����
						if (status < 0)		printf("GetFrame Error: %d\r\n",status);
						
						mlx90640_vdd = MLX90640_GetVdd(eeMLX90640, &mlx90640);  //���� Vdd�������п��ޣ�
						mlx90640_temp = MLX90640_GetTa(eeMLX90640, &mlx90640);  //����ʵʱ����¶�
						mlx90640_temp = mlx90640_temp - TA_SHIFT;
						MLX90640_CalculateTo(eeMLX90640,&mlx90640,emissivity,mlx90640_temp,mlx90640To);		//�������ص��¶�

						//printf("vdd: %f Tr: %f\r\n",mlx90640_vdd,mlx90640_temp);
						
						i++;
				}
		}
}


/*******************************************************************************
	* @name:						Mlx90640_Get_Frame	
	* @instructions:		MLX90640��ȡһ֡����
	*
	* @input:						NULL
	* @output:					out		״̬��־λ
	*****************************************************************************/
uint8_t Mlx90640_Get_Frame(void)
{
		uint8_t flag = 0;
		mlx90640_status = 0;
	
		MLX90640_I2CRead(MLX90640_ADDR, 0x8000, 1, &mlx90640_status);

		if(mlx90640_status & 0x0008)
		{
				int status = MLX90640_GetFrameData(MLX90640_ADDR, eeMLX90640);  //��ȡһ֡ԭʼ����
				if (status < 0)		printf("GetFrame Error: %d\r\n",status);
				
				mlx90640_vdd = MLX90640_GetVdd(eeMLX90640, &mlx90640);  //���� Vdd�������п��ޣ�
				mlx90640_temp = MLX90640_GetTa(eeMLX90640, &mlx90640);  //����ʵʱ����¶�
				mlx90640_temp = mlx90640_temp - TA_SHIFT;
				MLX90640_CalculateTo(eeMLX90640,&mlx90640,emissivity,mlx90640_temp,mlx90640To);		//�������ص��¶�

				//printf("vdd: %f Tr: %f\r\n",mlx90640_vdd,mlx90640_temp);
				
				flag = 1;
		}
		
		return flag;
}




/*******************************************************************************
	* @name:						Disp_Temp_Pic	
	* @instructions:		MLX90640��ʾһ֡����
	*
	* @input:						NULL
	* @output:					NULL
	*****************************************************************************/
void Disp_Temp_Pic(void)
{
		uint16_t count = 0;

		//���ݾ���
		for(int i=0;i<24;i++)
		{
				for(int j=0;j<32;j++)
				{
						mlx90640_disp_buf[i*32+j] = mlx90640To[i*32+31-j] + TEMP_ADD;
				}
		}
	
		//��ȡ�豸�������¶�
		mlx90640_max_temp = Array_Return_Max_Z(mlx90640_disp_buf,768);
		mlx90640_min_temp = Array_Return_Min_Z(mlx90640_disp_buf,768);
		mlx90640_med_temp = (mlx90640_disp_buf[399]+mlx90640_disp_buf[400]+mlx90640_disp_buf[431]+mlx90640_disp_buf[432])/4;
		
//		//������ʾ��ԭʼ������ʾ��
//		for(int i=0;i<24;i++)
//		{
//				for(int j=0;j<32;j++)
//				{
//						LCD_Fill(j*10,i*10,j*10+10,i*10+10,pseudo2[Temp_Change_Bit(mlx90640_disp_buf[count++]-400,mlx90640_max_temp-380,mlx90640_min_temp-450)]);
//				}
//		}
		
		//������ʾ���˲��Ż���ʾ��
		for(int i=0;i<23;i++)
		{
				for(int j=0;j<31;j++)
				{
						//˫���Բ�ֵ�㷨
						Bilinear_Interpolation(i*32+j);
						
						//������ת��Ϊ��ʾ��ɫ
						for(int k=0;k<25;k++)		mlx90640_temp_buf[k] = pseudo2[Temp_Change_Bit(mlx90640_temp_buf[k]-400,mlx90640_max_temp-390,mlx90640_min_temp-430)];
					
						//��������Ŵ�
						count = 0;
						for(int k=0;k<5;k++)
						{
								for(int l=0;l<5;l++)
								{
										mlx90640_lcd_buf[l*2+k*20] = mlx90640_temp_buf[count];
										mlx90640_lcd_buf[l*2+k*20+1] = mlx90640_temp_buf[count];
									
										mlx90640_lcd_buf[l*2+k*20+10] = mlx90640_temp_buf[count];
										mlx90640_lcd_buf[l*2+k*20+11] = mlx90640_temp_buf[count];
										count++;
								}
						}
							
						//����������������������������
						count = 0;
						for(int k=0;k<10;k++)
						{
								for(int l=0;l<10;l++)
								{
										mlx90640_line_buf[k*310+j*10+l] = mlx90640_lcd_buf[count];
										count++;
								}
								
						}
				}
				
				LCD_Fill_Buf(5,i*10+9,315,i*10+19,mlx90640_line_buf);
		}
}

/*******************************************************************************
	* @name:						Disp_Dlc_Mod	
	* @instructions:		��ʾ�������Ϣ
	*
	* @input:						NULL
	* @output:					NULL
	*****************************************************************************/
void Disp_Dlc_Mod(void)
{
		//������ʾͼƬ������¶�
		uint16_t address,x,y,sta;

		address = Array_Return_Max_Sub_Z(mlx90640_disp_buf,768);
		x = address%32*10+5;
		y = address/32*10+9;
	
		if(x == 315)		x--;
		if(y == 239)		y--;
	
		sta = x-4;
		for(int i=0;i<9;i++)
		{
				if((sta+i)>5 && (sta+i)<315)
				{
						LCD_DrawPoint(sta+i,y,WHITE);
				}
		}
	
		sta = y-4;
		for(int i=0;i<9;i++)
		{
				if((sta+i)>9 && (sta+i)<239)
				{
						LCD_DrawPoint(x,sta+i,WHITE);
				}
		}
		
		//��ʾ�������¶�		
		LCD_ShowString(8,10,"Min:",WHITE,BLACK,16,1);
		LCD_Show_Temp_Superposition(60,10,mlx90640_min_temp-400,WHITE,BLACK);
		
		LCD_ShowString(242,10,"Max:",WHITE,BLACK,16,1);
		LCD_Show_Temp_Superposition(294,10,mlx90640_max_temp-400,WHITE,BLACK);
		
		//��ʾ����
		LCD_ShowString(242,28,"Dis:",WHITE,BLACK,16,1);
		LCD_Show_Dis_Superposition(294,28,Vl53l0x_Data_Z.distance,WHITE,BLACK);

		//��ʾ��ϵ��
		LCD_ShowString(8,204,"E",WHITE,BLACK,16,1);
		LCD_ShowString(18,204,"=",WHITE,BLACK,16,1);
		LCD_Show_Vdd_Superposition(45,204,emissivity,WHITE,BLACK);
		
		//��ʾ�豸��ѹ
		LCD_ShowString(8,222,"Vdd:",WHITE,BLACK,16,1);
		LCD_Show_Vdd_Superposition(58,222,mlx90640_vdd,WHITE,BLACK);
		
		//��ʾ����Բ���¶�
		Draw_Circle(160,124,4,WHITE);
		LCD_Show_Temp_Superposition(160,222,mlx90640_med_temp-400,WHITE,BLACK);
		
		//��ʾ�豸�����¶�
		LCD_ShowString(250,222,"DT:",WHITE,BLACK,16,1);
		LCD_Show_Vdd_Superposition(294,222,mlx90640_temp,WHITE,BLACK);
}

/*******************************************************************************
	* @name:						Disp_Temp_Ribbon	
	* @instructions:		��ʾ�¶�ɫ��
	*
	* @input:						NULL
	* @output:					NULL
	*****************************************************************************/
void Disp_Temp_Ribbon(void)
{
		LCD_DrawRectangle(5,0,262,8,WHITE);
		for(int i=0;i<256;i++)		LCD_DrawLine(i+6,1,i+6,8,pseudo2[i]);
}
	
	
/*******************************************************************************
	* @name:						Bilinear_Interpolation	
	* @instructions:		˫���Բ�ֵ�㷨
	*
	* @input:						flag	λ����Ϣ
	* @output:					NULL
	*****************************************************************************/
void Bilinear_Interpolation(uint16_t flag)
{
		uint16_t scale_x = 102;		//512/5=102
		uint16_t scale_y = 102;		//512/5=102
	
		uint16_t cbufx[2],cbufy[2];
		int x,y;
	
    for (y = 0; y < 5; y++)
    {
        cbufy[0] = 512-y*scale_y;
        cbufy[1] = 512-cbufy[0];
		
        for (x = 0; x < 5; x++)
        {
						cbufx[0] = 512-x*scale_x;
						cbufx[1] = 512-cbufx[0];

						mlx90640_temp_buf[y*5+x] = (mlx90640_disp_buf[flag   ] * cbufx[0] * cbufy[0] + 
																				mlx90640_disp_buf[flag+32] * cbufx[0] * cbufy[1] + 
																				mlx90640_disp_buf[flag+1 ] * cbufx[1] * cbufy[0] + 
																				mlx90640_disp_buf[flag+33] * cbufx[1] * cbufy[1] )>>18;
        }
		}
}



/*******************************************************************************
	* @name:						Temp_Change_Bit	
	* @instructions:		�¶�ת��Ϊ���ֽ���ֵ
	*
	* @input:						temp	������¶�
	* @output:					mul		
	*****************************************************************************/
uint8_t Temp_Change_Bit(int temp,int max,int min)
{
		static float mul;
	
		if(temp > max)		temp = max;
		if(temp < min)		temp = min;

		mul = ((float)(temp - min)/(float)(max - min));
	
		return mul *255;
}

/*******************************************************************************
	* @name:						Bit_Change_Colour	
	* @instructions:		�ֽ�ת��Ϊ��ɫ
	*
	* @input:						value		�ֽ���ֵ
	*										flag		ת������
	* @output:					out			��ɫ16λ
	*****************************************************************************/
uint16_t Bit_Change_Colour(uint8_t value,uint8_t flag)
{
		uint16_t out_data;
		uint8_t colorR,colorG,colorB;
	
		switch(flag)
		{
				case 1:		//�ڰ׻�
				{
						colorR = (float)value/256*31;
						colorG = (float)value/256*63;
						colorB = (float)value/256*31;
					
						break;
				}
				case 2:		//α��ɫ2
				{
						if(value<=63)  
						{
							colorR = 0;
							colorG = 0;
							colorB = (float)value/64*31;
						}
						else if((value>=64) && (value<=127))  
						{
							colorR = 0;
							colorG = (float)(value-64)/64*63;
							colorB = (float)(127-value)/64*31;
						}
						else if((value>=128) && (value<=191))  
						{
							colorR = (float)(value-128)/64*31;
							colorG = 63;
							colorB = 0;
						}
						else if((value>=192) && (value<=255))  
						{
							colorR = 31;
							colorG = (float)(255-value)/64*63;
							colorB = 0;
						}		

						break;
				}
				case 3:		//�ʺ�2
				{
						if(value<=63)  
						{
								colorR=0;
								colorG=((float)value/64)*63;
								colorB=31;
						}
						else if((value>=64) && (value<=95))  
						{
								colorR=0;
								colorG=63;
								colorB=((float)(95-value)/32)*31;
						}
						else if((value>=96) && (value<=127))  
						{
								colorR=((float)(value-96)/32)*31;
								colorG=63;
								colorB=0;
						}
						else if((value>=128) && (value<=191))  
						{
								colorR=31;
								colorG=((float)(191-value)/64)*63;
								colorB=0;
						}
						else if((value>=192) && (value<=255))  
						{
								colorR=31;
								colorG=((float)(value-192)/64)*63;
								colorB=((float)(value-192)/64)*31;
						}
						break;

				}
				case 4:		//�ʺ�3
				{
						if(value<=51)  
						{
								colorR = 0;
								colorG = (float)(value*5)/256*63;
								colorB = 31;
						}
						else if((value>=52) && (value<=102))  
						{
								colorR = 0;
								colorG = 63;
								colorB = (float)(255-(value-51)*5)/256*31;
						}
						else if((value>=103) && (value<=153))  
						{
								colorR = (float)(5*value-510)/256*31;
								colorG = 63;
								colorB = 0;
						}
						else if((value>=154) && (value<=204))  
						{
								colorR = 31;
								colorG = (255-128*(float)(value-153)/51)/256*63;
								colorB = 0;
						}
						else if((value>=205) && (value<=255))  
						{
								colorR = 31;
								colorG = (127-127*(float)(value-204)/51)/256*63;
								colorB = 0;
						}
						break;
				}
				case 5:		//����
				if(value<=63)  
				{
						colorR = 0;
						colorG = 0;
						colorB = ((float)value/64*255)/256*31;
				}
				else if((value>=64) && (value<=95))  
				{
						colorR = ((float)(value-63)/32*127)/256*31;
						colorG = ((float)(value-63)/32*127)/256*63;
						colorB = 31;
				}
				else if((value>=96) && (value<=127))  
				{
						colorR = (((float)(value-95)/32*127)+128)/256*31;
						colorG = (((float)(value-95)/32*127)+128)/256*63;
						colorB = ((float)(127-value)/32*255)/256*31;
				}
				else if((value>=128) && (value<=191))  
				{
						colorR = 31;
						colorG = 63;
						colorB = 0;
				}
				else if((value>=192) && (value<=255))  
				{
						colorR = 31;
						colorG = 63;
						colorB = (float)(value-192)/64*255/256*31;
				}
				break;

				default: break;
		}

		out_data = (colorR<<11)|(colorG<<5)|colorB;
	
		return out_data;
		
}







