/********************************************************

*********************************************************/

/********************************************************
                    include
*********************************************************/

#include "ModbusRTU.h"
#include "JCHAppData.h"
#include "mcgs.h"
#if 1
#if defined(_USE_RTT)
	#include "SEGGER_RTT.h"
#endif



/********************************************************
                    macro
*********************************************************/

/********************************************************
                    typedef
*********************************************************/
uint8_t (*pFnReadInputRegs)(Channel_t channel, uint8_t* pBuf);
//uint8_t (*pFnReadOutputRegs)(Channel_t channel, uint16_t size,uint8_t* pBuf,ModbusRTU_Frame_t *pRepFrame,uint8_t addr);
uint8_t (*pFnReadOutputRegs)(Channel_t regAddr, uint16_t size, ModbusRTU_Frame_t *pRepFrame, uint8_t addr);
uint8_t (*pFnWriteOutputReg)(Channel_t channel, uint16_t value,ModbusRTU_Frame_t *pRepFrame, uint8_t addr);
uint8_t (*pFnWriteOutputRegs)(Channel_t channel, uint16_t size, uint16_t* pValue, ModbusRTU_Frame_t *pRepFrame,uint8_t addr);

void (*pFnModbusSend)(uint8_t *dataBuff,uint16_t BufferSize,uint8_t TranIndex);
static queue_data_t  mcgs_send_buff={0};
static queue_data_t  usart1_send_buff={0};
static queue_data_t  usart2_send_buff={0};
static queue_data_t  usart3_send_buff={0};
static queue_data_t  usart6_send_buff={0};

/********************************************************
                    variables
*********************************************************/
/* Table of CRC values for high-order byte */
static const uint8_t table_crc_hi[] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* Table of CRC values for low-order byte */
static const uint8_t table_crc_lo[] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};
static ModbusRTU_Frame_t g_ReqFrame;                              
static  uint16_t g_OutputRegSetValues[128] = {0};

/********************************************************
                    functions
*********************************************************/
uint16_t crc16(uint8_t *buffer, uint16_t buffer_length)
{
    uint8_t crc_hi = 0xFF; /* high CRC byte initialized */
    uint8_t crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i; /* will index into CRC lookup */

    /* pass through message buffer */
    while (buffer_length--)
    {
        i = crc_hi ^ *buffer++; /* calculate the CRC  */
        crc_hi = crc_lo ^ table_crc_hi[i];
        crc_lo = table_crc_lo[i];
    }

    return (crc_hi << 8 | crc_lo);
}

/*
    regAddr 从0开始,对应的通道为1
    错误返回0
*/
static uint8_t readInputRegs(uint16_t regAddr, uint16_t size, ModbusRTU_Frame_t *pRepFrame)
{
//    uint16_t channel = regAddr + 1;
//    uint8_t repBytesCnt = size * 2;
//    //uint8_t additionalLen = 0;              /* 附件长度 */
//    uint8_t ret = 0;

//    if(pFnReadInputRegs)
//			ret = pFnReadInputRegs((Channel_t)channel, &(pRepFrame->data[1]));

//    pRepFrame->data[0] = repBytesCnt;
//    pRepFrame->len = repBytesCnt + 1;
//    return ret;
}

static uint8_t readOutputRegs(uint16_t regAddr, uint16_t size, ModbusRTU_Frame_t *pRepFrame, uint8_t addr)
{
    uint16_t channel = regAddr;
    uint8_t repBytesCnt = size * 2;
    uint8_t ret = 0;

//    if(pFnReadOutputRegs)
			ret = eth_readOutputRegs((Channel_t)channel, size, pRepFrame,addr);

    pRepFrame->data[0] = repBytesCnt;
    pRepFrame->len = repBytesCnt + 1;
    return ret;
}

static uint8_t writeOutputReg(uint16_t regAddr, uint16_t value, ModbusRTU_Frame_t *pRepFrame,uint8_t addr)
{
    uint16_t channel = regAddr;
    uint8_t ret = 0;

//    if(pFnWriteOutputReg)
			ret = eth_writeOutputReg((Channel_t)channel, value,pRepFrame, addr);

    pRepFrame->data[0] = (regAddr >> 8) & 0xff;
    pRepFrame->data[1] = (regAddr) & 0xff;

    pRepFrame->data[2] = (value >> 8) & 0xff;
    pRepFrame->data[3] = (value) & 0xff;
    pRepFrame->len = 4;
    
    return ret;
}

static uint8_t writeOutputRegs(uint16_t regAddr, uint16_t size,
                               uint16_t *pValue, ModbusRTU_Frame_t *pRepFrame,uint8_t addr)
{
    uint16_t channel = regAddr;
    uint8_t ret = 0;

//    if(pFnWriteOutputRegs)
			ret = eth_writeOutputRegs((Channel_t)channel, size, pValue,pRepFrame,addr);
    pRepFrame->data[0] = (regAddr >> 8) & 0xff;
    pRepFrame->data[1] = (regAddr) & 0xff;
    pRepFrame->data[2] = (size >> 8) & 0xff;
    pRepFrame->data[3] = (size) & 0xff;
    pRepFrame->len = 4;
    
    return ret;
}


/*
    把modbusRTU帧转化成buf并计算校验，并且返回要发送的长度
*/
 uint16_t modbusRTU_Frame2Buf(ModbusRTU_Frame_t *pFrame, uint8_t *pBuf)
{
    uint16_t i = 0;

    pBuf[0] = pFrame->slaveAddr;
    pBuf[1] = pFrame->funCode;
	
    for (i = 0; i < pFrame->len; i++)
    {
        pBuf[i + 2] = pFrame->data[i];
    }
    pFrame->crc = crc16(pBuf, i + 2);
    pBuf[i + 2] = (pFrame->crc >> 8) & 0xff;
    pBuf[i + 3] = (pFrame->crc) & 0xff;

#if defined( _USE_RTT)&0
      SEGGER_RTT_SetTerminal(8);
	SEGGER_RTT_printf(0,"Modbus Pack:\r\n");
#endif
	
	#if defined(_USE_RTT)&0
		for(uint16_t j=0;j<i + 4;j++)
			SEGGER_RTT_printf(0,"%02X ",pBuf[j]);

#endif
#if defined(_USE_RTT)&0
     SEGGER_RTT_SetTerminal(8);
		SEGGER_RTT_printf(0,"\r\n len:%d,crc:%x \r\n",i+4,pFrame->crc );
#endif
		
    return i + 4;
}

/* 错误返回0 */
 bool buf2ModbusRTU_Frame(uint8_t *pReqFrameBuf,
                                   uint16_t reqFrameBufSize,
                                   ModbusRTU_Frame_t *pReqFrame)
{
    int i;

    if (reqFrameBufSize < 4)
    {
        /* 长度小于4返回错误 */
        return false;
    }
    pReqFrame->slaveAddr = pReqFrameBuf[0];
    pReqFrame->funCode = pReqFrameBuf[1];

    for (i = 2; i < reqFrameBufSize - 2; i++)
    {
        pReqFrame->data[i-2] = pReqFrameBuf[i];
    }
    //TODO:检查crc
    pReqFrame->crc = (pReqFrameBuf[i]<<8) + pReqFrameBuf[i+1];
    pReqFrame->len = reqFrameBufSize - 4;
    return true;
}

//void McgsModbusRTU_Frame(uint8_t *pReqFrameBuf, uint16_t reqFrameBufSize,struct udp_pcb *ethg_pcb,const ip_addr_t *addr, u16_t port)
void McgsModbusRTU_Frame(uint8_t *pReqFrameBuf, uint16_t reqFrameBufSize,uint8_t Chanel)
{
    uint16_t regAddr = 0;
    uint16_t reqSize = 0;
//    uint16_t sendSize = 0;
    uint8_t ret = 0;
    uint16_t i = 0;
		struct pbuf *pSend=NULL;
	  static ModbusRTU_Frame_t g_RepFrame;	

//		//使用广播地址
//		if(pReqFrameBuf[0] == 0xFF)
//		{
//				mcgs_send_buff.len = 1;
//				mcgs_send_buff.data[0] =0x01;
//				if(pFnModbusSend && mcgs_send_buff.len>0)
//					pFnModbusSend(mcgs_send_buff.data, mcgs_send_buff.len,Chanel);	
//				return;
//		}
	
    ModbusRTU_Frame_t *pReqFrame = &g_ReqFrame;

    if (false == buf2ModbusRTU_Frame(pReqFrameBuf, reqFrameBufSize, pReqFrame))
    {
        //解析帧失败则直接返回
        return ;
    }

    if ((pReqFrame->slaveAddr != 0x01) && (pReqFrame->slaveAddr!=0xFF))
    {
        return ;    /* 地址不对直接返回 */
    }

	if(pReqFrame->crc != crc16(pReqFrameBuf,  reqFrameBufSize - 2))
	{
		return;
	}

    g_RepFrame.slaveAddr = pReqFrame->slaveAddr;
    g_RepFrame.funCode = pReqFrame->funCode;

    switch(pReqFrame->funCode)
    {
        case FC_ReadCoilStatus:
        {
            /* 1 读取线圈状态 0区 */
        }
        break;

        case FC_ReadInputStatus:
        {
            // 2 读取输入状态 1区
        }
        break;

        case FC_ReadHoldingReg:
        {
            // 3 读取保持寄存器 4区
            regAddr = (pReqFrameBuf[2]<<8 ) + pReqFrameBuf[3];
            reqSize = (pReqFrameBuf[4]<<8 ) + pReqFrameBuf[5];
            ret = readOutputRegs(regAddr, reqSize, &g_RepFrame, g_RepFrame.slaveAddr);

        }
        break;

        case FC_ReadInputReg:
        {
            // 4 读输入寄存器 3区
            regAddr = (pReqFrameBuf[2]<<8 ) + pReqFrameBuf[3];
            reqSize = (pReqFrameBuf[4]<<8 ) + pReqFrameBuf[5];
            ret = readInputRegs(regAddr, reqSize, &g_RepFrame);
        }
        break;
        case FC_ForceSingleCoil:
        {
            //5 强制单个线圈 0区
        }
        break;
        case FC_PresetSingleReg:
        {
            //6 预置单个寄存器 4区
            uint16_t setValue = 0;
            regAddr = (pReqFrameBuf[2]<<8 ) + pReqFrameBuf[3];
            setValue = (pReqFrameBuf[4]<<8 ) + pReqFrameBuf[5];
            ret = writeOutputReg(regAddr, setValue, &g_RepFrame, g_RepFrame.slaveAddr);
        }
        break;

        case FC_ForceMultipleCoils:
        {
            //15 强制多个线圈 0区
        }
        break;
        case FC_PresetMultipleRegisters:
        {
            //16 预置多个寄存器 4区
            //uint8_t byteCnt = 0;
            regAddr = (pReqFrameBuf[2]<<8 ) + pReqFrameBuf[3];
            reqSize = (pReqFrameBuf[4]<<8 ) + pReqFrameBuf[5];
            //byteCnt = pReqFrameBuf[6];
            for(i = 0; i < reqSize; i++)
            {
                g_OutputRegSetValues[i] = (pReqFrameBuf[7+i*2]<<8) + pReqFrameBuf[7+1+i*2];
            }
            ret = writeOutputRegs(regAddr, reqSize, g_OutputRegSetValues, &g_RepFrame, g_RepFrame.slaveAddr);
						
        }
        break;

        default:
            break;
    }

    if (ret == false)
    {
        return ;
    }
		
		pReqFrame->slaveAddr = 0x01;
		g_RepFrame.slaveAddr = pReqFrame->slaveAddr;
		mcgs_send_buff.len = modbusRTU_Frame2Buf(&g_RepFrame, mcgs_send_buff.data);
		if(pFnModbusSend && mcgs_send_buff.len>0)
			pFnModbusSend(mcgs_send_buff.data, mcgs_send_buff.len,Chanel);			

}

/* 拼成一个modbus读取寄存器的buf，返回buf长度 */
int HostReadRegs2Buf(uint8_t slaveAddr, uint16_t regAddr, uint16_t reqSize, uint8_t *buf)
{
    uint16_t crc = 0;
    uint16_t len = 0;

    buf[0] = slaveAddr;
    buf[1] = FC_ReadHoldingReg;
    len += 2;

    buf[2] = (regAddr >> 8) & 0xff;
    buf[3] = regAddr & 0xff;
    len += 2;

    buf[4] = (reqSize >> 8) & 0xff;
    buf[5] = reqSize & 0xff;
    len += 2;

    crc = crc16(buf, len);
    buf[6] = (crc >> 8) & 0xff;
    buf[7] = crc & 0xff;
    len += 2;

    return len;
}


int HostReadInput2Buf(uint8_t slaveAddr, uint16_t regAddr, uint16_t reqSize, uint8_t *buf)
{
    uint16_t crc = 0;
    uint16_t len = 0;

    buf[0] = slaveAddr;
    buf[1] = FC_ReadInputReg;
    len += 2;

    buf[2] = (regAddr >> 8) & 0xff;
    buf[3] = regAddr & 0xff;
    len += 2;

    buf[4] = (reqSize >> 8) & 0xff;
    buf[5] = reqSize & 0xff;
    len += 2;

    crc = crc16(buf, len);
    buf[6] = (crc >> 8) & 0xff;
    buf[7] = crc & 0xff;
    len += 2;

    return len;
}

/* 拼成一个modbus读取寄存器的buf，返回buf长度 */
int HostSetReg2Buf(uint8_t slaveAddr, uint16_t regAddr, uint16_t value, uint8_t *buf)
{
    uint16_t crc = 0;
    uint16_t len = 0;

    buf[0] = slaveAddr;
    buf[1] = FC_PresetSingleReg;
    len += 2;

    buf[2] = (regAddr >> 8) & 0xff;
    buf[3] = regAddr & 0xff;
    len += 2;

    buf[4] = (value >> 8) & 0xff;
    buf[5] = value & 0xff;
    len += 2;

    crc = crc16(buf, len);
    buf[6] = (crc >> 8) & 0xff;
    buf[7] = crc & 0xff;
    len += 2;

    return len;
}



/* 拼成一个modbus读取寄存器的buf，返回buf长度 */
int HostSetMultiRegs2Buf(uint8_t slaveAddr, uint16_t regAddr, uint16_t *pvalues,uint8_t valueSize, uint8_t *buf)
{
    uint16_t crc = 0;
    uint16_t len = 0;

    buf[0] = slaveAddr;
    buf[1] = FC_PresetSingleReg;
    len += 2;

    buf[2] = (regAddr >> 8) & 0xff;
    buf[3] = regAddr & 0xff;
    len += 2;

		for(int i=0;i<valueSize;i++)
		{
				buf[len] = (pvalues[i] >> 8) & 0xff;
				buf[len+1] = pvalues[i] & 0xff;
				len += 2;
		}
    

    crc = crc16(buf, len);
    buf[6] = (crc >> 8) & 0xff;
    buf[7] = crc & 0xff;
    len += 2;

    return len;
}


int HostSetCoil2Buf(uint8_t slaveAddr, uint16_t regAddr, uint16_t value, uint8_t *buf)
{
    uint16_t crc = 0;
    uint16_t len = 0;

    buf[0] = slaveAddr;
    buf[1] = FC_ForceSingleCoil;
    len += 2;

    buf[2] = (regAddr >> 8) & 0xff;
    buf[3] = regAddr & 0xff;
    len += 2;

    buf[4] = (value >> 8) & 0xff;
    buf[5] = value & 0xff;
    len += 2;

    crc = crc16(buf, len);
    buf[6] = (crc >> 8) & 0xff;
    buf[7] = crc & 0xff;
    len += 2;

    return len;
}


/* 主机处理接收的回复包 */
uint8_t HostProModbusRTU_RepFrame(uint8_t *pRepFrameBuf, uint8_t repFrameBufSize,
                                  ModbusRTU_Frame_t *pReqFrame)
{
		 uint8_t Ret= false;
		do{
				if (buf2ModbusRTU_Frame(pRepFrameBuf, repFrameBufSize, pReqFrame) == false)
				{
						break;
				}
				
				if (pReqFrame->slaveAddr != 0x01 )
				{
						break ;    /* 地址不对直接返回 */
				}

				if(pReqFrame->crc != crc16(pRepFrameBuf,  repFrameBufSize - 2))
				{
					break;
				}
				
				Ret = false;
		}while(false);
   
    return Ret;
}


int  HostSetMultipleRegs2Buf(uint8_t    devaddress, uint16_t staraddress_h, uint16_t staraddress_l, uint16_t value,uint8_t *buf)
{
    uint16_t crc = 0;
    uint16_t len = 0;

    buf[0] = devaddress;
    buf[1] = FC_PresetMultipleRegisters;
    len += 2;

    buf[2] = staraddress_h;
    buf[3] = staraddress_l;
    len += 2;
	
		buf[4] = 0x0;
    buf[5] = 0x1;
    len += 2;
	
		buf[6] = 0x2;
    len += 1;

    buf[7] = (value >> 8) & 0xff;
    buf[8] = value & 0xff;
    len += 2;

    crc = crc16(buf, len);
		buf[9] = (crc >> 8) & 0xff;
		buf[10] = crc & 0xff;
    len += 2;

    return len;
}

#endif