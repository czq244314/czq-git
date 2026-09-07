#ifndef _NETIO_ADAPTER_H
#define _NETIO_ADAPTER_H

#include "lwip/opt.h"
#include <lwip/sockets.h>
#include "lwip/sys.h"
#include "lwip/api.h"
#include <lwip/udp.h>
#include "stdbool.h"
#include "QueueGroup.h"

extern queue_mcgs_t  modbus_data;

extern struct udp_pcb *udp_server_pcb;
typedef void (*udprecv_callback)(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, uint16_t port);
extern void  UdpIOInit(udprecv_callback pFn,uint32_t Port);
extern void UdpSendLoop();
extern void mcgs_init();
extern bool mcgs_Recv(queue_mcgs_t *pQueueBuff);
//extern void RecvData2Modbus();
#endif