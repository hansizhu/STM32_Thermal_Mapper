/**
  ******************************************************************************
	*
  * File Name          : z_system.c
  * Description        : 系统相关函数
  * author						 : 张公子
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "zfy_library.h"


/*******************************************************************************
	* @name:						Delay_us_Z
	* @instructions:		微秒延时
	*
	* @input:						delay					:延时时间
	*
	* @output:					NULL
	*****************************************************************************/
	#define CPU_FREQUENCY_MHZ    (SystemCoreClock/1000000)
void Delay_us_Z(__IO uint32_t delay)
{
    int last, curr, val;
    int temp;

    while (delay != 0)
    {
        temp = delay > 900 ? 900 : delay;
        last = SysTick->VAL;
        curr = last - CPU_FREQUENCY_MHZ * temp;
        if (curr >= 0)
        {
            do
            {
                val = SysTick->VAL;
            }
            while ((val < last) && (val >= curr));
        }
        else
        {
            curr += CPU_FREQUENCY_MHZ * 1000;
            do
            {
                val = SysTick->VAL;
            }
            while ((val <= last) || (val > curr));
        }
        delay -= temp;
    }
}

/*******************************************************************************
	* @name:						APP_Jump
	* @instructions:		跳转至指定内存运行
	*
	* @input:						app_address		程序跳转的位置(0x08020000)
	* @output:					NULL
	*****************************************************************************/
typedef  void (*iapfun)(void);													//定义一个函数类型的参数.
iapfun jump2app; 
void APP_Jump_Z(uint32_t appxaddr)
{
	if(((*(uint32_t*)appxaddr)&0x2FFE0000)==0x20000000)		//检查栈顶地址是否合法.
	{
		jump2app=(iapfun)*(uint32_t*)(appxaddr+4);	//用户代码区第二个字为程序开始地址(复位地址)                
		__set_MSP(*(uint32_t*)appxaddr);						//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
//		SCB->VTOR=appxaddr;													//中断向量表偏移
		__disable_irq();														//关闭所有中断
		jump2app();																	//跳转到APP.
					
	}
}

/*******************************************************************************
	* @name:						APP_Jump_Run
	* @instructions:		程序跳转后运行
	*
	* @input:						app_address		程序跳转的位置(0x08020000)
	* @output:					NULL
	*****************************************************************************/
void APP_Jump_Run_Z(uint32_t app_address)
{
//		SCB->VTOR = app_address;
		__enable_irq();
}










/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
