/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include <zfy_library.h>

#include "lcd.h"
#include "lcd_init.h"
#include "w25qxx.h"
#include "vl53l0x.h"
#include "mlx90640.h"
#include "bmp.h"
#include "usbd_core.h"
#include "pic.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

FATFS fs;													/* FatFs文件系统对象 */
FIL file;													/* 文件对象 */
FRESULT f_res;                    /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[]= "han si zhu\n\r";/* 写缓冲区*/  
BYTE fatfs_work[1024];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t key_down_sign = 0;		//按键按下标志位

DEVIE_STATE devie_state;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
//  MX_FATFS_Init();
//  MX_USB_DEVICE_Init();
  MX_TIM5_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */

	//LCD
	TIM3 -> CCR4 = 0;
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	TIM3 -> CCR4 = LCD_START_LIGHT;

	//W25Qxx
	W25QXX_Init();
	while(W25QXX_ReadID()!=W25Q256)
	{
			printf("w25qxx error\r\n");
			HAL_Delay(1000);
	}
	//MX_USB_DEVICE_Init();
	MX_FATFS_Init();

	//VL53L0x	 
	while(vl53l0x_init(&vl53l0x_dev))//vl53l0x初始化
	{	 
			printf("VL53L0X Error!!!\r\n"); 
			HAL_Delay(1000);
	}
	
	//MLX90640
	Mlx90640_Init();
	
	//LCD静态显示
	Disp_Temp_Ribbon();
	Disp_Bat_Frame();


	HAL_TIM_Base_Start_IT(&htim5);
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim7);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	f_res = f_mount(&fs,(TCHAR const*)USERPath,1);		//挂载Fatfs系统
	devie_state = DEVIE_RUN;
	
	printf("All Devie ok\r\n");
	
  while (1)
	{
		//vl53l0测距
		if(tim6_time1 >= 500)
		{
				tim6_time1 = 0;

				Get_Vl53l0x_Data();
		}
		
		//电池显示相关
		if(tim6_time2 >= 1000)
		{
				tim6_time2 = 0;
			
				bat_adc_scale = Bat_Adc_Change_Scale(bat_adc_val,BAT_MAX,BAT_MIN);
			
				if(Historical_Differences_Z(bat_adc_scale,0) == ZFY_UNPASS)		Disp_Bat_Scale(bat_adc_scale);
		}
		
		//mlx90640转换完成
		if(Mlx90640_Get_Frame() == 1)
		{
				if(devie_state == DEVIE_RUN)
				{
						Disp_Temp_Pic();		//屏幕显示图像
						Disp_Dlc_Mod();			//显示额外的信息
				}
		}

	
		
		//按键
		if(HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == GPIO_PIN_RESET)
		{
				key_scan_count = 0;
			
				HAL_Delay(50);
				while((HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == GPIO_PIN_RESET) && (key_down_sign == 0))
				{
						if(key_scan_count >= 20)		//按键长按
						{
								key_down_sign = 1;
							
								if(devie_state == DEVIE_RUN)		//进入调试模式
								{
										devie_state = DEVIE_DEBUG;
									
										//显示信息
										printf("挂载USB设备\r\n");
										LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
										LCD_ShowString(100,10,"Debug Mode",WHITE,BLACK,24,1);
										Disp_Bat_Frame();
										Disp_Bat_Scale(bat_adc_scale);
									
										LCD_ShowString(20,50,"Connect the computer in this state,",WHITE,BLACK,16,1);
										LCD_ShowString(25,75,"Copy the recorded pictures to the",WHITE,BLACK,16,1);
										LCD_ShowString(124,100,"computer.",WHITE,BLACK,16,1);
									
										LCD_ShowString(51,120,"Type-C",WHITE,BLACK,16,1);
										LCD_ShowPicture(50,140,50,50,gImage_usb_pic);
										for(int i=0;i<5;i++)		LCD_DrawLine(113+i*20,165,123+i*20,165,WHITE);
										LCD_ShowString(221,120,"Type-A",WHITE,BLACK,16,1);
										LCD_ShowPicture(220,140,50,50,gImage_computer_pic);
									
									
										//切换挂载的设备
										f_res = f_mount(NULL,(TCHAR const*)USERPath,1);		//取消挂载
										FATFS_UnLinkDriver(USERPath);											//注销FatFs设备
									
										MX_USB_DEVICE_Init();
								}
								else
								{
										devie_state = DEVIE_RUN;
									
										//显示信息
										printf("挂载FATFS设备\r\n");
										Disp_Temp_Ribbon();
								
										//切换挂载设备
										USBD_LL_Stop(&hUsbDeviceFS);
									 
										MX_FATFS_Init();
										f_res = f_mount(&fs,(TCHAR const*)USERPath,1);		//挂载Fatfs系统
								}
						}
				}
				if(key_down_sign == 0)		//按键单击
				{
						if(devie_state == DEVIE_RUN)		//运行模式
						{
								if(Screen_Cut_Pic() == 0)		printf("图片写入成功\r\n");
								else												printf("图片写入失败\r\n");
						}
					

				}
		}
		else if(key_down_sign == 1 && (HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == GPIO_PIN_SET))		//按键释放
		{
				key_down_sign = 0;
		}
		
		


		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
