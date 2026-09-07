#ifndef _ADAPTERIOTASK_H
#define _ADAPTERIOTASK_H

#include "string.h"
#if _USE_CAN_DATA
extern void JCH_AdapterCanIOTask(CAN_HandleTypeDef *hcanx);
#endif	
//
extern void JCH_AdapterUartIOTask();
#endif

