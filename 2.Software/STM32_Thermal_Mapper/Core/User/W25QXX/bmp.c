#include "bmp.h"
#include "ff.h"
#include "lcd.h"

#include <string.h>

uint16_t bmp_data_buf[640];				//数据缓存区地址

//BMP编码函数
//将当前LCD屏幕的指定区域截图,存为16位格式的BMP文件 RGB565格式.
//保存为rgb565则需要掩码,需要利用原来的调色板位置增加掩码.这里我们已经增加了掩码.
//保存为rgb555格式则需要颜色转换,耗时间比较久,所以保存为565是最快速的办法.
//filename:存放路径
//x,y:在屏幕上的起始坐标  
//mode:模式.0,仅仅创建新文件的方式编码;1,如果之前存在文件,则覆盖之前的文件.如果没有,则创建新的文件.
//返回值:0,成功;其他,错误码.

uint8_t Screen_Cut_Pic(void)
{				
	FIL f_bmp;
	uint32_t bw = 0;
	uint16_t bmpheadsize;			//bmp头大小	   	
 	BITMAPINFO hbmp;					//bmp头	 
	uint8_t res = 0;
	
	int16_t tx,ty;				   	//图像尺寸   
 	char fname[8] = "000.bmp";
	char show_save_pic[24] = "Save Picture...";

	bmpheadsize=sizeof(hbmp);												//得到bmp文件头的大小      
	hbmp.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);	//信息头大小
	hbmp.bmiHeader.biWidth=320;											//bmp的宽度
	hbmp.bmiHeader.biHeight=240;										//bmp的高度
	hbmp.bmiHeader.biPlanes=1;											//恒为1
	hbmp.bmiHeader.biBitCount=16;										//bmp为16位色bmp
	hbmp.bmiHeader.biCompression=BI_BITFIELDS;			//每个象素的比特由指定的掩码决定。
 	hbmp.bmiHeader.biSizeImage=hbmp.bmiHeader.biHeight*hbmp.bmiHeader.biWidth*hbmp.bmiHeader.biBitCount/8;	//bmp数据区大小
 				   
	hbmp.bmfHeader.bfType=((uint16_t)'M'<<8)+'B';										//BM格式标志
	hbmp.bmfHeader.bfSize=bmpheadsize+hbmp.bmiHeader.biSizeImage;		//整个bmp的大小
  hbmp.bmfHeader.bfOffBits=bmpheadsize;														//到数据区的偏移

	hbmp.RGB_MASK[0] = 0x00F800;	 		//红色掩码
	hbmp.RGB_MASK[1] = 0x0007E0;	 		//绿色掩码
	hbmp.RGB_MASK[2] = 0x00001F;	 		//蓝色掩码

	while(1)
	{
			res=f_open(&f_bmp,(const TCHAR*)fname,FA_READ|FA_WRITE);//尝试打开之前的文件
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

	res = f_write(&f_bmp,(uint8_t*)&hbmp,bmpheadsize,&bw);//写入BMP首部  

	bmpheadsize=0;
	for(ty=239;ty>=0;ty--)
	{
			for(tx=0;tx<320;tx++)
			{
					bmp_data_buf[bmpheadsize+tx]=LCD_ReadPoint(tx,ty);		//读取坐标点的值
			}

			if((ty%2)==0 )
			{
					res=f_write(&f_bmp,(uint8_t*)bmp_data_buf,1280,&bw);//写入数据
					bmpheadsize=0;
			}
			else
			{
					bmpheadsize=320;
			}
			
			//显示存储信息
			if(ty == 223)
			{
					strcat(show_save_pic,fname);
					LCD_ShowString(5,224,(uint8_t *)show_save_pic,WHITE,BLACK,16,0);
			}
	}


	f_close(&f_bmp);
    
	return res;
}


