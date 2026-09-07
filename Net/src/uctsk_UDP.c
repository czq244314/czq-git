#include "uctsk_UDP.h"

#if 0
#include "JCHAppData.h"
#include "Modbusrtu.h"

//#include "queue.h"
//#include "can.h"

#if _USE_RTT
#include "SEGGER_RTT.h"
#endif

extern volatile uint32_t  g_timer_1ms;
#if 0
 struct udp_pcb *g_pcb;
f
  ip_addr_t Default_ip;

 void RecvData2Modbus(ST_NET_PACK *pDAta);


void udp_recv_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
		ip_addr_t local_ipaddr;
		ST_NET_PACK netData={0};
		unsigned char *temp= (unsigned char *) addr;
		IP4_ADDR(&netData.addr,temp[0],temp[1],temp[2],temp[3]);
#ifdef _USE_RTT
    SEGGER_RTT_SetTerminal(7);
		SEGGER_RTT_printf(0,"udp on recv\r\n");
#endif		
		if(Default_ip.addr!=netData.addr.addr)
			 Default_ip.addr = netData.addr.addr;
		
		netData.port = port;
		netData.PackLen = p->len;
		memcpy(netData.Data,p->payload,p->len);

#ifdef _USE_RTT
    SEGGER_RTT_SetTerminal(7);
		SEGGER_RTT_printf(0,"recv from:%d,len:%d\r\n",port,netData.PackLen);
#endif
		
		RecvData2Modbus(&netData);
	
		pbuf_free(p);
#ifdef _USE_RTT
    SEGGER_RTT_SetTerminal(7);
		SEGGER_RTT_printf(0,"udp on recv[END]\r\n");
#endif
}                   


#endi

void UdpSendLoop()
{
	struct pbuf *pSend=NULL;
	ST_NET_PACK netData={0};	
	uint8_t ret=0;
	int8_t err=0;
	do
	{
     if(xQueueReceive( NET_MCGS_SEND_QUEUE,&netData,0)!=pdPASS)
		 {
				//OSTimeDlyHMSM(0, 0, 0, 1);
				osDelay(1);
				break;
			}

		 pSend = pbuf_alloc(PBUF_TRANSPORT, netData.PackLen, PBUF_RAM);
		 if(!pSend)
		 {
			// printf("out of PBUF_RAM\n");
#ifdef _USE_RTT
       SEGGER_RTT_SetTerminal(8);
			SEGGER_RTT_printf(0,"out of PBUF_RAM\r\n");
#endif
			 break;
		 }
#if defined( _USE_RTT)&0
       SEGGER_RTT_SetTerminal(8);
			SEGGER_RTT_printf(0,"send data len:%d,port:%d\r\n",netData.PackLen,netData.port);
#endif
		 memset(pSend->payload, 0 , NETPACK_LEN-1);
		 memcpy(pSend->payload, netData.Data, netData.PackLen);
		 err = udp_sendto(g_pcb,pSend,&netData.addr,netData.port);
		 
		 pbuf_free(pSend);
		 pSend=NULL;
		 
#if defined( _USE_RTT)&0
     SEGGER_RTT_SetTerminal(8);
		 SEGGER_RTT_printf(0,"send ok,ret:%d\r\n",err);
#endif
	}while(0);

}
#endif  
void SendData( ip_addr_t ip,uint16_t Port,uint8_t *pData,uint16_t len)
{
		 do{

#if defined( _USE_RTT)&0
       SEGGER_RTT_SetTerminal(8);
				 SEGGER_RTT_printf(0,"send len:%d\r\n",len);
#endif
			if(len<=0)
				break;
			
			if(pData==NULL)
				break;
			 ST_NET_PACK RecvData={0};
			 RecvData.addr = ip;
			 RecvData.port = Port;
			 
			 uint8_t SendLen=0;
			 do{
				 SendLen = (len/(NETPACK_LEN-1))>0?(NETPACK_LEN-1):len;

				 if(SendLen==0)
					 break;
				 len-= SendLen;
				 
				 RecvData.PackLen =SendLen;
				 
				 memcpy( RecvData.Data,pData,RecvData.PackLen);
				 xQueueSend(NET_MCGS_SEND_QUEUE,&RecvData,10);
				 
				 //QueueWrite(UDP_Buff_Send,(void *)&RecvData);
		 }while(1);
			
		}while(0);
}


void UdpRecvLoop()
{
	#if 0
		ST_NET_PACK RecvData={0};	
		if(xQueueReceive(NET_MCGS_RECV_QUEUE,&RecvData,10)==pdPASS)
		{
			
#ifdef _USE_RTT
       SEGGER_RTT_SetTerminal(7);
			SEGGER_RTT_printf(0,"recv data len:%d,port:%d\r\n",RecvData.PackLen,RecvData.port);
#endif
			RecvData2Modbus(&RecvData);
			
				// QueueWrite(UDP_Buff_Send,(void *)&RecvData);
		}
		#endif
}

 #include "juart.h"

static ST_NET_PACK modbus_pack={0}; 
static queue_mcgs_t  modbus_data={0};
void RecvData2Modbus(ST_NET_PACK *pDAta)
{
       modbus_pack.addr = pDAta->addr;
       modbus_pack.port = pDAta->port; 
			 
			 modbus_data.len = pDAta->PackLen;
			 modbus_data.Ext=1;
			 memcpy(modbus_data.data,pDAta->Data,pDAta->PackLen);
       
			xQueueSend(NET_MCGS_RECV_QUEUE,&modbus_data,10); //发送到mcgs
}


void SendData2Modbus(uint8_t* pWriteBuf, uint16_t len,uint8_t Chanel)
{
#if defined( _USE_RTT)&0
      SEGGER_RTT_SetTerminal(8);
			SEGGER_RTT_printf(0,"SendData2Modbus\r\n");
#endif
		

		if(Chanel==1)
		{
			modbus_pack.PackLen = len;
			memcpy( modbus_pack.Data,pWriteBuf,modbus_pack.PackLen);
			xQueueSend(NET_MCGS_SEND_QUEUE,&modbus_pack,10);
		}else{
			modbus_data.len = len;
			memcpy( modbus_data.data,pWriteBuf,modbus_data.len);
			xQueueSend(UART_MCGS_SEND_QUEUE,&modbus_data,10);
		}			
  
  }

	#if _USE_CAN_DATA
void UpdateCanMessage(uint8_t *pData,uint16_t len,uint32_t tm)
{
#if CAN2NET==1
	 	uint32_t time_mark=0;
	  uint8_t offset=0;
	
		CAN_MSG *pMsg = (CAN_MSG *)pData;
		static uint8 send_buff[128]={0};
		
		uint8 msgID[4]={0};
		time_mark = tm;//g_timer_1ms;
		//附加报文*时间戳
		send_buff[offset++]= (time_mark>>24) & 0xff;	
    send_buff[offset++]= (time_mark>>16) & 0xff;
    send_buff[offset++]= (time_mark>>8) & 0xff;
    send_buff[offset++]= time_mark & 0xff;
		//附加报文*数据长度= ID+数据
		send_buff[offset++]=pMsg->DLC+4; //can报文数据段长度
		//can报文*ID
		msgID[3]= pMsg->Id.ExtId&0xFF;
		msgID[2]= pMsg->Id.ExtId>>8&0xFF;
		msgID[1]= pMsg->Id.ExtId>>16&0xFF;
		msgID[0]= pMsg->Id.ExtId>>24&0xFF;

		memcpy(&send_buff[offset], msgID, 4);
		offset+=4;
	
		//can报文*
    memcpy(&send_buff[offset], pMsg->Data, pMsg->DLC);
    offset+=pMsg->DLC;
    send_buff[offset++]=0x0D;
    send_buff[offset++]=0x0A; 
#endif

#if defined( _USE_RTT)&0
       SEGGER_RTT_SetTerminal(6);
			SEGGER_RTT_printf(0,"recv CAN ID:%X\r\n",pMsg->Id.ExtId);
#endif

#if _USE_UDP
	if(Default_ip.addr!=0)
	{
#if defined( _USE_RTT)
       SEGGER_RTT_SetTerminal(6);
		SEGGER_RTT_printf(0,RTT_CTRL_TEXT_BRIGHT_WHITE "can update data len:%d,time:%d,(%X),tick:%d\r\n",len,time_mark,pMsg->Id.ExtId,osKernelSysTick());
#endif
		
		SendData( Default_ip,10000-g_SlaveAddr,send_buff,offset);
		//SendData( Default_ip,10000,pData,len);
	}else{
#if defined( _USE_RTT)&0
       SEGGER_RTT_SetTerminal(6);
			SEGGER_RTT_printf(0,RTT_CTRL_TEXT_WHITE "Default_ip.addr is NULL\r\n");
#endif
	}
			//SendData( Default_ip,10000,pData,len);
#endif
}
#endif
void UpdateADValue(uint8_t *pData,uint16_t len)
{
#if _USE_UDP
	if(Default_ip.addr!=0)
			SendData( Default_ip,10001,pData,len);
#endif
}

void UpdateStatus(uint8_t *pData,uint16_t len)
{
#if _USE_UDP
	if(Default_ip.addr!=0)
			SendData( Default_ip,10002,pData,len);
#endif
}

#endif