/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
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

#include "fatfs.h"
#include <stdio.h>

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

/**
  * ��������: FatFS�ļ�ϵͳ���������Ϣ����.
  * �������: FatFS�ļ�ϵͳ���������FRESULT
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void printf_fatfs_error(FRESULT fresult)
{
  switch(fresult)
  {
    case FR_OK:                   //(0)
      printf("�������ɹ���\r\n");
    break;
    case FR_DISK_ERR:             //(1)
      printf("����Ӳ�����������������\r\n");
    break;
    case FR_INT_ERR:              //(2)
      printf("�������Դ���\r\n");
    break;
    case FR_NOT_READY:            //(3)
      printf("���������豸�޷�������\r\n");
    break;
    case FR_NO_FILE:              //(4)
      printf("�����޷��ҵ��ļ���\r\n");
    break;
    case FR_NO_PATH:              //(5)
      printf("�����޷��ҵ�·����\r\n");
    break;
    case FR_INVALID_NAME:         //(6)
      printf("������Ч��·������\r\n");
    break;
    case FR_DENIED:               //(7)
    case FR_EXIST:                //(8)
      printf("�����ܾ����ʡ�\r\n");
    break;
    case FR_INVALID_OBJECT:       //(9)
      printf("������Ч���ļ���·����\r\n");
    break;
    case FR_WRITE_PROTECTED:      //(10)
      printf("�����߼��豸д������\r\n");
    break;
    case FR_INVALID_DRIVE:        //(11)
      printf("������Ч���߼��豸��\r\n");
    break;
    case FR_NOT_ENABLED:          //(12)
      printf("������Ч�Ĺ�������\r\n");
    break;
    case FR_NO_FILESYSTEM:        //(13)
      printf("������Ч���ļ�ϵͳ��\r\n");
    break;
    case FR_MKFS_ABORTED:         //(14)
      printf("���������������⵼��f_mkfs��������ʧ�ܡ�\r\n");
    break;
    case FR_TIMEOUT:              //(15)
      printf("����������ʱ��\r\n");
    break;
    case FR_LOCKED:               //(16)
      printf("�����ļ���������\r\n");
    break;
    case FR_NOT_ENOUGH_CORE:      //(17)
      printf("�������ļ���֧�ֻ�ȡ�ѿռ�ʧ�ܡ�\r\n");
    break;
    case FR_TOO_MANY_OPEN_FILES:  //(18)
      printf("������̫���ļ���\r\n");
    break;
    case FR_INVALID_PARAMETER:    // (19)
      printf("����������Ч��\r\n");
    break;
  }
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
