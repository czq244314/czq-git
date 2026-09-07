#include "QueueGroup.h"
#include "JCHAppData.h"

#include "freertos.h"
#include "queue.h"
#include "cmsis_os.h"
void QueueInit()
{
	#if _USE_NET_DATA
		g_NetMcgsRecvQueue = xQueueCreate(10,sizeof(queue_mcgs_t));
		g_NetMcgsSendQueue = xQueueCreate(10,sizeof(ST_NET_PACK));
	#endif
	 
	#if _USE_UART1_DATA
		g_UartRecvQueue = xQueueCreate(10,sizeof(queue_mcgs_t));
		g_UartSendQueue = xQueueCreate(10,sizeof(queue_data_t));
	#endif
	 
	#if _USE_UART2_DATA
		g_Uart2RecvQueue = xQueueCreate(10,sizeof(queue_mcgs_t));
		g_Uart2SendQueue = xQueueCreate(10,sizeof(queue_data_t));
	#endif
	 
#if _USE_UART3_DATA
		g_Uart3RecvQueue = xQueueCreate(10,sizeof(queue_mcgs_t));
		g_Uart3SendQueue = xQueueCreate(10,sizeof(queue_data_t));
	#endif
	 
#if _USE_UART6_DATA
		g_Uart6RecvQueue = xQueueCreate(10,sizeof(queue_mcgs_t));
		g_Uart6SendQueue = xQueueCreate(10,sizeof(queue_data_t));
	#endif

}
 
