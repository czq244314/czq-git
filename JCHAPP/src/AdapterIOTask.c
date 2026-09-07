#include "AdapterIOTask.h"
#if 1
#include "JCHAppData.h"

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart6_rx;
#if _USE_CAN_DATA
static queue_can_t CanWriteBuff={0};
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

#if _USE_UART3_DATA
extern UART_HandleTypeDef huart3;
//static queue_can_t Uart3WriteBuff={0};
static queue_data_t Uart3IOBuff={0};
#endif

#if _USE_UART6_DATA
extern UART_HandleTypeDef huart6;
//static queue_can_t Uart3WriteBuff={0};
static queue_data_t Uart6IOBuff={0};
#endif

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
 {
		int cnt=0;
		if(huart->Instance==USART1)
		{
#if _USE_UART1_DATA
			
				uart1RecvData.len = BUFFER_SIZE-__HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
				HAL_UARTEx_ReceiveToIdle_DMA(&huart1,uart1RecvData.data,BUFFER_SIZE);
				if(uart1RecvData.len>1)
				{
					uart1RecvFlag=1;
				}
			 
#endif
		}
		else if(huart->Instance==USART2)
		{
#if _USE_UART2_DATA
			
				uart2RecvData.len = BUFFER_SIZE-__HAL_DMA_GET_COUNTER(&hdma_usart2_rx);
				HAL_UARTEx_ReceiveToIdle_DMA(&huart2,uart2RecvData.data,BUFFER_SIZE);
				if(uart2RecvData.len>1)
				{
					uart2RecvFlag=1;
				}
			 
#endif		
		}
		else if(huart->Instance==USART3)
		{
#if _USE_UART3_DATA
			
				uart3RecvData.len = BUFFER_SIZE-__HAL_DMA_GET_COUNTER(&hdma_usart3_rx);
				HAL_UARTEx_ReceiveToIdle_DMA(&huart3,uart3RecvData.data,BUFFER_SIZE);
				if(uart3RecvData.len>1)
				{
					uart3RecvFlag=1;
				}
			 
#endif	
		}
		else if(huart->Instance==USART6)
		{
#if _USE_UART6_DATA
			
				uart6RecvData.len = BUFFER_SIZE-__HAL_DMA_GET_COUNTER(&hdma_usart6_rx);
				HAL_UARTEx_ReceiveToIdle_DMA(&huart6,uart6RecvData.data,BUFFER_SIZE);
				if(uart6RecvData.len>1)
				{
					uart6RecvFlag=1;
				}
			 
#endif	
		}
}

void JCH_AdapterUartIOTask()
{
#if 1
	 if(uart1RecvFlag ==1)
			{
				uart1RecvFlag =0;
			
				xQueueSend(UART1_RECV_QUEUE,&uart1RecvData,10);
				// 清除缓存区内容，方便进行下次接收
				memset(&uart1RecvData,0,sizeof(queue_mcgs_t));
			}else{
				 if(xQueueReceive(UART1_SEND_QUEUE,&Uart1IOBuff,0)==pdPASS)
				 {

#ifdef RS485_EN1_Pin
						HAL_GPIO_WritePin(RS485_EN1_GPIO_Port,RS485_EN1_Pin,GPIO_PIN_SET);
#endif
						osDelay(5);
						HAL_UART_Transmit(&huart1,(const uint8_t *)(&Uart1IOBuff.data), Uart1IOBuff.len, 1000);
#ifdef RS485_EN1_Pin
						HAL_GPIO_WritePin(RS485_EN1_GPIO_Port,RS485_EN1_Pin,GPIO_PIN_RESET);
#endif
						memset(&Uart1IOBuff,0,sizeof(queue_data_t));
				 }
				 osDelay(1);
			}
#endif
	
	
#if 1	
	
		if(uart2RecvFlag ==1)
			{
				uart2RecvFlag =0;
			
				xQueueSend(UART2_RECV_QUEUE,&uart2RecvData,10);
				// 清除缓存区内容，方便进行下次接收
				memset(&uart2RecvData,0,sizeof(queue_mcgs_t));
			}else{
					if(xQueueReceive(UART2_SEND_QUEUE,&Uart2IOBuff,0)==pdPASS)
				 {

#ifdef RS485_EN2_Pin
						HAL_GPIO_WritePin(RS485_EN2_GPIO_Port,RS485_EN2_Pin,GPIO_PIN_SET);
#endif
						osDelay(5);
						HAL_UART_Transmit(&huart2,(const uint8_t *)(&Uart2IOBuff.data), Uart2IOBuff.len, 1000);
#ifdef RS485_EN2_Pin
						HAL_GPIO_WritePin(RS485_EN2_GPIO_Port,RS485_EN2_Pin,GPIO_PIN_RESET);
#endif					 
						memset(&Uart2IOBuff,0,sizeof(queue_data_t));
				 }
				 osDelay(1);
			}
			
#endif
			
#if 1
	if(uart3RecvFlag==1)
	{
			uart3RecvFlag=0;
			xQueueSend(UART3_RECV_QUEUE,&uart3RecvData,10);
		memset(&uart3RecvData,0,sizeof(queue_mcgs_t));
	}else{
			if(xQueueReceive(UART3_SEND_QUEUE,&Uart3IOBuff,0)==pdPASS)
			 {
#ifdef RS485_EN3_Pin
					HAL_GPIO_WritePin(RS485_EN3_GPIO_Port,RS485_EN3_Pin,GPIO_PIN_SET);
					osDelay(5);
#endif
					HAL_UART_Transmit(&huart3,(const uint8_t *)(&Uart3IOBuff.data), Uart3IOBuff.len, 1000);
#ifdef RS485_EN3_Pin
					HAL_GPIO_WritePin(RS485_EN3_GPIO_Port,RS485_EN3_Pin,GPIO_PIN_RESET);
#endif
				 
					memset(&Uart3IOBuff,0,sizeof(queue_data_t));
			 }
			 osDelay(1);
	}
#endif
	
#if 1
	if(uart6RecvFlag==1)
	{
			uart6RecvFlag=0;
			xQueueSend(UART6_RECV_QUEUE,&uart6RecvData,10);
		memset(&uart6RecvData,0,sizeof(queue_mcgs_t));
	}else{
			if(xQueueReceive(UART6_SEND_QUEUE,&Uart6IOBuff,0)==pdPASS)
			 {
#ifdef RS485_EN4_Pin
					HAL_GPIO_WritePin(RS485_EN4_GPIO_Port,RS485_EN4_Pin,GPIO_PIN_SET);
					osDelay(5);
#endif
					HAL_UART_Transmit(&huart6,(const uint8_t *)(&Uart6IOBuff.data), Uart6IOBuff.len, 1000);
#ifdef RS485_EN4_Pin
					HAL_GPIO_WritePin(RS485_EN4_GPIO_Port,RS485_EN4_Pin,GPIO_PIN_RESET);
#endif
				 
					memset(&Uart6IOBuff,0,sizeof(queue_data_t));
			 }
			 osDelay(1);
	}
#endif
	
}
#endif
