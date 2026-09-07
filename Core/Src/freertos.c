/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "NetIOAdapter.h"
#include "UdpIOServer.h"
#include "JCHAppData.h"
#include "ModbusRTU.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
queue_mcgs_t mcgs_recv_buff;
queue_mcgs_t USART1_QueueBuff;
queue_mcgs_t USART2_QueueBuff;
queue_mcgs_t USART3_QueueBuff;
queue_mcgs_t USART6_QueueBuff;
//extern volatile uint8_t Udp_ReceFlag;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
osThreadId myTask04Handle;
osThreadId myTask05Handle;
osThreadId myTask06Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void USART1Handle(void const * argument);
void USART2Handle(void const * argument);
void USART3Handle(void const * argument);
void USART6Handle(void const * argument);
void UsartHandle(void const * argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 256);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, USART1Handle, osPriorityNormal, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, USART2Handle, osPriorityNormal, 0, 128);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* definition and creation of myTask04 */
  osThreadDef(myTask04, USART3Handle, osPriorityNormal, 0, 128);
  myTask04Handle = osThreadCreate(osThread(myTask04), NULL);

  /* definition and creation of myTask05 */
  osThreadDef(myTask05, USART6Handle, osPriorityNormal, 0, 128);
  myTask05Handle = osThreadCreate(osThread(myTask05), NULL);

  /* definition and creation of myTask06 */
  osThreadDef(myTask06, UsartHandle, osPriorityNormal, 0, 128);
  myTask06Handle = osThreadCreate(osThread(myTask06), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartDefaultTask */
	mcgs_init();
	UdpServerInit();
  /* Infinite loop */
  for(;;)
  {
		if(mcgs_Recv(&mcgs_recv_buff))
		{
			McgsModbusRTU_Frame( mcgs_recv_buff.data, mcgs_recv_buff.len,mcgs_recv_buff.Ext);				
		}
		UdpSendLoop();
    osDelay(10);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_USART1Handle */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_USART1Handle */
void USART1Handle(void const * argument)
{
  /* USER CODE BEGIN USART1Handle */
  /* Infinite loop */
  for(;;)
  {
		if(xQueueReceive( UART1_RECV_QUEUE,&USART1_QueueBuff,0)==pdPASS)
		{
//				McgsUSARTModbusRTU_Frame(USART1_QueueBuff.data,USART1_QueueBuff.len);
		}
    osDelay(10);
  }
  /* USER CODE END USART1Handle */
}

/* USER CODE BEGIN Header_USART2Handle */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_USART2Handle */
void USART2Handle(void const * argument)
{
  /* USER CODE BEGIN USART2Handle */
  /* Infinite loop */
  for(;;)
  {
		if(xQueueReceive( UART2_RECV_QUEUE,&USART2_QueueBuff,0)==pdPASS)
		{
//				McgsUSARTModbusRTU_Frame(USART1_QueueBuff.data,USART1_QueueBuff.len);
		}
    osDelay(10);
  }
  /* USER CODE END USART2Handle */
}

/* USER CODE BEGIN Header_USART3Handle */
/**
* @brief Function implementing the myTask04 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_USART3Handle */
void USART3Handle(void const * argument)
{
  /* USER CODE BEGIN USART3Handle */
  /* Infinite loop */
  for(;;)
  {
		if(xQueueReceive( UART3_RECV_QUEUE,&USART3_QueueBuff,0)==pdPASS)
		{
//				McgsUSARTModbusRTU_Frame(USART1_QueueBuff.data,USART1_QueueBuff.len);
		}
    osDelay(10);
  }
  /* USER CODE END USART3Handle */
}

/* USER CODE BEGIN Header_USART6Handle */
/**
* @brief Function implementing the myTask05 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_USART6Handle */
void USART6Handle(void const * argument)
{
  /* USER CODE BEGIN USART6Handle */
  /* Infinite loop */
  for(;;)
  {
		if(xQueueReceive( UART6_RECV_QUEUE,&USART6_QueueBuff,0)==pdPASS)
		{
//				McgsUSARTModbusRTU_Frame(USART1_QueueBuff.data,USART1_QueueBuff.len);
		}
    osDelay(10);
  }
  /* USER CODE END USART6Handle */
}

/* USER CODE BEGIN Header_UsartHandle */
/**
* @brief Function implementing the myTask06 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_UsartHandle */
void UsartHandle(void const * argument)
{
  /* USER CODE BEGIN UsartHandle */
  /* Infinite loop */
  for(;;)
  {
		JCH_AdapterUartIOTask();
    osDelay(10);
  }
  /* USER CODE END UsartHandle */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
