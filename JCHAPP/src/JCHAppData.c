#include "JCHAppData.h"
#include "freertos.h"
#include "queue.h"
#include "cmsis_os.h"

#if _USE_NET_DATA
	QueueHandle_t g_NetMcgsRecvQueue;
	QueueHandle_t g_NetMcgsSendQueue;
	uint8_t NetRecvFlag=0;
#endif

#if _USE_UART1_DATA
	QueueHandle_t g_UartRecvQueue;
	QueueHandle_t g_UartSendQueue;
	
	uint8_t uart1RecvFlag=0;
	queue_mcgs_t uart1RecvData;
#endif

#if _USE_UART2_DATA
	QueueHandle_t g_Uart2RecvQueue;
	QueueHandle_t g_Uart2SendQueue;
	
	uint8_t uart2RecvFlag=0;
	queue_mcgs_t uart2RecvData;

#endif

#if _USE_UART3_DATA
	QueueHandle_t g_Uart3RecvQueue;
	QueueHandle_t g_Uart3SendQueue;
	uint8_t uart3RecvFlag=0;
	
	queue_mcgs_t uart3RecvData;

#endif

#if _USE_UART6_DATA
	QueueHandle_t g_Uart6RecvQueue;
	QueueHandle_t g_Uart6SendQueue;
	uint8_t uart6RecvFlag=0;
	
	queue_mcgs_t uart6RecvData;

#endif
	
ST_NET_PACK modbus_pack={0};


