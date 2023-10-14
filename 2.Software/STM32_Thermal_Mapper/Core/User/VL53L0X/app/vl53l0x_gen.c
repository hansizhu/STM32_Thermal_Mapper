#include "vl53l0x_gen.h"


extern Vl53l0x_Data_Z_TypeDef Vl53l0x_Data_Z;

VL53L0X_RangingMeasurementData_t vl53l0x_data;//测距测量结构体


//VL53L0X 测量模式配置
//dev:设备I2C参数结构体
//mode: 0:默认;1:高精度;2:长距离
VL53L0X_Error vl53l0x_set_mode(VL53L0X_Dev_t *dev,u8 mode)
{
	
	 VL53L0X_Error status = VL53L0X_ERROR_NONE;
	 uint8_t VhvSettings;
	 uint8_t PhaseCal;
	 uint32_t refSpadCount;
	 uint8_t isApertureSpads;

	 status = VL53L0X_StaticInit(dev);

	
	 status = VL53L0X_PerformRefSpadManagement(dev, &refSpadCount, &isApertureSpads);//执行参考SPAD管理
	 if(status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",status);
	 HAL_Delay(2);	
	 status = VL53L0X_PerformRefCalibration(dev, &VhvSettings, &PhaseCal);//Ref参考校准
	 if(status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",status);
	 HAL_Delay(2);

	 status = VL53L0X_SetDeviceMode(dev,VL53L0X_DEVICEMODE_SINGLE_RANGING);//使能单次测量模式
	 if(status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",status);
	 HAL_Delay(2);
	 status = VL53L0X_SetLimitCheckEnable(dev,VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,1);//使能SIGMA范围检查
	 if(status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",status);
	 HAL_Delay(2);
	 status = VL53L0X_SetLimitCheckEnable(dev,VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,1);//使能信号速率范围检查
	 if(status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",status);
	 HAL_Delay(2);
	 status = VL53L0X_SetLimitCheckValue(dev,VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,Mode_data[mode].sigmaLimit);//设定SIGMA范围
	 if(status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",status);
	 HAL_Delay(2);
	 status = VL53L0X_SetLimitCheckValue(dev,VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,Mode_data[mode].signalLimit);//设定信号速率范围范围
	 if(status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",status);
	 HAL_Delay(2);
	 status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(dev,Mode_data[mode].timingBudget);//设定完整测距最长时间
	 if(status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",status);
	 HAL_Delay(2);
	 status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, Mode_data[mode].preRangeVcselPeriod);//设定VCSEL脉冲周期
	 if(status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",status);
	 HAL_Delay(2);
	 status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, Mode_data[mode].finalRangeVcselPeriod);//设定VCSEL脉冲周期范围

	 return status;
}	


//启动普通测量
//dev：设备I2C参数结构体
void vl53l0x_general_start(VL53L0X_Dev_t *dev)
{
	VL53L0X_Error Status=VL53L0X_ERROR_NONE;//工作状态

	 if(Status==VL53L0X_ERROR_NONE)
	 {

			VL53L0X_StartMeasurement(dev);		//开启测量
		 
			VL53L0X_ClearInterruptMask(dev,0);		//清除中断完成位
		 
		  VL53L0X_GetRangingMeasurementData(dev,&vl53l0x_data);		//获取测量信息

			Vl53l0x_Data_Z.code = vl53l0x_data.RangeStatus;
			Vl53l0x_Data_Z.distance = vl53l0x_data.RangeMilliMeter;
	 }
}




