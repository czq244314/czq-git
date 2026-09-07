#ifndef _UCTSK_UDP_H
#define _UCTSK_UDP_H
#include "stdint.h"
#include "lwip/ip_addr.h"
#include "JCHAppconfig.h"

#define CAN2NET 1

#define _USE_TCP 0
#define _USE_UDP 1


// extern OS_EVENT *g_qmsg_recv;
// extern OS_EVENT *g_qmsg_send; 

extern void UdpInit();
extern void UdpSendLoop();
extern void UdpRecvLoop();

extern void SendData2Modbus(uint8_t* pWriteBuf, uint16_t len,uint8_t Chanel);


extern void UpdateADValue(uint8_t *pData,uint16_t len);
extern void UpdateStatus(uint8_t *pData,uint16_t len);

#if _USE_CAN_DATA

extern void UpdateCanMessage(uint8_t *pData,uint16_t len,uint32_t tm);
#endif

#endif