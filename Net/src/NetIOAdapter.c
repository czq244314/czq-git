#include "NetIOAdapter.h"
#include "UdpIOServer.h"
#include "JCHAppData.h"
#include "mcgs.h"

#define MODBUS_DATA_MAX_SIZE  (sizeof(modbus_data.data))
ip_addr_t Default_ip;
struct udp_pcb *udp_server_pcb;
ip_addr_t Remote_ip;
void (*pOnRecvNetData)(ST_NET_PACK *pDAta);

volatile uint8_t Udp_ReceFlag=0;
static	ST_NET_PACK netData={0};
static	ST_NET_PACK SendnetData={0};
static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
void RecvData2Modbus(ST_NET_PACK *pDAta)
{
		queue_mcgs_t  modbus_data={0};
    modbus_pack.addr = pDAta->addr;
    modbus_pack.port = pDAta->port; 
    
    if (pDAta->PackLen > MODBUS_DATA_MAX_SIZE) {

        modbus_data.len = MODBUS_DATA_MAX_SIZE;
        memcpy(modbus_data.data, pDAta->Data, MODBUS_DATA_MAX_SIZE);
    } else {
        modbus_data.len = pDAta->PackLen;
        memcpy(modbus_data.data, pDAta->Data, pDAta->PackLen);
    }
    modbus_data.Ext = 1;
    
    if (xQueueSend(NET_MCGS_RECV_QUEUE, &modbus_data, 0) != pdPASS) {

    }
}

void udp_callback_default(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
		ip_addr_t local_ipaddr;

		unsigned char *temp= (unsigned char *) addr;
		IP4_ADDR(&netData.addr,temp[0],temp[1],temp[2],temp[3]);
		
		if(Default_ip.addr!=netData.addr.addr)
			 Default_ip.addr = netData.addr.addr;

		netData.port = port;
		netData.PackLen = p->len;
		memcpy(netData.Data,p->payload,p->len);

#ifdef _USE_RTT
    SEGGER_RTT_SetTerminal(7);
		SEGGER_RTT_printf(0,"recv from:%d,len:%d\r\n",port,netData.PackLen);
#endif
		
		//bms_scan_cmd=1;
		
		RecvData2Modbus(&netData);
		pbuf_free(p);
}

//void UdpSendLoop()
//{
//	struct pbuf *pSend=NULL;
//	uint8_t ret=0;
//	int8_t err=0;
//	do
//	{
////		IP4_ADDR(&netData.addr,0xC0,0xA8,0x01,0x0A);
////		netData.port =502;
////		netData.PackLen =8;
////		uint8_t pbuf[8] ={0x01,0x06,0x00,0x00,0x00,0x00,0x89,0xCA};
////		memcpy(&netData.Data,pbuf,netData.PackLen);
//		  
//		if(xQueueReceive( NET_MCGS_SEND_QUEUE,&SendnetData,0)!=pdPASS)
//		{

//			osDelay(1);
//			break;
//		}

//		pSend = pbuf_alloc(PBUF_TRANSPORT, SendnetData.PackLen+1, PBUF_RAM);
//		if(!pSend)
//		{

//		 break; 
//		}

//		memset(pSend->payload, 0 , NETPACK_LEN-1);
//		memcpy(pSend->payload, SendnetData.Data, SendnetData.PackLen);
//		err = udp_sendto(udp_server_pcb,pSend,&SendnetData.addr,SendnetData.port);

//		pbuf_free(pSend);
////		pSend=NULL;
//		 
//	}while(0);

//}


#define NETPACK_LEN_MAX  1024  

void UdpSendLoop()
{
    struct pbuf *pSend = NULL;
    err_t err = ERR_OK;

    if (xQueueReceive(NET_MCGS_SEND_QUEUE, &SendnetData, 0) != pdPASS) {
        osDelay(1);
        return;
    }


    if (SendnetData.PackLen == 0 || SendnetData.PackLen > NETPACK_LEN_MAX) {
        return; 
    }
		
    pSend = pbuf_alloc(PBUF_TRANSPORT, SendnetData.PackLen, PBUF_RAM);
    if (pSend == NULL) {
        return; 
    }

    memset(pSend->payload, 0, SendnetData.PackLen);
    memcpy(pSend->payload, SendnetData.Data, SendnetData.PackLen);

    err = udp_sendto(udp_server_pcb, pSend, &SendnetData.addr, SendnetData.port);

    pbuf_free(pSend);

    (void)err; 
}

void  UdpIOInit(udprecv_callback pFn,uint32_t Port)
{

    err_t err;
    
    // 创建 UDP 控制块
    udp_server_pcb = udp_new();
    if (udp_server_pcb == NULL) {
        return; // 创建失败
    }
    
    // 绑定端口（例如端口 7 - Echo 服务）
    err = udp_bind(udp_server_pcb, IP_ADDR_ANY, 6000);
    if (err != ERR_OK) {
        udp_remove(udp_server_pcb);
        udp_server_pcb = NULL;
        return;
    }
    
    // 注册接收回调
    udp_recv(udp_server_pcb, udp_callback_default, NULL);

}

bool mcgs_Recv(queue_mcgs_t *pQueueBuff)
{
		bool fRecvData=false;

		 if(xQueueReceive( NET_MCGS_RECV_QUEUE,pQueueBuff,0)==pdPASS)
		 {
				fRecvData=true;
		 }
		return fRecvData;
}

void mcgs_init()
{
	pFnModbusSend = SendData2Modbus;
	pFnWriteOutputReg = eth_writeOutputReg;
	pFnWriteOutputRegs = eth_writeOutputRegs;
	pFnReadOutputRegs=eth_readOutputRegs;
 
}

