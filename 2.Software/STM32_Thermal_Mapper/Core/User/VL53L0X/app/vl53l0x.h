#ifndef __VL53L0X_H
#define __VL53L0X_H

#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_gen.h"
#include "main.h"
#include "usart.h"

//VL53L0X传感器上电默认IIC地址为0X52(不包含最低位)
#define VL53L0X_Addr 0x52

//控制Xshut电平,从而使能VL53L0X工作 1:使能 0:关闭
#define VL53L0X_Xshut PAout(15)	

//使能2.8V IO电平模式
#define USE_I2C_2V8  0

//测量模式
#define Default_Mode   0// 默认
#define HIGH_ACCURACY  1//高精度
#define LONG_RANGE     2//长距离
#define HIGH_SPEED     3//高速

//vl53l0x模式配置参数集
typedef __packed struct
{
	FixPoint1616_t signalLimit;    //Signal极限数值 
	FixPoint1616_t sigmaLimit;     //Sigmal极限数值
	uint32_t timingBudget;         //采样时间周期
	uint8_t preRangeVcselPeriod ;  //VCSEL脉冲周期
	uint8_t finalRangeVcselPeriod ;//VCSEL脉冲周期范围
	
}mode_data;

extern mode_data Mode_data[];

VL53L0X_Error vl53l0x_init(VL53L0X_Dev_t *dev);//初始化vl53l0x
void print_pal_error(VL53L0X_Error Status);//错误信息打印
void vl53l0x_test(void);//vl53l0x测试
void vl53l0x_reset(VL53L0X_Dev_t *dev);//vl53l0x复位

void vl53l0x_info(void);//获取vl53l0x设备ID信息



void Get_Vl53l0x_Data(void);

//最终数据
typedef struct
{
	volatile uint8_t code;
	volatile uint16_t distance;
	
}	Vl53l0x_Data_Z_TypeDef;
extern Vl53l0x_Data_Z_TypeDef Vl53l0x_Data_Z;;

extern VL53L0X_Dev_t vl53l0x_dev;


#endif


