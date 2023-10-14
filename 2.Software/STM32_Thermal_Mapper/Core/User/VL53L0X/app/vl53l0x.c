#include "vl53l0x.h"
#include "vl53l0x_gen.h"


Vl53l0x_Data_Z_TypeDef Vl53l0x_Data_Z;


VL53L0X_Dev_t vl53l0x_dev;//�豸I2C���ݲ���
VL53L0X_DeviceInfo_t vl53l0x_dev_info;//�豸ID�汾��Ϣ


//VL53L0X������ģʽ����
//0��Ĭ��;1:�߾���;2:������;3:����
mode_data Mode_data[]=
{
    {(FixPoint1616_t)(0.25*65536), 
	 (FixPoint1616_t)(18*65536),
	 33000,
	 14,
	 10},//Ĭ��
		
	{(FixPoint1616_t)(0.25*65536) ,
	 (FixPoint1616_t)(18*65536),
	 200000, 
	 14,
	 10},//�߾���
		
    {(FixPoint1616_t)(0.1*65536) ,
	 (FixPoint1616_t)(60*65536),
	 33000,
	 18,
	 14},//������
	
    {(FixPoint1616_t)(0.25*65536) ,
	 (FixPoint1616_t)(32*65536),
	 20000,
	 14,
	 10},//����
		
};


//��ʼ��vl53l0x
//dev:�豸I2C�����ṹ��
VL53L0X_Error vl53l0x_init(VL53L0X_Dev_t *dev)
{
	GPIO_InitTypeDef GPIO_Initure;
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;
	VL53L0X_Dev_t *pMyDevice = dev;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();            //����GPIOAʱ��
	
	GPIO_Initure.Pin=GPIO_PIN_15;            //PA15
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP; 	 //�������
	GPIO_Initure.Pull=GPIO_PULLUP;           //����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 //����
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);

	pMyDevice->I2cDevAddr = VL53L0X_Addr;//I2C��ַ(�ϵ�Ĭ��0x52)
	pMyDevice->comms_type = 1;           //I2Cͨ��ģʽ
	pMyDevice->comms_speed_khz = 400;    //I2Cͨ������
	
	VL53L0X_i2c_init();//��ʼ��IIC����
	
	VL53L0X_Xshut=0;//ʧ��VL53L0X
	HAL_Delay(30);
	VL53L0X_Xshut=1;//ʹ��VL53L0X,�ô��������ڹ���
	HAL_Delay(30);
	
	Status = VL53L0X_DataInit(pMyDevice);//�豸��ʼ��
	if(Status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",Status);
	HAL_Delay(2);
	Status = VL53L0X_GetDeviceInfo(pMyDevice,&vl53l0x_dev_info);//��ȡ�豸ID��Ϣ
	if(Status!=VL53L0X_ERROR_NONE)		printf("Error code:%d",Status);
	
	vl53l0x_set_mode(dev,HIGH_ACCURACY);//���ò���ģʽ

	return Status;
}

//----------���º���Ϊ����������------------//

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





