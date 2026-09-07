#ifndef _UDPIO_SERVER_H
#define _UDPIO_SERVER_H


#include "lwip/opt.h"
#include <lwip/sockets.h>
#include "lwip/sys.h"
#include "lwip/api.h"
#include <lwip/udp.h>
#include "JCHAppData.h"
#if _UDP_TEST
extern uint8_t udp_test(void);
#endif

typedef void (*udprecv_callback)(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, uint16_t port);

extern void UdpServerInit();
extern void SendData2Modbus(uint8_t* pWriteBuf, uint16_t len,uint8_t Chanel);
extern void udp_recv_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
#endif 