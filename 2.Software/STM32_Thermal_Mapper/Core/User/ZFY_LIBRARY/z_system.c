/**
  ******************************************************************************
	*
  * File Name          : z_system.c
  * Description        : ϵͳ��غ���
  * author						 : �Ź���
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "zfy_library.h"


/*******************************************************************************
	* @name:						Delay_us_Z
	* @instructions:		΢����ʱ
	*
	* @input:						delay					:��ʱʱ��
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
	* @instructions:		��ת��ָ���ڴ�����
	*
	* @input:						app_address		������ת��λ��(0x08020000)
	* @output:					NULL
	*****************************************************************************/
typedef  void (*iapfun)(void);													//����һ���������͵Ĳ���.
iapfun jump2app; 
void APP_Jump_Z(uint32_t appxaddr)
{
	if(((*(uint32_t*)appxaddr)&0x2FFE0000)==0x20000000)		//���ջ����ַ�Ƿ�Ϸ�.
	{
		jump2app=(iapfun)*(uint32_t*)(appxaddr+4);	//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)                
		__set_MSP(*(uint32_t*)appxaddr);						//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
//		SCB->VTOR=appxaddr;													//�ж�������ƫ��
		__disable_irq();														//�ر������ж�
		jump2app();																	//��ת��APP.
					
	}
}

/*******************************************************************************
	* @name:						APP_Jump_Run
	* @instructions:		������ת������
	*
	* @input:						app_address		������ת��λ��(0x08020000)
	* @output:					NULL
	*****************************************************************************/
void APP_Jump_Run_Z(uint32_t app_address)
{
//		SCB->VTOR = app_address;
		__enable_irq();
}










/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
