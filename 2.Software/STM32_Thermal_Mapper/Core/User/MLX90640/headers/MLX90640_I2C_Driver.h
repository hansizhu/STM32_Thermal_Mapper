/**
 * @copyright (C) 2017 Melexis N.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef _MLX90640_I2C_Driver_H_
#define _MLX90640_I2C_Driver_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include "main.h"

	 
//IO方向设置
#define SDA_IN()  {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}		//PB7设置输入
#define SDA_OUT() {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;}		//PB7设置输出

//IO操作函数	
/*********************************** IIC_SCL ******************************/

#define IIC_SCL_LOW()             PBout(6)=0
#define IIC_SCL_HIGH()            PBout(6)=1

/*********************************** IIC_SDA ******************************/

#define IIC_SDA_LOW()             PBout(7)=0
#define IIC_SDA_HIGH()            PBout(7)=1
#define IIC_SDA_READ()            PBin(7)



void MLX90640_I2CInit(void);
int  MLX90640_I2CRead(uint8_t slaveAddr,uint16_t startAddress, uint16_t nMemAddressRead, uint16_t *data);
int  MLX90640_I2CWrite(uint8_t slaveAddr,uint16_t writeAddress, uint16_t data);
void MLX90640_I2CFreqSet(int freq);

#ifdef __cplusplus
}
#endif

#endif
