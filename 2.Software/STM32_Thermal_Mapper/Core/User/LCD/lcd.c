#include "lcd.h"
#include "lcd_init.h"
#include "lcdfont.h"

#include <stdlib.h>
#include <zfy_library.h>


u16 Lcd_ReadData_16Bit(void)
{
	u16 r,g,b;
	LCD_DC_Set();

	//dummy data
	LCD_RD_Clr();
	Delay_us_Z(1);
	r = DATAIN;
	LCD_RD_Set();
	
	//8bit:red data
	LCD_RD_Clr();	Delay_us_Z(1);
	r = DATAIN;
	LCD_RD_Set();
	
	//8bit:green data
	LCD_RD_Clr();	Delay_us_Z(1);
	g = DATAIN;
	LCD_RD_Set();
	//blue data
	LCD_RD_Clr();	Delay_us_Z(1);
	b = DATAIN;
	LCD_RD_Set();
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

/******************************************************************************
      函数说明：设置光标位置
      入口数据：x,y 光标位置
      返回值：  无
******************************************************************************/
void LCD_SetCursor(u16 x,u16 y)
{
	LCD_WR_REG(0x2a);//列地址设置
	LCD_WR_DATA8(x>>8);
	LCD_WR_DATA8(x&0xff);
	LCD_WR_REG(0x2b);//行地址设置
	LCD_WR_DATA8(y>>8);
	LCD_WR_DATA8(y&0xff);
}


u16 LCD_ReadPoint(u16 x,u16 y)
{
	u16 color;
	if(x>=LCD_W||y>=LCD_H)
	{
		return 0;	//超过了范围,直接返回	
	}
	LCD_SetCursor(x,y);//设置光标位置 
	LCD_WR_REG(0x2E);
  LCD_DATA_IN();
	color = Lcd_ReadData_16Bit();
  LCD_DATA_OUT(); 
	return color;
}



/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//设置显示范围
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			LCD_WR_DATA(color);
		}
	}
}

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_Address_Set(x,y,x,y);//设置光标位置 
	LCD_WR_DATA(color);
} 


/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   线的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);//画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}


/******************************************************************************
      函数说明：画矩形
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   矩形的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2+1,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}


/******************************************************************************
      函数说明：画圆
      入口数据：x0,y0   圆心坐标
                r       半径
                color   圆的颜色
      返回值：  无
******************************************************************************/
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//判断要画的点是否过远
		{
			b--;
		}
	}
}

/******************************************************************************
      函数说明：显示汉字串
      入口数据：x,y显示坐标
                *s 要显示的汉字串
                fc 字的颜色
                bc 字的背景色
                sizey 字号 可选 16 24 32
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	while(*s!=0)
	{
		if(sizey==16) LCD_ShowChinese16x16(x,y,s,fc,bc,sizey,mode);
		else if(sizey==24) LCD_ShowChinese24x24(x,y,s,fc,bc,sizey,mode);
		else if(sizey==32) LCD_ShowChinese32x32(x,y,s,fc,bc,sizey,mode);
		else return;
		s+=2;
		x+=sizey;
	}
}

/******************************************************************************
      函数说明：显示单个16x16汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese16x16(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;//汉字数目
	u16 TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
	TypefaceNum=sizey/8*sizey;//此算法只适用于字宽等于字高，且字高是8的倍数的字，
	                          //也建议用户使用这样大小的字,否则显示容易出问题！
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont16[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
					}
					else//叠加方式
					{
						if(tfont16[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 


/******************************************************************************
      函数说明：显示单个24x24汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese24x24(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;//汉字数目
	u16 TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
	TypefaceNum=sizey/8*sizey;//此算法只适用于字宽等于字高，且字高是8的倍数的字，
	                          //也建议用户使用这样大小的字,否则显示容易出问题！
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont24[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
					}
					else//叠加方式
					{
						if(tfont24[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 

/******************************************************************************
      函数说明：显示单个32x32汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese32x32(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;//汉字数目
	u16 TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
	TypefaceNum=sizey/8*sizey;//此算法只适用于字宽等于字高，且字高是8的倍数的字，
	                          //也建议用户使用这样大小的字,否则显示容易出问题！
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont32[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
					}
					else//叠加方式
					{
						if(tfont32[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
}


/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u32 temp,sizex,t,m=0;
	u32 i,TypefaceNum;//一个字符所占字节大小
	u32 x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    //得到偏移后的值
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //设置光标位置 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12)temp=ascii_1206[num][i];		       //调用6x12字体
		else if(sizey==16)temp=ascii_1608[num][i];		 //调用8x16字体
		else if(sizey==24)temp=ascii_2412[num][i];		 //调用12x24字体
		else if(sizey==32)temp=ascii_3216[num][i];		 //调用16x32字体
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)//非叠加模式
			{
				if(temp&(0x01<<t))LCD_WR_DATA(fc);
				else LCD_WR_DATA(bc);
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else//叠加模式
			{
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//画一个点
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}   	 	 	  
}


/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode)
{         
	while(*p!='\0')
	{       
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}


/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;
	return result;
}


/******************************************************************************
      函数说明：显示整数变量
      入口数据：x,y显示坐标
                num 要显示整数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp;
	u8 enshow=0;
	u8 sizex=sizey/2;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
} 


/******************************************************************************
      函数说明：显示两位小数变量
      入口数据：x,y显示坐标
                num 要显示小数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp,sizex;
	u16 num1;
	sizex=sizey/2;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
}


/******************************************************************************
      函数说明：显示图片
      入口数据：x,y起点坐标
                length 图片长度
                width  图片宽度
                pic[]  图片数组    
      返回值：  无
******************************************************************************/
void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[])
{
	u16 i,j,k=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			LCD_WR_DATA8(pic[k*2]);
			LCD_WR_DATA8(pic[k*2+1]);
			k++;
		}
	}			
}






/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_Fill_Buf(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color[])
{          
	u16 i,j;
	u16 count = 0;
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//设置显示范围
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			LCD_WR_DATA(color[count]);
			count++;
			
		}
	}
}


/******************************************************************************
      函数说明：叠加居中显示温度
      入口数据：xsta,ysta   起始坐标
								num					变量
								fc 字的颜色
								bc 字的背景色
								sizey 字号
      返回值：  无
******************************************************************************/
void LCD_Show_Temp_Superposition(u16 x,u16 y,int temp,u16 fc,u16 bc)
{
		if(temp > 999)		//100度以上
		{
				uint8_t buf[6];
			
				buf[0] = temp/1000%10+0x30;
				buf[1] = temp/100%10+0x30;
				buf[2] = temp/10%10+0x30;
				buf[3] = 0x2E;
				buf[4] = temp%10+0x30;
				buf[5] = 0;
			
				LCD_ShowString(x-20,y,buf,fc,bc,16,1);				
		}
		else if(temp > 99)		//10度以上
		{
				uint8_t buf[5];
			
				buf[0] = temp/100%10+0x30;
				buf[1] = temp/10%10+0x30;
				buf[2] = 0x2E;
				buf[3] = temp%10+0x30;
				buf[4] = 0;
			
				LCD_ShowString(x-16,y,buf,fc,bc,16,1);
		}
		else if(temp >= 0)		//0-10度
		{
				uint8_t buf[4];
			
				buf[0] = temp/10%10+0x30;
				buf[1] = 0x2E;
				buf[2] = temp%10+0x30;
				buf[3] = 0;
			
				LCD_ShowString(x-12,y,buf,fc,bc,16,1);
		}
		else if(temp > -99)		//-10-0度
		{
				temp = abs(temp);
				uint8_t buf[5];
			
				buf[0] = 0x2D;
				buf[1] = temp/10%10+0x30;
				buf[2] = 0x2E;
				buf[3] = temp%10+0x30;
				buf[4] = 0;
			
				LCD_ShowString(x-16,y,buf,fc,bc,16,1);
		}
		else		//-10度以下
		{
				temp = abs(temp);
				uint8_t buf[6];
			
				buf[0] = 0x2D;
				buf[1] = temp/100%10+0x30;
				buf[2] = temp/10%10+0x30;
				buf[3] = 0x2E;
				buf[4] = temp%10+0x30;
				buf[5] = 0;
			
				LCD_ShowString(x-20,y,buf,fc,bc,16,1);
		}
}

/******************************************************************************
      函数说明：叠加距离
      入口数据：xsta,ysta   起始坐标
								num					变量
								fc 字的颜色
								bc 字的背景色
								sizey 字号
      返回值：  无
******************************************************************************/
void LCD_Show_Dis_Superposition(u16 x,u16 y,u16 dis,u16 fc,u16 bc)
{
		if(dis < 56)		//50毫米以下
		{
				uint8_t buf[4];
			
				buf[0] = 0x3C;
				buf[1] = 0x35;
				buf[2] = 0x30;
				buf[3] = 0;
			
				LCD_ShowString(x-12,y,buf,fc,bc,16,1);
		}
		else if(dis > 2000)		//2000毫米以上
		{
				uint8_t buf[6];
			
				buf[0] = 0x45;
				buf[1] = 0x72;
				buf[2] = 0x72;
				buf[3] = 0x6F;
				buf[4] = 0x72;
				buf[5] = 0;
			
				LCD_ShowString(x-20,y,buf,fc,bc,16,1);	
		}
		else if(dis > 999)		//1000毫米以上
		{
				uint8_t buf[5];
				dis -=6;
			
				buf[0] = dis/1000%10+0x30;
				buf[1] = dis/100%10+0x30;
				buf[2] = dis/10%10+0x30;
				buf[3] = dis%10+0x30;
				buf[4] = 0;
			
				LCD_ShowString(x-16,y,buf,fc,bc,16,1);
		}
		else if(dis > 99)		//100毫米以上
		{
				uint8_t buf[4];
				dis -=6;
			
				buf[0] = dis/100%10+0x30;
				buf[1] = dis/10%10+0x30;
				buf[2] = dis%10+0x30;
				buf[3] = 0;
			
				LCD_ShowString(x-12,y,buf,fc,bc,16,1);
		}
		else if(dis > 9)		//100毫米以上
		{
				uint8_t buf[3];
				dis -=6;
			
				buf[0] = dis/10%10+0x30;
				buf[1] = dis%10+0x30;
				buf[2] = 0;
			
				LCD_ShowString(x-8,y,buf,fc,bc,16,1);
		}
}

/******************************************************************************
      函数说明：叠加居中显示电压
      入口数据：xsta,ysta   起始坐标
								num					变量
								fc 字的颜色
								bc 字的背景色
								sizey 字号
      返回值：  无
******************************************************************************/
void LCD_Show_Vdd_Superposition(u16 x,u16 y,float vdd,u16 fc,u16 bc)
{
		uint16_t num = vdd*100;
	
		if(num > 999)		//十位
		{
				uint8_t buf[6];
			
				buf[0] = num/1000%10+0x30;
				buf[1] = num/100%10+0x30;
				buf[2] = 0x2E;
				buf[3] = num/10%10+0x30;
				buf[4] = num%10+0x30;
				buf[5] = 0;
			
				LCD_ShowString(x-20,y,buf,fc,bc,16,1);
		}
		else if(num > 99)		//十位
		{
				uint8_t buf[5];
			
				buf[0] = num/100%10+0x30;
				buf[1] = 0x2E;
				buf[2] = num/10%10+0x30;
				buf[3] = num%10+0x30;
				buf[4] = 0;
			
				LCD_ShowString(x-16,y,buf,fc,bc,16,1);
		}
		else if(num > 9)		//零点
		{
				uint8_t buf[5];
			
				buf[0] = 0x30;
				buf[1] = 0x2E;
				buf[2] = num/10%10+0x30;
				buf[3] = num%10+0x30;
				buf[4] = 0;
			
				LCD_ShowString(x-16,y,buf,fc,bc,16,1);
		}
}









