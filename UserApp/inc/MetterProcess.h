#ifndef _METTER_PROCESS_H
#define _METTER_PROCESS_H 
#include "includes.h"



extern  uint8_t sendflag;
extern  uint8_t sendflag1;

extern void MatterModbusRTU_Frame(uint8_t *pReqFrameBuf, uint16_t reqFrameBufSize);
#endif