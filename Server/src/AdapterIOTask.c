#include "AdapterIOTask.h"
#if 1
#include "JCHAppData.h"
#include "includes.h"
#include "JCHAppData.h"
//#include "BmsData.h"

//#include "uctsk_UDP.h"
#include "main.h"
#if _USE_CAN_DATA
static queue_can_t CanWriteBuff={0};
#endif
#endif
#if _USE_UART1_DATA
extern UART_HandleTypeDef huart1;
static queue_data_t Uart1IOBuff={0};
#endif


#if _USE_UART2_DATA
extern UART_HandleTypeDef huart2;
//static queue_can_t MeterWriteBuff={0};
static queue_data_t Uart2IOBuff={0};
#endif

#if defined(_USE_UART3_DATA)
extern UART_HandleTypeDef huart3;
//static queue_can_t Uart3WriteBuff={0};
static queue_data_t Uart3IOBuff={0};
#endif

#if _USE_CAN_DATA
void JCH_AdapterCanIOTask(CAN_HandleTypeDef *hcanx)
{
#if defined( _USE_RTT)&0
       SEGGER_RTT_SetTerminal(1);
				SEGGER_RTT_printf(0,"JCH_AdapterIOTask,tick:%d\r\n",HAL_GetTick());
#endif
	
	//写驱动
	if(Can1SendFlag==1)
	{
			Can1SendFlag=0;
			
			while(xQueueReceive(CAN_SEND_QUEUE,&CanWriteBuff,0)==pdPASS)
			{
				Can_Write(hcanx,&CanWriteBuff.msg_data);
				if(bms_can_cmd)
				{
					UpdateCanMessage((uint8_t *)&CanWriteBuff.msg_data,CanWriteBuff.len,g_timer_1ms);		
				}					
#if defined( _USE_RTT)&0
       SEGGER_RTT_SetTerminal(1);
				SEGGER_RTT_printf(0,"[io task]Can_Write,tick:%d\r\n",HAL_GetTick());
#endif
				DelayMs(1);
			}
	}
}
#endif
void JCH_AdapterUartIOTask()
{
		#if _USE_UART1_DATA
	 if(uart1RecvFlag ==1)
			{
				uart1RecvFlag =0;
			
				xQueueSend(UART1_RECV_QUEUE,&uart1RecvData,10);
#if defined( _USE_RTT)&0
       SEGGER_RTT_SetTerminal(1);
				SEGGER_RTT_printf(0,"uart1 recv,tick:%d\r\n",HAL_GetTick());
#endif

#if defined( _USE_RTT)
       SEGGER_RTT_SetTerminal(1);
			 SEGGER_RTT_printf(0,"[io task]uart1 xQueueRecv,size:%d\r\n",uart1RecvData.len);
#endif
				// 清除缓存区内容，方便进行下次接收
				memset(&uart1RecvData,0,sizeof(queue_data_t));
			}else{
				 if(xQueueReceive(UART1_SEND_QUEUE,&Uart1IOBuff,0)==pdPASS)
				 {
#if defined( _USE_RTT)
       SEGGER_RTT_SetTerminal(1);
				SEGGER_RTT_printf(0,"[io task]uart1 xQueueSend,size:%d\r\n",Uart1IOBuff.len);
#endif
					 #if 1
#ifdef RS485_EN_1_Pin
						HAL_GPIO_WritePin(RS485_EN_1_GPIO_Port,RS485_EN_1_Pin,GPIO_PIN_SET);
#endif
						osDelay(5);
						HAL_UART_Transmit(&huart1,(const uint8_t *)(&Uart1IOBuff.data), Uart1IOBuff.len, 1000);
#ifdef RS485_EN_1_Pin
						HAL_GPIO_WritePin(RS485_EN_1_GPIO_Port,RS485_EN_1_Pin,GPIO_PIN_RESET);
#endif
					 #endif
						memset(&Uart1IOBuff,0,sizeof(queue_data_t));
				 }
				 osDelay(1);
			}
	#endif
	
	
	#if _USE_UART2_DATA	
	
		if(uart2RecvFlag ==1)
			{
				uart2RecvFlag =0;
			
				xQueueSend(UART2_RECV_QUEUE,&uart2RecvData,10);
				// 清除缓存区内容，方便进行下次接收
				memset(&uart2RecvData,0,sizeof(queue_data_t));
			}else{
					if(xQueueReceive(UART2_SEND_QUEUE,&Uart2IOBuff,0)==pdPASS)
				 {
	#if defined( _USE_RTT)&0
       SEGGER_RTT_SetTerminal(4);
				SEGGER_RTT_printf(0,"uart2 send,tick:%d\r\n",HAL_GetTick());
#endif
#ifdef RS485_2_EN_Pin
						HAL_GPIO_WritePin(RS485_2_EN_GPIO_Port,RS485_2_EN_Pin,GPIO_PIN_SET);
#endif
						osDelay(5);
						HAL_UART_Transmit(&huart2,(const uint8_t *)(&Uart2IOBuff.data), Uart2IOBuff.len, 1000);
#ifdef RS485_2_EN_Pin
						HAL_GPIO_WritePin(RS485_2_EN_GPIO_Port,RS485_2_EN_Pin,GPIO_PIN_RESET);
#endif					 
						memset(&Uart2IOBuff,0,sizeof(queue_data_t));
				 }
				 osDelay(1);
			}
			
	#endif
		#if _USE_UART3_DATA
	if(uart3RecvFlag==1)
	{
			uart3RecvFlag=0;
			xQueueSend(UART3_RECV_QUEUE,&uart3RecvData,10);
			//uart3RecvFlag = 1;
			
#if defined( _USE_RTT)
       SEGGER_RTT_SetTerminal(1);
		SEGGER_RTT_printf(0,"[io task]uart3 xQueueRecv,size:%d\r\n",uart3RecvData.len);
#endif
		memset(&uart3RecvData,0,sizeof(queue_data_t));
	}else{
			if(xQueueReceive(UART3_SEND_QUEUE,&Uart3IOBuff,0)==pdPASS)
			 {
#ifdef RS485_3_EN_Pin
					HAL_GPIO_WritePin(RS485_2_EN_GPIO_Port,RS485_2_EN_Pin,GPIO_PIN_SET);
					osDelay(5);
#endif
					HAL_UART_Transmit(&huart3,(const uint8_t *)(&Uart3IOBuff.data), Uart3IOBuff.len, 1000);
#ifdef RS485_3_EN_Pin
					HAL_GPIO_WritePin(RS485_2_EN_GPIO_Port,RS485_2_EN_Pin,GPIO_PIN_RESET);
#endif
				 
					memset(&Uart3IOBuff,0,sizeof(queue_data_t));
			 }
			 osDelay(1);
	}
	#endif
	
}

