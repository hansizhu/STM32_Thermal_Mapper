#include "vl53l0x_gen.h"


extern Vl53l0x_Data_Z_TypeDef Vl53l0x_Data_Z;

VL53L0X_RangingMeasurementData_t vl53l0x_data;//�������ṹ��


//VL53L0X ����ģʽ����
//dev:�豸I2C�����ṹ��
//mode: 0:Ĭ��;1:�߾���;2:������
VL53L0X_Error vl53l0x_set_mode(VL53L0X_Dev_t *dev,u8 mode)
{
	
	 VL53L0X_Error status = VL53L0X_ERROR_NONE;
	 uint8_t VhvSettings;
	 uint8_t PhaseCal;
	 uint32_t refSpadCount;
	 uint8_t isApertureSpads;

	 status = VL53L0X_StaticInit(dev);

	
	 status = VL53L0X_PerformRefSpadManagement(dev, &refSpadCount, &isApertureSpads);//ִ�вο�SPAD����
	 if(status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",status);
	 HAL_Delay(2);	
	 status = VL53L0X_PerformRefCalibration(dev, &VhvSettings, &PhaseCal);//Ref�ο�У׼
	 if(status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",status);
	 HAL_Delay(2);

	 status = VL53L0X_SetDeviceMode(dev,VL53L0X_DEVICEMODE_SINGLE_RANGING);//ʹ�ܵ��β���ģʽ
	 if(status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",status);
	 HAL_Delay(2);
	 status = VL53L0X_SetLimitCheckEnable(dev,VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,1);//ʹ��SIGMA��Χ���
	 if(status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",status);
	 HAL_Delay(2);
	 status = VL53L0X_SetLimitCheckEnable(dev,VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,1);//ʹ���ź����ʷ�Χ���
	 if(status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",status);
	 HAL_Delay(2);
	 status = VL53L0X_SetLimitCheckValue(dev,VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,Mode_data[mode].sigmaLimit);//�趨SIGMA��Χ
	 if(status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",status);
	 HAL_Delay(2);
	 status = VL53L0X_SetLimitCheckValue(dev,VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,Mode_data[mode].signalLimit);//�趨�ź����ʷ�Χ��Χ
	 if(status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",status);
	 HAL_Delay(2);
	 status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(dev,Mode_data[mode].timingBudget);//�趨��������ʱ��
	 if(status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",status);
	 HAL_Delay(2);
	 status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, Mode_data[mode].preRangeVcselPeriod);//�趨VCSEL��������
	 if(status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",status);
	 HAL_Delay(2);
	 status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, Mode_data[mode].finalRangeVcselPeriod);//�趨VCSEL�������ڷ�Χ

	 return status;
}	


//������ͨ����
//dev���豸I2C�����ṹ��
void vl53l0x_general_start(VL53L0X_Dev_t *dev)
{
	VL53L0X_Error Status=VL53L0X_ERROR_NONE;//����״̬

	 if(Status==VL53L0X_ERROR_NONE)
	 {

			VL53L0X_StartMeasurement(dev);		//��������
		 
			VL53L0X_ClearInterruptMask(dev,0);		//����ж����λ
		 
		  VL53L0X_GetRangingMeasurementData(dev,&vl53l0x_data);		//��ȡ������Ϣ

			Vl53l0x_Data_Z.code = vl53l0x_data.RangeStatus;
			Vl53l0x_Data_Z.distance = vl53l0x_data.RangeMilliMeter;
	 }
}




