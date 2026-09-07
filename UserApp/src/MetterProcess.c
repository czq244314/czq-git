#include "MetterProcess.h"

uint16_t Metter_SlaveAddr=1;

 uint8_t sendflag;
 uint8_t sendflag1;

static ModbusRTU_Frame_t g_ReqFrame;
extern UART_HandleTypeDef huart1;


void readOutputRegs0(uint8_t* buf)
{
//	g_CHANNEL.ammeter_volt = (buf[0]<<24) | (buf[1]<<16) | (buf[2]<<8) | buf[3];
//	g_CHANNEL.ammeter_current = (buf[8]<<24) | (buf[9]<<16) | (buf[10]<<8) | buf[11];
//	g_CHANNEL.ammeter_power = (buf[16]<<24) | (buf[17]<<16) | (buf[18]<<8) | buf[19];
	sendflag=0;
	
#ifdef _USE_RTT
		SEGGER_RTT_SetTerminal(0);
		for(int i=0;i<4;i++)
		{
			SEGGER_RTT_printf(0,"%X ",buf[i]);
		}
		SEGGER_RTT_printf(0,"\r\n");
		
		for(int i=0;i<4;i++)
		{
			SEGGER_RTT_printf(0,"%X ",buf[8+i]);
		}
		SEGGER_RTT_printf(0,"\r\n");
		
		for(int i=0;i<4;i++)
		{
			SEGGER_RTT_printf(0,"%X ",buf[16+i]);
		}
		SEGGER_RTT_printf(0,"\r\n");
		SEGGER_RTT_printf(0,"volt:%X,curr:%X,power:%X\r\n",g_CHANNEL.ammeter_volt,g_CHANNEL.ammeter_current,g_CHANNEL.ammeter_power);
		//SEGGER_RTT_printf(0,"volt:%d,curr:%d,power:%d\r\n",g_CHANNEL.ammeter_volt,g_CHANNEL.ammeter_current,g_CHANNEL.ammeter_power);
#endif
}

float bufToFloat(uint8_t* pValue)
{
    float x;
    unsigned char s[4];
    s[0] = pValue[3];
    s[1] = pValue[2];
    s[2] = pValue[1];
    s[3] = pValue[0];
    memcpy(&x, s, 4);
    return x + 5E-6;
}

void METTER(void)
{
	uint8_t pbuf[8]={0};
	uint8_t i=0;
	uint16_t crc;
	pbuf[i++]=0x33;
	pbuf[i++]=0x03;//1
	pbuf[i++]=0x02;//3
	pbuf[i++]=0x00;//4
	pbuf[i++]=0x20;
	crc=crc16(pbuf,i);
	pbuf[i++]=(crc>>8)&0xFF;
	pbuf[i++]=crc&0xFF; //16	
	HAL_GPIO_WritePin(RS485_EN1_GPIO_Port,RS485_EN1_Pin,GPIO_PIN_SET);
	osDelay(3);
	HAL_UART_Transmit(&huart1,pbuf,i,1000);
	HAL_GPIO_WritePin(RS485_EN1_GPIO_Port,RS485_EN1_Pin,GPIO_PIN_RESET);

}

//03功能码读函数处理
void readOutputRegs1(uint8_t* buf)
{ 
	//g_CHANNEL.ammeter_energy = (float)((buf[0]<<24) | (buf[1]<<16) | (buf[2]<<8) | buf[3]);
//	g_CHANNEL.ammeter_energy=bufToFloat(buf);
	sendflag1=0;
}

void MatterModbusRTU_Frame(uint8_t *pReqFrameBuf, uint16_t reqFrameBufSize)
{
    uint16_t regAddr = 0;
    uint16_t reqSize = 0;
//    uint16_t sendSize = 0;
    uint8_t ret = 0;
    uint16_t i = 0;
	  static ModbusRTU_Frame_t g_RepFrame;//定义函数内部结构体

    ModbusRTU_Frame_t *pReqFrame = &g_ReqFrame;//ModbusRTU_Frame_t类型的指针指向g_ReqFrame结构体

		if(pReqFrameBuf[0]==0x33 && pReqFrameBuf[1]==0x03)
		{
			METTER();
			return;
		}
	//验证收取的报文正确性
    if (false == buf2ModbusRTU_Frame(pReqFrameBuf, reqFrameBufSize, pReqFrame))
    {
        //解析帧失败则直接返回
        return ;
    }
		//CRC校验
		if(pReqFrame->crc != crc16(pReqFrameBuf,  reqFrameBufSize - 2)){return;}
   
    g_RepFrame.slaveAddr = pReqFrame->slaveAddr;
    g_RepFrame.funCode = pReqFrame->funCode;
		

//		//对比从站地址，不正确退出函数
//		if(g_RepFrame.slaveAddr!=g_SlaveAddr)return;
 
    switch(pReqFrame->funCode)
    {

        case FC_ReadHoldingReg:
        {
            // 3 读取保持寄存器 4区
            if(sendflag==1)
						{
							readOutputRegs0(pReqFrame->data+1);
						}
            if(sendflag1==1)
						{
							readOutputRegs1(pReqFrame->data+1);
						}
        }
        break;



        default:
            break;
    }

    if (ret == false)
    {
        return ;
    }
		


 }