/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */

#include "lcd.h"

volatile uint16_t adc_raw_buf[1];

uint16_t bat_adc_val = 0;
int bat_adc_scale = 0;

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)adc_raw_buf,1);
	
  /* USER CODE END ADC1_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PC0     ------> ADC1_IN10
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA2_Stream0;
    hdma_adc1.Init.Channel = DMA_CHANNEL_0;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
    hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PC0     ------> ADC1_IN10
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0);

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/*******************************************************************************
	* @name:						Disp_Bat_Frame
	* @instructions:		显示电池框架
	*
	* @input:						NULL
	* @output:					NULL
	*****************************************************************************/
void Disp_Bat_Frame(void)
{
		LCD_ShowString(267,0,"XX",WHITE,BLACK,12,1);
		LCD_ShowString(281,0,"%",WHITE,BLACK,12,1);
	
		LCD_Fill(292,2,294,6,WHITE);
		LCD_DrawRectangle(294,0,314,7,WHITE);
}

/*******************************************************************************
	* @name:						Bat_Adc_Change_Scale
	* @instructions:		电池ADC转换成比例
	*
	* @input:						adc		电池的ADC
	*										max		ADC最大值
	*										min		ADC最小值
	* @output:					电池容量比例 -1错误
	*****************************************************************************/
int Bat_Adc_Change_Scale(uint16_t adc,uint16_t max,uint16_t min)
{
		int out_data = -1;
	
		if(adc >= min)
		{
				out_data = (adc-min)*100/(max-min);
		}
		
		if(out_data > 99)		out_data = 99;
		
		return out_data;
}

/*******************************************************************************
	* @name:						Disp_Bat_Scale
	* @instructions:		显示电池的比率
	*
	* @input:						bat 电池百分比
	* @output:					NULL
	*****************************************************************************/
void Disp_Bat_Scale(int bat)
{
		if(bat > 0)		//有效值
		{
				if(bat > 9)		//大于10%
				{
						//判断显示的颜色
						uint16_t disp_color = 0;
						if(bat > 19)		disp_color = WHITE;
						else											disp_color = RED;
					
						//显示值
						LCD_Fill(267,0,279,11,BLACK);
						uint8_t buf[3];
						buf[0] = bat/10%10 + 0x30;
						buf[1] = bat%10 + 0x30;
						buf[2] = 0;
						LCD_ShowString(267,0,buf,disp_color,BLACK,12,1);
					
						//显示电池比例
						LCD_Fill(295,1,314,7,BLACK);
						for(int i=296;i<297+bat*17/100;i++)
						{
								LCD_DrawLine(i,2,i,6,disp_color);
						}
				}
				else		//小于10%
				{
						//显示值
						LCD_Fill(267,0,279,11,BLACK);
						uint8_t buf[2];
						buf[0] = bat%10 + 0x30;
						buf[1] = 0;
						LCD_ShowString(270,0,buf,RED,BLACK,12,1);
					
						//显示电池比例
						LCD_Fill(295,1,314,7,BLACK);
						for(int i=296;i<297+bat*17/100;i++)
						{
								LCD_DrawLine(i,2,i,6,RED);
						}
				}
		}
		else
		{
				LCD_Fill(267,0,279,11,BLACK);
				LCD_ShowString(267,0,"XX",RED,BLACK,12,1);
			
				LCD_Fill(295,1,314,7,BLACK);
				LCD_DrawLine(295,1,314,7,RED);
				LCD_DrawLine(313,1,295,7,RED);
		}
}


/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
