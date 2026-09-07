#include "mcgs.h"
#include "stdint.h"
#include "main.h"
#include "stdbool.h"
#include "eeprom.h"
#include "modbusrtu.h"
#include "typedef.h"

#if 1

int32_t float_to_int32_saturate(float f)
{
    // 四舍五入：加0.5后取整（注意负数处理）
    if (f >= 0.0f)
        f += 0.5f;
    else
        f -= 0.5f;

    // 饱和处理
    if (f >= (float)INT32_MAX)
        return INT32_MAX;
    if (f <= (float)INT32_MIN)
        return INT32_MIN;

    return (int32_t)f;
}
	


//读功能码处理函数
bool eth_McgsReadOutputReg(Channel_t channel, uint8_t* pBuf,uint8_t Count, uint8_t addr)
{
		uint8_t cPos = 0;	
		int32_t intRep =0;
		uint8_t mid[4]={0};
		Channel_t ChanPos=channel;
	
		while((ChanPos<eth_MAX) && ChanPos<(channel+Count))
		{
				pBuf[cPos+1]=0;
	
	#if defined(_USE_RTT)
					SEGGER_RTT_SetTerminal(4);
					SEGGER_RTT_printf(0,"McgsReadOutputReg Addr:%d\r\n",ChanPos);
#endif
			
				switch (ChanPos){
			
					
					default:
						break;
				}
				ChanPos++;
				cPos+=2;
				//ChanPos = channel+cPos;
		}
    return true;
}
 
//写功能码处理函数 
bool eth_McgsWriteOutputReg(Channel_t channel, uint16_t value, uint8_t addr)
{
		uint16_t RMvar =0;
		uint8_t mid[4]={0};
    switch (channel) {
				
			
			default:
					break;
					
				}
    return true;
}

bool eth_McgsWriteOutputRegs(Channel_t channel, uint16_t size, uint16_t* pValue, uint8_t addr)
{
		uint8_t cPos = 0;

		Channel_t ChanPos = channel+cPos;
		while(ChanPos<eth_MAX)
		{
				eth_McgsWriteOutputReg(	ChanPos,pValue[cPos],0);
				
				cPos++;
				ChanPos = channel+cPos;
			
				if(cPos>=size)
					break;
		}
    return true;
}

//03功能码读函数处理
 uint8_t ETHeth_readOutputRegs(Channel_t regAddr, uint16_t size, ETHModbusRTU_Frame_t *pRepFrame, uint8_t addr)
{
    //uint16_t channel = regAddr + 1;
    uint8_t repBytesCnt = size * 2;//一寄存器数据两字节
    uint8_t ret = 0;

		ret = eth_McgsReadOutputReg((Channel_t)regAddr, &(pRepFrame->data[1]),size,addr);

    pRepFrame->data[0] = repBytesCnt;
    pRepFrame->len = repBytesCnt + 1;
    return ret;
}

//03功能码读函数处理
 uint8_t eth_readOutputRegs(Channel_t regAddr, uint16_t size, ModbusRTU_Frame_t *pRepFrame, uint8_t addr)
{
    //uint16_t channel = regAddr + 1;
    uint8_t repBytesCnt = size * 2;//一寄存器数据两字节
    uint8_t ret = 0;

		ret = eth_McgsReadOutputReg((Channel_t)regAddr, &(pRepFrame->data[1]),size,addr);

    pRepFrame->data[0] = repBytesCnt;
    pRepFrame->len = repBytesCnt + 1;
    return ret;
}

//06功能码写函数处理
 uint8_t eth_writeOutputReg(Channel_t regAddr, uint16_t value, ModbusRTU_Frame_t *pRepFrame,uint8_t addr)
{
    //uint16_t channel = regAddr + 1;
    uint8_t ret = 0;

    ret = eth_McgsWriteOutputReg((Channel_t)regAddr, value, addr);

    pRepFrame->data[0] = (regAddr >> 8) & 0xff;
    pRepFrame->data[1] = (regAddr) & 0xff;

    pRepFrame->data[2] = (value >> 8) & 0xff;
    pRepFrame->data[3] = (value) & 0xff;
    pRepFrame->len = 4;
    
    return ret;
}

//10功能码写函数处理
 uint8_t ETHeth_writeOutputRegs(Channel_t regAddr, uint16_t size,
                               uint16_t *pValue, ETHModbusRTU_Frame_t *pRepFrame,uint8_t addr)
{
    //uint16_t channel = regAddr + 1;
    uint8_t ret = 0;

    ret = eth_McgsWriteOutputRegs((Channel_t)regAddr, size, pValue,addr);
    pRepFrame->data[0] = (regAddr >> 8) & 0xff;
    pRepFrame->data[1] = (regAddr) & 0xff;
    pRepFrame->data[2] = (size >> 8) & 0xff;
    pRepFrame->data[3] = (size) & 0xff;
    pRepFrame->len = 4;
    
    return ret;
}

//10功能码写函数处理
 uint8_t eth_writeOutputRegs(Channel_t regAddr, uint16_t size,
                               uint16_t *pValue, ModbusRTU_Frame_t *pRepFrame,uint8_t addr)
{
    //uint16_t channel = regAddr + 1;
    uint8_t ret = 0;

    ret = eth_McgsWriteOutputRegs((Channel_t)regAddr, size, pValue,addr);
    pRepFrame->data[0] = (regAddr >> 8) & 0xff;
    pRepFrame->data[1] = (regAddr) & 0xff;
    pRepFrame->data[2] = (size >> 8) & 0xff;
    pRepFrame->data[3] = (size) & 0xff;
    pRepFrame->len = 4;
    
    return ret;
}

#endif