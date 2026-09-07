#ifndef _BROADCAST_H
#define _BROADCAST_H
#include "stdint.h"

#define UDP_BROADCAST_EN 1


extern uint32_t g_BroadCast_Time;
extern uint32_t g_BroadCast_TimeOut;


extern void BroadCast_Init();

extern void BroadCast_Send();
#endif