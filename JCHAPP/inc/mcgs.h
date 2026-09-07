#ifndef _MCGS_H
#define _MCGS_H
#include "stdint.h"
#include "includes.h"
#include "modbusrtu.h"
#include "typedef.h"

//typedef int myint;

//extern CHANNEL g_CHANNEL;

extern bool eth_McgsReadOutputReg(Channel_t channel, uint8_t* pBuf,uint8_t Count, uint8_t addr);
extern void Insulation_simulation_Z_handle(uint16_t value);
extern  uint8_t ETHeth_readOutputRegs(Channel_t regAddr, uint16_t size, ETHModbusRTU_Frame_t *pRepFrame, uint8_t addr);
extern  uint8_t eth_readOutputRegs(Channel_t regAddr, uint16_t size, ModbusRTU_Frame_t *pRepFrame, uint8_t addr);
extern bool eth_McgsWriteOutputRegs(Channel_t channel, uint16_t size, uint16_t* pValue, uint8_t addr);
extern bool eth_McgsWriteOutputReg(Channel_t channel, uint16_t value, uint8_t addr);
extern  uint8_t eth_writeOutputReg(Channel_t regAddr, uint16_t value, ModbusRTU_Frame_t *pRepFrame,uint8_t addr);
extern  uint8_t eth_writeOutputRegs(Channel_t regAddr, uint16_t size, uint16_t *pValue, ModbusRTU_Frame_t *pRepFrame,uint8_t addr);
extern uint8_t ETHeth_writeOutputRegs(Channel_t regAddr, uint16_t size,uint16_t *pValue, ETHModbusRTU_Frame_t *pRepFrame,uint8_t addr);

#endif


