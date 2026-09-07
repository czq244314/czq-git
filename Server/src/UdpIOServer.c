#include "UdpIOServer.h"
#include "string.h"
#include "NetIOAdapter.h"
#include "JCHAppData.h"


static ip_addr_t Remote_ip;
struct udp_pcb* pPcb;
static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
void udp_recv_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{


}                   

void SendData2Modbus(uint8_t* pWriteBuf, uint16_t len,uint8_t Chanel)
{
	modbus_pack.PackLen = len;
	memcpy( modbus_pack.Data,pWriteBuf,modbus_pack.PackLen);
//	xQueueSend(NET_MCGS_SEND_QUEUE,&modbus_pack,1);
	if (xQueueSend(NET_MCGS_SEND_QUEUE, &modbus_pack, 0) != pdPASS) {

	}
//	xQueueSendFromISR(NET_MCGS_SEND_QUEUE,&modbus_pack,);
}

void UdpServerInit()
{
	UdpIOInit(udp_recv_callback,6000);
}


