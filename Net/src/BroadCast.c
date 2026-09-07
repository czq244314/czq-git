#include "BroadCast.h"
#if 0
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/ip_addr.h"
#include "lwip/ip.h"
#include "lwip/udp.h"
//#include "lwip/opt.h"
//#include "modbusrtu.h"
#include "jchappdata.h"
#include <string.h>  
#if UDP_BROADCAST_EN

uint32_t g_BroadCast_Time=0;
uint32_t g_BroadCast_TimeOut=0;

#define BROADCAST_PORT 31700
ip4_addr_t broadcast_addr;
struct udp_pcb udp_broadcast_pcb;

struct pbuf *pSendMsg=NULL;
char pMessage[64]={0};
void BroadCast_Init()
{
   IP4_ADDR(&broadcast_addr, 192, 168, 1, 255);
	
	 ip_set_option(&udp_broadcast_pcb, SOF_BROADCAST);

   //pMessage = "BMS Semulator BROAD CAST,From [192.168.1.xxx]\n";
	 sprintf(pMessage,"BMS Semulator(207),Addr:%d,IP:192.168.%d.%d",(unsigned int)g_SlaveAddr,(unsigned int)g_IP_ADDR[0],(unsigned int)g_IP_ADDR[1]);

   
}

void BroadCast_Send()
{
	 err_t send_err;
/*	
	if(pSendMsg!=NULL)
	{
		pbuf_free(pSendMsg);
		pSendMsg=NULL;
	}
	*/
	pSendMsg = pbuf_alloc(PBUF_TRANSPORT, strlen(pMessage), PBUF_RAM);
   if (pSendMsg!= NULL) {
       memcpy(pSendMsg->payload, pMessage, strlen(pMessage));
   } else {
       //printf("Message buffer allocation failed\n");
   }
	 
   send_err = udp_sendto(&udp_broadcast_pcb, pSendMsg, &broadcast_addr, BROADCAST_PORT);
   if (send_err!= ERR_OK) {

       //printf("UDP sendto failed with error code: %d\n", send_err);
  } 

	if (pSendMsg != NULL) {
		pbuf_free(pSendMsg);
		pSendMsg=NULL;
	}

}


#endif
#endif