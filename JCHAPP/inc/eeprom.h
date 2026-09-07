#ifndef _EEPROM_H
#define _EEPROM_H

#include "stdbool.h"
#include "stm32f4xx_hal.h"

#define SCL_PIN GPIO_PIN_6
#define SDA_PIN GPIO_PIN_7
#define SCL_PORT GPIOB
#define SDA_PORT GPIOB
#define SCL_RCC_CLOCK RCC_APB2Periph_GPIOB
#define SDA_RCC_CLOCK RCC_APB2Periph_GPIOB

#define SCL_H  HAL_GPIO_WritePin(SCL_PORT,SCL_PIN,GPIO_PIN_SET)
#define SCL_L  HAL_GPIO_WritePin(SCL_PORT,SCL_PIN,GPIO_PIN_RESET)

#define SDA_H  HAL_GPIO_WritePin(SDA_PORT,SDA_PIN,GPIO_PIN_SET)
#define SDA_L  HAL_GPIO_WritePin(SDA_PORT,SDA_PIN,GPIO_PIN_RESET)

#define SCL_read  HAL_GPIO_ReadPin(SCL_PORT,SCL_PIN)
#define SDA_read  HAL_GPIO_ReadPin(SDA_PORT,SDA_PIN)

#define I2C_PageSize 8 

extern uint8_t usart1_slaveaddr;
extern uint8_t usart2_slaveaddr;
extern uint8_t usart3_slaveaddr;
extern uint8_t usart6_slaveaddr;
extern uint8_t eth_slaveaddr;
extern uint8_t eppWriteBuffer1[10];
extern uint8_t eppWriteBuffer2[10];
extern uint8_t eppWriteBuffer3[10];
extern uint8_t eppWriteBuffer4[10];

bool I2C_WriteByte(uint8_t SendByte, uint16_t WriteAddress, uint8_t DeviceAddress);
bool I2C_BufferWrite(uint8_t* pBuffer, uint8_t length, uint16_t WriteAddress, uint8_t DeviceAddress);
void I2C_PageWrite(uint8_t* pBuffer, uint8_t length, uint16_t WriteAddress, uint8_t DeviceAddress);
bool I2C_ReadByte(uint8_t* pBuffer, uint8_t length, uint16_t ReadAddress, uint8_t DeviceAddress);
extern void I2C_Test(void);

void I2C_SendByte(uint8_t SendByte);
uint8_t I2C_ReceiveByte(void);

bool I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
bool I2C_WaitAck(void);

extern void EEPROM_Writer1();
extern void EEPROM_Writer2();
extern void EEPROM_Writer3();
extern void EEPROM_Writer4();
extern void EEPROM_Reader();







#endif
