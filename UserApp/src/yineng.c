#include "yineng.h"
#include "JCHAppData.h"

//volatile uint8_t execute_flag;
volatile uint8_t yinengflag;
volatile uint8_t sendAmmeterFlag;
volatile uint8_t sendAmmeterFlag1;
volatile uint8_t sendAmmeterFlag2;
uint16_t startgetflag1=0;
#if 1
#include "stdbool.h"
#include "includes.h"
#include "JCHAppData.h"
#include "queue.h"

#if defined(_USE_RTT) 
	#include "segger_rtt.h"
#endif

g_232data YN_data;
static RS232_Frame_t g_RepFrame;
extern UART_HandleTypeDef huart3;

osThreadId RS232HandleHandle;

static const uint8_t YNtable_crc_hi[] =
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
static const uint8_t YNtable_crc_lo[] =
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

static uint16_t YNcrc16(uint8_t *buffer, uint16_t buffer_length)
{
    uint8_t crc_hi = 0xFF; /* high CRC byte initialized */
    uint8_t crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i; /* will index into CRC lookup */

    /* pass through message buffer */
    while (buffer_length--)
    {
        i = crc_hi ^ *buffer++; /* calculate the CRC  */
        crc_hi = crc_lo ^ YNtable_crc_hi[i];
        crc_lo = YNtable_crc_lo[i];
    }

    return (crc_hi << 8 | crc_lo);
}

//查询全部数据
void FirstSend(void)
{
	queue_data_t cmdData={0};
	uint8_t pbuf[50]={0};
	uint8_t i=0;
	uint16_t crc;
	pbuf[i++]=0x55;
	pbuf[i++]=0xAA;//1
	i++;
	i++;
	pbuf[i++]=0x11;//3
	pbuf[i++]=0x12;//4
	pbuf[i++]=0x01;
	pbuf[i++]=0x00;//5
	pbuf[i++]=0x01;
	pbuf[i++]=0x00;//6
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;//7
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;//8
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;//9
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;//11
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;//12
	crc=YNcrc16(&pbuf[6],i-6);
	pbuf[i++]=(crc>>8)&0xFF;
	pbuf[i++]=crc&0xFF; //16	
	pbuf[2]= i&0xFF;
	pbuf[3]= (i>>8)&0xFF;
	//HAL_UART_Transmit(&huart3,pbuf,i,1000);
	#if 0
	HAL_UART_Transmit(&huart3,pbuf,i,1000);
	#else
	cmdData.len =i;//
	memcpy(cmdData.data,pbuf,i);

	//HAL_UART_Transmit(&huart3,pbuf,i,1000);
	xQueueSend(PLC_SEND_QUEUE,&cmdData,10);
	#endif
 
}

//回复CMD102
void CMD101(void)
{
	queue_data_t cmdData={0};
	uint8_t pbuf[50]={0};
	uint8_t i=0;
	uint16_t crc;
	pbuf[i++]=0x55;
	pbuf[i++]=0xAA;//1
	i++;
	i++;
	pbuf[i++]=0x11;//3
	pbuf[i++]=0x01;//4
	pbuf[i++]=0x65;
	pbuf[i++]=0x00;//5
	pbuf[i++]=0x01;
	pbuf[i++]=0x00;//6	
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;//7
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;//7
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;//7
	pbuf[i++]=0x00;
	pbuf[i++]=0xBB;//10
	pbuf[i++]=0x01;
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;
	crc=YNcrc16(&pbuf[6],i-6);
	pbuf[i++]=(crc>>8)&0xFF;
	pbuf[i++]=crc&0xFF;//16	
	pbuf[2]= i&0xFF;
	pbuf[3]= (i>>8)&0xFF;
	//HAL_UART_Transmit(&huart3,pbuf,i,1000);
	
	cmdData.len =i;//
	memcpy(cmdData.data,pbuf,i);

	//HAL_UART_Transmit(&huart3,pbuf,i,1000);
	xQueueSend(PLC_SEND_QUEUE,&cmdData,10);

}

//CMD1发送修改充电需求参数
void ChangeData(uint32_t addr,uint32_t data)
{

	queue_data_t cmdData={0};
	
	uint8_t pbuf[50]={0};
	uint8_t i=0;
	uint16_t crc;
	pbuf[i++]=0x55;
	pbuf[i++]=0xAA;//1
	i++;
	i++;
	pbuf[i++]=0x11;//3
	pbuf[i++]=0x74;//4
	pbuf[i++]=0x01;
	pbuf[i++]=0x00;//5
	pbuf[i++]=0x01;
	pbuf[i++]=0x00;//6
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;//7
	pbuf[i++]=0x01;
	pbuf[i++]=0x00;//8
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;//9
	pbuf[i++]=addr&0xFF;
	pbuf[i++]=(addr>>8)&0xFF;//10
	pbuf[i++]=(addr>>16)&0xFF;
	pbuf[i++]=(addr>>24)&0xFF;//11
	pbuf[i++]=0x01;
	pbuf[i++]=0x00;//12
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;//13
	pbuf[i++]=0x04;
	pbuf[i++]=0x00;//14
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;//15
	pbuf[i++]=data&0xFF;//16
	pbuf[i++]=(data>>8)&0xFF;	
	pbuf[i++]=(data>>16)&0xFF;//16	
	pbuf[i++]=(data>>24)&0xFF;
	crc=YNcrc16(&pbuf[6],i-6);
	pbuf[i++]=(crc>>8)&0xFF;
	pbuf[i++]=crc&0xFF;//16	
	pbuf[2]= i&0xFF;
	pbuf[3]= (i>>8)&0xFF;
	//HAL_UART_Transmit(&huart3,pbuf,i,1000);

	cmdData.len =i;//
	memcpy(cmdData.data,pbuf,i);

	//HAL_UART_Transmit(&huart3,pbuf,i,1000);
	xQueueSend(PLC_SEND_QUEUE,&cmdData,10);

}

//CMD5发送修改工作状态参数
void ChangeChargeStatus(uint32_t data)
{
	queue_data_t cmdData={0};
	
	uint8_t pbuf[50]={0};
	uint8_t i=0;
	uint16_t crc;
	pbuf[i++]=0x55;
	pbuf[i++]=0xAA;//1
	i++;
	i++;
	pbuf[i++]=0x11;//3
	pbuf[i++]=0x01;//4
	pbuf[i++]=0x05;
	pbuf[i++]=0x00;//5
	pbuf[i++]=0x01;
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;//7
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;//9
	pbuf[i++]=0x01;
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;//7
	pbuf[i++]=0x04;
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;
	pbuf[i++]=0x00;//9
	pbuf[i++]=data&0xFF;//11	
	pbuf[i++]=(data>>8)&0xFF;
	pbuf[i++]=(data>>16)&0xFF;//10	
	pbuf[i++]=(data>>24)&0xFF;
	crc=YNcrc16(&pbuf[6],i-6); 
	pbuf[i++]=(crc>>8)&0xFF;
	pbuf[i++]=crc&0xFF;//16	
	pbuf[2]= i&0xFF;
	pbuf[3]= (i>>8)&0xFF;
	//HAL_UART_Transmit(&huart3,pbuf,i,1000);
	
	cmdData.len =i;//
	memcpy(cmdData.data,pbuf,i);

	//HAL_UART_Transmit(&huart3,pbuf,i,1000);
	xQueueSend(PLC_SEND_QUEUE,&cmdData,10);
}
 
static bool buf2RS232_Frame(uint8_t *pReqFrameBuf,
                                   uint16_t reqFrameBufSize,
                                   RS232_Frame_t *pReqFrame)
{
    int i;

    if (reqFrameBufSize < 4)
    {
        /* 长度小于4返回错误 */
        return false;
    }
    pReqFrame->head = (pReqFrameBuf[0]<<8) + pReqFrameBuf[1];
		pReqFrame->info =  pReqFrameBuf[4];
		pReqFrame->CMD  = (pReqFrameBuf[7]<<8) + pReqFrameBuf[6];
		
    for (i = 8; i < reqFrameBufSize - 2; i++)
    {
        pReqFrame->data[i-8] = pReqFrameBuf[i];
    }
    //TODO:检查crc
    pReqFrame->crc = (pReqFrameBuf[i+1]<<8) + pReqFrameBuf[i];//注意改
    pReqFrame->len = reqFrameBufSize -2;
    return true;
}

void CMD2Handle(uint8_t *data)
{

	YN_data.req_vol = data[20] | (data[21] << 8) | (data[22] << 16) | (data[23]<<24);
	YN_data.req_cur =	data[24] | (data[25] << 8) | (data[26] << 16) | (data[27]<<24);
	YN_data.volt_max_ev = data[28] | (data[29] << 8) | (data[30] << 16) | (data[31]<<24);
	YN_data.curr_max_ev = data[32] | (data[33] << 8) | (data[34] << 16) | (data[35]<<24);
	YN_data.power_max_ev = data[36] | (data[37] << 8) | (data[38] << 16) | (data[39]<<24);
	YN_data.bat_volt_be_charge = data[40] | (data[41] << 8) | (data[42] << 16) | (data[43]<<24);
	YN_data.soc_start = data[44] | (data[45] << 8) | (data[46] << 16) | (data[47]<<24);
	YN_data.chargetime_remain = data[56] | (data[57] << 8) | (data[58] << 16) | (data[59]<<24);
	YN_data.bro = data[60] | (data[61] << 8) | (data[62] << 16) | (data[63]<<24);
	YN_data.charge_allow = data[64] | (data[65] << 8) | (data[66] << 16) | (data[67]<<24);
	YN_data.charge_stop = data[68] | (data[69] << 8) | (data[70] << 16) | (data[71]<<24);
	YN_data.err_code_ev = data[72] | (data[73] << 8) | (data[74] << 16) | (data[75]<<24);
	#if defined(_USE_RTT)
			SEGGER_RTT_SetTerminal(5);
			SEGGER_RTT_printf(0,"start soc:%d(%x)\r\n",YN_data.soc_start,YN_data.soc_start);
#endif
}

void CMD102Handle(uint8_t *data)
{
	YN_data.charge_status = data[4] | (data[5] << 8) | (data[6] << 16) | (data[7] <<24);
	YN_data.volt_charge_plc = data[16] | (data[17] << 8) | (data[18] << 16) | (data[19]<<24);
	YN_data.curr_charge_plc = data[20] | (data[21] << 8) | (data[22] << 16) | (data[23]<<24);
	YN_data.charge_time = data[24] | (data[25] << 8) | (data[26] << 16) | (data[27]<<24);
	YN_data.soc_now = data[28] | (data[29] << 8) | (data[30] << 16) | (data[31]<<24);
	YN_data.slac = data[40] | (data[41] << 8) | (data[42] << 16) | (data[43]<<24);
	YN_data.cro = data[44] | (data[45] << 8) | (data[46] << 16) | (data[47]<<24);
	YN_data.remain_chargetime = data[48] | (data[49] << 8) | (data[50] << 16) | (data[51]<<24);
	YN_data.inside_status =	data[52] | (data[53] << 8) | (data[54] << 16) | (data[55]<<24);
	YN_data.R2_Contol = data[56] | (data[57] << 8) | (data[58] << 16) | (data[59]<<24);
	#if defined(_USE_RTT)
			SEGGER_RTT_SetTerminal(3);
	SEGGER_RTT_printf(0,"soc:%d(%x),tim:%d(%x)\r\n",YN_data.soc_now,YN_data.soc_now,YN_data.charge_time,YN_data.charge_time);
#endif
}

void YANZHENG(void)
{
	queue_data_t cmdData={0};
	uint8_t pbuf[8]={0};
	uint8_t i=0;
	uint16_t crc;
	pbuf[i++]=0x32;
	pbuf[i++]=0x03;//1
	pbuf[i++]=0x02;//3
	pbuf[i++]=0x00;//4
	pbuf[i++]=0x10;
	crc=YNcrc16(pbuf,i);
	pbuf[i++]=(crc>>8)&0xFF;
	pbuf[i++]=crc&0xFF; //16	
	cmdData.len =i;//
	memcpy(cmdData.data,pbuf,i);
	xQueueSend(PLC_SEND_QUEUE,&cmdData,10);

}

//串口处理线程调用函数
void ProRS232_Frame(uint8_t *pReqFrameBuf, uint16_t reqFrameBufSize)
{
    uint16_t regAddr = 0;
    uint16_t reqSize = 0;
//    uint16_t sendSize = 0;
    uint8_t ret = 0;
    uint16_t i = 0;
	  static RS232_Frame_t g_ReqFrame;//定义函数内部结构体

    RS232_Frame_t *pReqFrame = &g_RepFrame;//ModbusRTU_Frame_t类型的指针指向g_ReqFrame结构体

		if(pReqFrameBuf[0]==0x32 && pReqFrameBuf[1]==0x03)
		{
			YANZHENG();
			return;
		}
		//验证收取的报文正确性
    if (false == buf2RS232_Frame(pReqFrameBuf, reqFrameBufSize, pReqFrame))
    {
        //解析帧失败则直接返回
        return ;
    }
		//CRC校验
//		if(pReqFrame->crc != YNcrc16(pReqFrameBuf,  reqFrameBufSize - 2)){return;}
		
	#if defined(_USE_RTT) 
    SEGGER_RTT_SetTerminal(2);
		SEGGER_RTT_printf(0,"recv CMD:%0x(%d)(ProRS232_Frame),len:%d\r\n",pReqFrame->CMD,pReqFrame->CMD,pReqFrame->len);
#endif
    switch(pReqFrame->CMD)
    {
        case tag232_CMD2://EVCC_PLC 进程回复参数整形设置/查询报文
        {
					if(pReqFrame->len==88)
            CMD2Handle(pReqFrame->data);
        }
        break;
				
        case tag232_CMD102://EVCC_PLC 设备向充电控制板发送状态报文
        {
            CMD102Handle(pReqFrame->data);
						CMD101();
        }
        break;
			
        default:
            break;
    }
 }
#if 0
 void meter_server_repones()
{
	queue_data_t RecvData={0};
	if(xQueueReceive(METER_RECV_QUEUE,&RecvData,100)==pdPASS)
	{
#if defined( _USE_RTT)
      SEGGER_RTT_SetTerminal(4);
			SEGGER_RTT_printf(0,"recv size:%d\r\n",RecvData.len);
		
			for(int i=0;i<RecvData.len;i++)
			{
				SEGGER_RTT_printf(0,"%02X ",RecvData.data[i]);
			}
			SEGGER_RTT_printf(0,"\r\n");
#endif
			ProRS232_Frame(RecvData.data,RecvData.len);
	}
}
#endif
#endif

void YINENG(void const * argument)
{
  /* USER CODE BEGIN YINENG */
	queue_data_t RecvData={0};
  /* Infinite loop */
	
  for(;;)
  {
		//if(uart3RecvFlag==1)
		if(xQueueReceive(PLC_RECV_QUEUE,&RecvData,100)==pdPASS)
		{
 			ProRS232_Frame(RecvData.data,RecvData.len);
			uart3RecvFlag=0;
			
#if defined( _USE_RTT)
	SEGGER_RTT_SetTerminal(2);
			SEGGER_RTT_printf(0," uart 3 recv,size:%d\r\n",RecvData.len);
#endif
		}
		if(startgetflag1==0)
		{
			FirstSend();	
			startgetflag1=1;
		}		
		

    osDelay(5);
  }
  /* USER CODE END YINENG */
}

void StartYiNengTask()
{
	osThreadDef(RS232Handle, YINENG, osPriorityNormal, 0, 256);
  RS232HandleHandle = osThreadCreate(osThread(RS232Handle), NULL);
}


