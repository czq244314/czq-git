#ifndef _JCHAPPDATA_H
#define _JCHAPPDATA_H

#include "freertos.h"
#include "queue.h"
#include "cmsis_os.h"
#include "JCHAppConfig.h"
#include "QueueGroup.h"
#include "Adapteriotask.h"
#if _USE_NET_DATA
#include "udp.h"

#define NETPACK_LEN  260
typedef struct _ST_NET_PACK{
		ip_addr_t addr;
		uint16_t port;
		uint16_t PackLen;
		uint8_t Data[NETPACK_LEN];
}ST_NET_PACK;

extern  ST_NET_PACK modbus_pack;
extern QueueHandle_t g_NetMcgsRecvQueue;
extern QueueHandle_t g_NetMcgsSendQueue;
extern uint8_t NetRecvFlag;
	
	#define NET_MCGS_SEND_QUEUE   g_NetMcgsSendQueue
	#define NET_MCGS_RECV_QUEUE   g_NetMcgsRecvQueue
#endif

#if _USE_UART1_DATA
	extern QueueHandle_t g_UartRecvQueue;
	extern QueueHandle_t g_UartSendQueue;
	extern uint8_t uart1RecvFlag;
	
	extern queue_mcgs_t uart1RecvData;

#define UART1_RECV_QUEUE  g_UartRecvQueue
#define UART1_SEND_QUEUE  g_UartSendQueue

#define UART1_RECVBUFF uart1RecvData.data
#endif

#if _USE_UART2_DATA
	extern QueueHandle_t g_Uart2RecvQueue;
	extern QueueHandle_t g_Uart2SendQueue;
	extern uint8_t uart2RecvFlag;
	
	extern queue_mcgs_t uart2RecvData;

	
	#define UART2_RECV_QUEUE  g_Uart2RecvQueue
	#define UART2_SEND_QUEUE  g_Uart2SendQueue
	
#endif

#if _USE_UART3_DATA
	extern QueueHandle_t g_Uart3RecvQueue;
	extern QueueHandle_t g_Uart3SendQueue;
	extern uint8_t uart3RecvFlag;
	
	extern queue_mcgs_t uart3RecvData;

	
	#define UART3_RECV_QUEUE  g_Uart3RecvQueue
	#define UART3_SEND_QUEUE  g_Uart3SendQueue
	
#endif

#if _USE_UART6_DATA
	extern QueueHandle_t g_Uart6RecvQueue;
	extern QueueHandle_t g_Uart6SendQueue;
	extern uint8_t uart6RecvFlag;
	
	extern queue_mcgs_t uart6RecvData;

	#define UART6_RECV_QUEUE  g_Uart6RecvQueue
	#define UART6_SEND_QUEUE  g_Uart6SendQueue
	
#endif

#endif
