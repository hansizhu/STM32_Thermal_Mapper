#include "bmp.h"
#include "ff.h"
#include "lcd.h"

#include <string.h>

uint16_t bmp_data_buf[640];				//���ݻ�������ַ

//BMP���뺯��
//����ǰLCD��Ļ��ָ�������ͼ,��Ϊ16λ��ʽ��BMP�ļ� RGB565��ʽ.
//����Ϊrgb565����Ҫ����,��Ҫ����ԭ���ĵ�ɫ��λ����������.���������Ѿ�����������.
//����Ϊrgb555��ʽ����Ҫ��ɫת��,��ʱ��ȽϾ�,���Ա���Ϊ565������ٵİ취.
//filename:���·��
//x,y:����Ļ�ϵ���ʼ����  
//mode:ģʽ.0,�����������ļ��ķ�ʽ����;1,���֮ǰ�����ļ�,�򸲸�֮ǰ���ļ�.���û��,�򴴽��µ��ļ�.
//����ֵ:0,�ɹ�;����,������.

uint8_t Screen_Cut_Pic(void)
{				
	FIL f_bmp;
	uint32_t bw = 0;
	uint16_t bmpheadsize;			//bmpͷ��С	   	
 	BITMAPINFO hbmp;					//bmpͷ	 
	uint8_t res = 0;
	
	int16_t tx,ty;				   	//ͼ��ߴ�   
 	char fname[8] = "000.bmp";
	char show_save_pic[24] = "Save Picture...";

	bmpheadsize=sizeof(hbmp);												//�õ�bmp�ļ�ͷ�Ĵ�С      
	hbmp.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);	//��Ϣͷ��С
	hbmp.bmiHeader.biWidth=320;											//bmp�Ŀ��
	hbmp.bmiHeader.biHeight=240;										//bmp�ĸ߶�
	hbmp.bmiHeader.biPlanes=1;											//��Ϊ1
	hbmp.bmiHeader.biBitCount=16;										//bmpΪ16λɫbmp
	hbmp.bmiHeader.biCompression=BI_BITFIELDS;			//ÿ�����صı�����ָ�������������
 	hbmp.bmiHeader.biSizeImage=hbmp.bmiHeader.biHeight*hbmp.bmiHeader.biWidth*hbmp.bmiHeader.biBitCount/8;	//bmp��������С
 				   
	hbmp.bmfHeader.bfType=((uint16_t)'M'<<8)+'B';										//BM��ʽ��־
	hbmp.bmfHeader.bfSize=bmpheadsize+hbmp.bmiHeader.biSizeImage;		//����bmp�Ĵ�С
  hbmp.bmfHeader.bfOffBits=bmpheadsize;														//����������ƫ��

	hbmp.RGB_MASK[0] = 0x00F800;	 		//��ɫ����
	hbmp.RGB_MASK[1] = 0x0007E0;	 		//��ɫ����
	hbmp.RGB_MASK[2] = 0x00001F;	 		//��ɫ����

	while(1)
	{
			res=f_open(&f_bmp,(const TCHAR*)fname,FA_READ|FA_WRITE);//���Դ�֮ǰ���ļ�
			if(res==FR_OK)
			{
					f_close(&f_bmp);
					res=(fname[0]-0x30)*100+(fname[1]-0x30)*10+(fname[2]-0x30)+1;
					fname[0]=res/100+0x30;
					fname[1]=res/10+0x30;
					fname[2]=res%10+0x30;		
			}
			else
			{
					res=f_open(&f_bmp,(const TCHAR*)fname,FA_WRITE|FA_CREATE_NEW);
					if(res==FR_OK)		break;
					else							return 1;
			}
	}

	res = f_write(&f_bmp,(uint8_t*)&hbmp,bmpheadsize,&bw);//д��BMP�ײ�  

	bmpheadsize=0;
	for(ty=239;ty>=0;ty--)
	{
			for(tx=0;tx<320;tx++)
			{
					bmp_data_buf[bmpheadsize+tx]=LCD_ReadPoint(tx,ty);		//��ȡ������ֵ
			}

			if((ty%2)==0 )
			{
					res=f_write(&f_bmp,(uint8_t*)bmp_data_buf,1280,&bw);//д������
					bmpheadsize=0;
			}
			else
			{
					bmpheadsize=320;
			}
			
			//��ʾ�洢��Ϣ
			if(ty == 223)
			{
					strcat(show_save_pic,fname);
					LCD_ShowString(5,224,(uint8_t *)show_save_pic,WHITE,BLACK,16,0);
			}
	}


	f_close(&f_bmp);
    
	return res;
}


