#include "eeprom.h"
#include "includes.h"

extern I2C_HandleTypeDef hi2c1;
static uint8 eppReadBuffer[74]= {0};
uint8_t eppWriteBuffer1[10];
uint8_t eppWriteBuffer2[10];
uint8_t eppWriteBuffer3[10];
uint8_t eppWriteBuffer4[10];
uint8_t usart1_slaveaddr;
uint8_t usart2_slaveaddr;
uint8_t usart3_slaveaddr;
uint8_t usart6_slaveaddr;
uint8_t eth_slaveaddr;

static void I2C_delay(void)
{
    uint8_t i = 100;
    while (i) {
        i--;
			 
    }
}

/*  
 * 函数名: bool I2C_Start(void) 
 * 描述  : 起始信号 
 * 输入  : 无 
 * 输出  : TRUE : 成功 
                     FALSE : 失败 
 * 说明  :  
 */
bool I2C_Start(void)
{
    SDA_H;
    SCL_H;
    I2C_delay();
    if (!SDA_read)
        return false; //SDA线为低电平则总线忙,退出
    SDA_L;
    I2C_delay();
    if (SDA_read)
        return false; //SDA线为高电平则总线出错,退出
    SDA_L;
    I2C_delay();
    return true;
}
/*  
 * 函数名: static void I2C_Stop(void) 
 * 描述  : 终止信号 
 * 输入  : 无 
 * 输出  : 无 
 * 说明  :  
 */
void I2C_Stop(void)
{
    SCL_L;
    I2C_delay();
    SDA_L;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SDA_H;
    I2C_delay();
}
/*  
 * 函数名: static void I2C_Ack(void) 
 * 描述  : 应答信号 
 * 输入  : 无 
 * 输出  : 无 
 * 说明  :  
 */
static void I2C_Ack(void)
{
    SCL_L;
    I2C_delay();
    SDA_L;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SCL_L;
    I2C_delay();
}
/*  
 * 函数名: void I2C_NoAck(void) 
 * 描述  : 无应答信号 
 * 输入  : 无 
 * 输出  : 无 
 * 说明  :  
 */
static void I2C_NoAck(void)
{
    SCL_L;
    I2C_delay();
    SDA_H;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SCL_L;
    I2C_delay();
}
/*  
 * 函数名: bool I2C_Start(void) 
 * 描述  : 等待应答信号 
 * 输入  : 无 
 * 输出  : TRUE : 有应答 
                     FALSE : 无应答 
 * 说明  :  
 */
bool I2C_WaitAck(void)
{
    SCL_L;
    I2C_delay();
    SDA_H;
    I2C_delay();
    SCL_H;
    I2C_delay();
    if (SDA_read) {
        SCL_L;
        return false;
    }
    SCL_L;
    return true;
}
/*  
 * 函数名: static void I2C_SendByte(u8 SendByte)  
 * 描述  : 发送一个字节 
 * 输入  : SendByte : 字节数据 
 * 输出  : 无 
 * 说明  : 数据从高位到低位 
 */
void I2C_SendByte(uint8_t SendByte)
{
    uint8_t i = 8;
    while (i--) {
        SCL_L;
        I2C_delay();
        if (SendByte & 0x80)
            SDA_H;
        else
            SDA_L;
        SendByte <<= 1;
        I2C_delay();
        SCL_H;
        I2C_delay();
    }
    SCL_L;
}
/*  
 * 函数名: static u8 I2C_ReceiveByte(void)  
 * 描述  : 读取一个字节 
 * 输入  : 无  
 * 输出  : 字节数据 
 * 说明  : ReceiveByte : 数据从高位到低位 
 */
static uint8_t I2C_ReceiveByte(void)
{
    uint8_t i = 8;
    uint8_t ReceiveByte = 0;

    SDA_H;
    while (i--) {
        ReceiveByte <<= 1;
        SCL_L;
        I2C_delay();
        SCL_H;
        I2C_delay();
        if (SDA_read) {
            ReceiveByte |= 0x01;
        }
    }
    SCL_L;
    return ReceiveByte;
}

/*  
 * 函数名: bool I2C_WriteByte(u8 SendByte, u16 WriteAddress, u8 DeviceAddress) 
 * 描述  : 写入1字节数据   
 * 输入  : SendByte : 要写入数据 
                     WriteAddress : 写入地址 
                     DeviceAddress : 器件地址 
 * 输出  : TRUE : 成功 
                     FALSE : 失败 
 * 说明  :  
 */
bool I2C_WriteByte(uint8_t SendByte, uint16_t WriteAddress, uint8_t DeviceAddress)
{
    if (!I2C_Start())
        return false;
    I2C_SendByte(((WriteAddress & 0x0700) >> 7) | (DeviceAddress & 0xFFFE)); //设置高起始地址+器件地址
    if (!I2C_WaitAck()) {
        I2C_Stop();
        return false;
    }
    I2C_SendByte((uint8_t)(WriteAddress & 0x00FF)); //设置低起始地址
    I2C_WaitAck();
    I2C_SendByte(SendByte);
    I2C_WaitAck();
    I2C_Stop();

    osDelay(10); //注意：因为这里要等待EEPROM写完，可以采用查询或延时方式(10ms)
		
    return true;
}
/*  
 * 函数名: bool I2C_WriteByte(u8 SendByte, u16 WriteAddress, u8 DeviceAddress) 
 * 描述  : 写入1串数据  
 * 输入  : pBuffer : 要写入数据缓冲区指针 
           length : 待写入长度   
                     WriteAddress : 写入地址 
                     DeviceAddress : 器件地址 
 * 输出  : TRUE : 成功 
                     FALSE : 失败 
 * 说明  : 注意不能跨页写 
 */
bool I2C_BufferWrite(uint8_t* pBuffer, uint8_t length, uint16_t WriteAddress, uint8_t DeviceAddress)
{
    if (!I2C_Start())
        return false;
    I2C_SendByte(((WriteAddress & 0x0700) >> 7) | (DeviceAddress & 0xFFFE)); //设置高起始地址+器件地址
    if (!I2C_WaitAck()) {
        I2C_Stop();
        return false;
    }
    I2C_SendByte((uint8_t)(WriteAddress & 0x00FF)); //设置低起始地址
    I2C_WaitAck();
    while (length--) {
        I2C_SendByte(*pBuffer);
        I2C_WaitAck();
        pBuffer++;
    }
    I2C_Stop();

    osDelay(10); //注意：因为这里要等待EEPROM写完，可以采用查询或延时方式(10ms)

    return true;
}

/*  
 * 函数名: bool I2C_WriteByte(u8 SendByte, u16 WriteAddress, u8 DeviceAddress) 
 * 描述  : 写入1串数据  
 * 输入  : pBuffer : 要写入数据缓冲区指针 
           length : 待写入长度   
                     WriteAddress : 写入地址 
                     DeviceAddress : 器件地址 
 * 输出  : TRUE : 成功 
                     FALSE : 失败 
 * 说明  : 跨页写入1串数据 
 */
void I2C_PageWrite(uint8_t* pBuffer, uint8_t length, uint16_t WriteAddress, uint8_t DeviceAddress)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
    Addr = WriteAddress % I2C_PageSize; //写入地址是开始页的第几位
    count = I2C_PageSize - Addr; //在开始页要写入的个数
    NumOfPage = length / I2C_PageSize; //要写入的页数
    NumOfSingle = length % I2C_PageSize; //不足一页的个数

    if (Addr == 0) //写入地址是页的开始
    {
        if (NumOfPage == 0) //数据小于一页
        {
            I2C_BufferWrite(pBuffer, NumOfSingle, WriteAddress, DeviceAddress); //写少于一页的数据
        } else //数据大于等于一页
        {
            while (NumOfPage) //要写入的页数
            {
                I2C_BufferWrite(pBuffer, I2C_PageSize, WriteAddress, DeviceAddress); //写一页的数据
                WriteAddress += I2C_PageSize;
                pBuffer += I2C_PageSize;
                NumOfPage--;
                osDelay(10);
            }
            if (NumOfSingle != 0) //剩余数据小于一页
            {
                I2C_BufferWrite(pBuffer, NumOfSingle, WriteAddress, DeviceAddress); //写少于一页的数据
                osDelay(10);
            }
        }
    }

    else //写入地址不是页的开始
    {
        if (NumOfPage == 0) //数据小于一页
        {
            I2C_BufferWrite(pBuffer, NumOfSingle, WriteAddress, DeviceAddress); //写少于一页的数据
        } else //数据大于等于一页
        {
            length -= count;
            NumOfPage = length / I2C_PageSize; //重新计算要写入的页数
            NumOfSingle = length % I2C_PageSize; //重新计算不足一页的个数

            if (count != 0) {
                I2C_BufferWrite(pBuffer, count, WriteAddress, DeviceAddress); //将开始的空间写满一页
                WriteAddress += count;
                pBuffer += count;
            }

            while (NumOfPage--) //要写入的页数
            {
                I2C_BufferWrite(pBuffer, I2C_PageSize, WriteAddress, DeviceAddress); //写一页的数据
                WriteAddress += I2C_PageSize;
                pBuffer += I2C_PageSize;
            }
            if (NumOfSingle != 0) //剩余数据小于一页
            {
                I2C_BufferWrite(pBuffer, NumOfSingle, WriteAddress, DeviceAddress); //写少于一页的数据
            }
        }
    }
}

/*  
 * 函数名: bool I2C_ReadByte(u8* pBuffer,   u8 length,     u16 ReadAddress,  u8 DeviceAddress) 
 * 描述  : 读出1串数据 
 * 输入  : pBuffer : 要读取数据缓冲区指针 
           length : 待读取长度   
                     WriteAddress : 读取地址 
                     DeviceAddress : 器件地址 
 * 输出  : TRUE : 成功 
                     FALSE : 失败 
 * 说明  : 跨页写入1串数据 
 */
bool I2C_ReadByte(uint8* pBuffer, uint8 length, uint16 ReadAddress, uint8 DeviceAddress)
{
    if (!I2C_Start())
        return FALSE;
    I2C_SendByte(((ReadAddress & 0x0700) >> 7) | (DeviceAddress & 0xFFFE)); //设置高起始地址+器件地址
    if (!I2C_WaitAck()){
        I2C_Stop();
        return FALSE;
    }
    I2C_SendByte((uint8)(ReadAddress & 0x00FF)); //设置低起始地址
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(((ReadAddress & 0x0700) >> 7) | DeviceAddress | 0x0001);
    I2C_WaitAck();
    while (length) {
        *pBuffer = I2C_ReceiveByte();
        if (length == 1)
            I2C_NoAck();
        else
            I2C_Ack();
        pBuffer++;
        length--;
    }
    I2C_Stop();
    return TRUE;
} 

void EEPROM_Writer1()
{
	memcpy(&eppWriteBuffer1[0], &usart1_slaveaddr, 1); 
	memcpy(&eppWriteBuffer1[1], &usart1_deviceType, 1); 
	memcpy(&eppWriteBuffer1[2], &NewBaudRate1, 4);
	I2C_PageWrite(eppWriteBuffer1, 6, 0x00, 0xa0);
}

void EEPROM_Writer2()
{
	memcpy(&eppWriteBuffer2[0], &usart2_slaveaddr, 1); 
	memcpy(&eppWriteBuffer2[1], &usart2_deviceType, 1); 
	memcpy(&eppWriteBuffer2[2], &NewBaudRate2, 4);
	I2C_PageWrite(eppWriteBuffer2, 6, 0x06, 0xa0);
}

void EEPROM_Writer3()
{
	memcpy(&eppWriteBuffer3[0], &usart3_slaveaddr, 1); 
	memcpy(&eppWriteBuffer3[1], &usart3_deviceType, 1); 
	memcpy(&eppWriteBuffer3[2], &NewBaudRate3, 4);
	I2C_PageWrite(eppWriteBuffer3, 6, 0x0C, 0xa0);
}

void EEPROM_Writer4()
{
	memcpy(&eppWriteBuffer4[0], &usart6_slaveaddr, 1); 
	memcpy(&eppWriteBuffer4[1], &usart6_deviceType, 1); 
	memcpy(&eppWriteBuffer4[2], &NewBaudRate4, 4);
	I2C_PageWrite(eppWriteBuffer4, 6, 0x12, 0xa0);
}

void EEPROM_Reader()
{
	I2C_ReadByte(eppReadBuffer, 24, 0x00, 0xa0);
	
	memcpy(&usart1_slaveaddr,&eppReadBuffer[0],1);	
	memcpy(&usart1_deviceType,&eppReadBuffer[1],1);
	memcpy(&NewBaudRate1,&eppReadBuffer[2],4);//读取串口1配置初始化
	memcpy(&usart2_slaveaddr,&eppReadBuffer[6],1);	
	memcpy(&usart2_deviceType,&eppReadBuffer[7],1);
	memcpy(&NewBaudRate2,&eppReadBuffer[8],4);//读取串口2配置初始化
	memcpy(&usart1_slaveaddr,&eppReadBuffer[12],1);	
	memcpy(&usart1_deviceType,&eppReadBuffer[13],1);
	memcpy(&NewBaudRate1,&eppReadBuffer[14],4);//读取串口3配置初始化
	memcpy(&usart1_slaveaddr,&eppReadBuffer[18],1);	
	memcpy(&usart1_deviceType,&eppReadBuffer[19],1);
	memcpy(&NewBaudRate1,&eppReadBuffer[20],4);//读取串口6配置初始化
}