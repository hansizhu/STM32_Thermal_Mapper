#ifndef __MLX90640_H
#define	__MLX90640_H

#include "main.h"

#define  FPS2HZ   0x02
#define  FPS4HZ   0x03
#define  FPS8HZ   0x04
#define  FPS16HZ  0x05
#define  FPS32HZ  0x06

#define  MLX90640_ADDR 0x33


#define	 RefreshRate FPS4HZ  //推荐用4Hz，干扰少
#define  TA_SHIFT 8          //Default shift for MLX90640 in open air

#define  TEMP_ADD		20		//温度的补偿值 /10℃


void Mlx90640_Init(void);

uint8_t Mlx90640_Get_Frame(void);

void Disp_Temp_Pic(void);
void Disp_Dlc_Mod(void);
void Disp_Temp_Ribbon(void);

uint8_t Temp_Change_Bit(int temp,int max,int min);
uint16_t Bit_Change_Colour(uint8_t value,uint8_t flag);

void Bilinear_Interpolation(uint16_t flag);

#endif
