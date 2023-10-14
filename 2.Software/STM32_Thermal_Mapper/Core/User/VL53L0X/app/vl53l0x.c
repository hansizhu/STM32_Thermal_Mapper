#include "vl53l0x.h"
#include "vl53l0x_gen.h"


Vl53l0x_Data_Z_TypeDef Vl53l0x_Data_Z;


VL53L0X_Dev_t vl53l0x_dev;//设备I2C数据参数
VL53L0X_DeviceInfo_t vl53l0x_dev_info;//设备ID版本信息


//VL53L0X各测量模式参数
//0：默认;1:高精度;2:长距离;3:高速
mode_data Mode_data[]=
{
    {(FixPoint1616_t)(0.25*65536), 
	 (FixPoint1616_t)(18*65536),
	 33000,
	 14,
	 10},//默认
		
	{(FixPoint1616_t)(0.25*65536) ,
	 (FixPoint1616_t)(18*65536),
	 200000, 
	 14,
	 10},//高精度
		
    {(FixPoint1616_t)(0.1*65536) ,
	 (FixPoint1616_t)(60*65536),
	 33000,
	 18,
	 14},//长距离
	
    {(FixPoint1616_t)(0.25*65536) ,
	 (FixPoint1616_t)(32*65536),
	 20000,
	 14,
	 10},//高速
		
};


//初始化vl53l0x
//dev:设备I2C参数结构体
VL53L0X_Error vl53l0x_init(VL53L0X_Dev_t *dev)
{
	GPIO_InitTypeDef GPIO_Initure;
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;
	VL53L0X_Dev_t *pMyDevice = dev;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();            //开启GPIOA时钟
	
	GPIO_Initure.Pin=GPIO_PIN_15;            //PA15
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP; 	 //推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;           //上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 //高速
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);

	pMyDevice->I2cDevAddr = VL53L0X_Addr;//I2C地址(上电默认0x52)
	pMyDevice->comms_type = 1;           //I2C通信模式
	pMyDevice->comms_speed_khz = 400;    //I2C通信速率
	
	VL53L0X_i2c_init();//初始化IIC总线
	
	VL53L0X_Xshut=0;//失能VL53L0X
	HAL_Delay(30);
	VL53L0X_Xshut=1;//使能VL53L0X,让传感器处于工作
	HAL_Delay(30);
	
	Status = VL53L0X_DataInit(pMyDevice);//设备初始化
	if(Status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",Status);
	HAL_Delay(2);
	Status = VL53L0X_GetDeviceInfo(pMyDevice,&vl53l0x_dev_info);//获取设备ID信息
	if(Status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",Status);
	
	vl53l0x_set_mode(dev,HIGH_ACCURACY);//配置测量模式

	return Status;
}

//----------以下函数为主函数调用------------//

void Get_Vl53l0x_Data(void)
{
		vl53l0x_general_start(&vl53l0x_dev);
	
//		switch(Vl53l0x_Data_Z.code)
//		{
//				case 0:		printf("Range Valid\r\n");		break;
//				case 1:		printf("Sigma Fail\r\n");			break;
//				case 2:		printf("Signal Fail\r\n");		break;
//				case 3:		printf("Min range\r\n");			break;		
//				case 4:		printf("Phase fail\r\n");			break;
//				case 5:		printf("HW fail\r\n");				break;
//				default:	printf("Error\r\n");					break;
//		}
}





