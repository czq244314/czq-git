/********************************************************


*********************************************************/
#ifndef _MODBUS_H_
#define _MODBUS_H_

/********************************************************
					include
*********************************************************/
//#include "common_def.h"
#include "stdint.h"
#include "typedef.h"
#include "main.h"
#include "udp.h"
/********************************************************
					macro
*********************************************************/

/********************************************************
					typedef
*********************************************************/
typedef enum tagModbusRTU_FunCode_e
{

	FC_ReadCoilStatus = 1,		
	FC_ReadInputStatus = 2,		
	FC_ReadHoldingReg = 3,
	FC_ReadInputReg = 4,
	FC_ForceSingleCoil = 5,		
	FC_PresetSingleReg = 6,		
	FC_ReadExceptionStatus = 7,
	FC_Diagnostics = 8,
	FC_Program484 = 9,
	FC_Poll484 = 10,
	FC_FetchCommEventCtr = 11,
	FC_FetchCommEventLog = 12,
	FC_ProgramController = 13,
	FC_PollController = 14,
	FC_ForceMultipleCoils = 15,
	FC_PresetMultipleRegisters = 16,
	FC_ReportSlaveID = 17,
	FC_Program884_M84 = 18,
	FC_ResetCommLink = 19,
	FC_ReadGeneralReference = 20,
	FC_WriteGeneralReference = 21,
	FC_MaskWrite4XRegister = 22,
	FC_ReadWrite4XRegisters = 23,

}ModbusRTU_FunCode_e;

#define MODBUSFRAME_SIZE 252
typedef struct tagModbusRTU_Frame_t
{
	uint8_t slaveAddr;		
	uint8_t funCode;		
	uint8_t data[MODBUSFRAME_SIZE];		
	uint8_t len;			
	uint16_t crc;			
}ModbusRTU_Frame_t;

typedef struct ETHModbusRTU_Frame_t
{
	uint8_t usartPort;
	uint8_t slaveAddr;		
	uint8_t funCode;		
	uint8_t data[MODBUSFRAME_SIZE];		
	uint8_t len;			
	
}ETHModbusRTU_Frame_t;

typedef struct coil{
	uint16_t coil1;
	uint16_t coil2;
	uint16_t coil3;
	uint16_t coil4;

}coil_t;

extern coil_t coil_tp;

extern uint8_t usart1_deviceType,usart2_deviceType,usart2_deviceType,usart3_deviceType,usart6_deviceType;
extern uint32_t NewBaudRate1,NewBaudRate2,NewBaudRate3,NewBaudRate4;
/********************************************************
					variables
*********************************************************/
//extern ModbusRTU_Data ModbusRTU_Data_t;
/********************************************************
					functions
*********************************************************/
extern void Usart1Send_Read(void);
extern void Usart2Send_Read(void);
extern void Usart3Send_Read(void);
extern void Usart6Send_Read(void);
extern uint8_t (*pFnReadInputRegs)(Channel_t channel, uint8_t* pBuf);
extern  uint8_t (*pFnReadOutputRegs)(Channel_t regAddr, uint16_t size, ModbusRTU_Frame_t *pRepFrame, uint8_t addr);
//extern uint8_t (*pFnReadOutputRegs)(Channel_t channel, uint16_t size,uint8_t* pBuf,ModbusRTU_Frame_t *pRepFrame,uint8_t addr);
//extern uint8_t (*pFnWriteOutputReg)(Channel_t channel, uint16_t value, uint8_t addr);
extern uint8_t (*pFnWriteOutputReg)(Channel_t channel, uint16_t value,ModbusRTU_Frame_t *pRepFrame, uint8_t addr);
extern uint8_t (*pFnWriteOutputRegs)(Channel_t channel, uint16_t size, uint16_t* pValue, ModbusRTU_Frame_t *pRepFrame,uint8_t addr);

extern void (*pFnModbusSend)(uint8_t *dataBuff,uint16_t BufferSize,uint8_t TranIndex);
//请求报文处理
//void McgsModbusRTU_Frame(uint8_t *pReqFrameBuf, uint16_t reqFrameBufSize,struct udp_pcb *ethg_pcb,const ip_addr_t *addr, u16_t port);
void McgsModbusRTU_Frame(uint8_t *pReqFrameBuf, uint16_t reqFrameBufSize,uint8_t Chanel);
void McgsUSART1ModbusRTU_Frame(uint8_t *pReqFrameBuf, uint16_t reqFrameBufSize);
void McgsUSART2ModbusRTU_Frame(uint8_t *pReqFrameBuf, uint16_t reqFrameBufSize);
void McgsUSART3ModbusRTU_Frame(uint8_t *pReqFrameBuf, uint16_t reqFrameBufSize);
void McgsUSART6ModbusRTU_Frame(uint8_t *pReqFrameBuf, uint16_t reqFrameBufSize);
int HostReadRegs2Buf(uint8_t slaveAddr, uint16_t regAddr, uint16_t reqSize, uint8_t *buf);
uint16_t modbusRTU_Frame2Buf(ModbusRTU_Frame_t *pFrame, uint8_t *pBuf);
int HostSetReg2Buf(uint8_t slaveAddr, uint16_t regAddr, uint16_t value, uint8_t *buf);
//回应报文处理
uint8_t HostProModbusRTU_RepFrame(uint8_t *pRepFrameBuf, uint8_t repFrameBufSize,
                                  ModbusRTU_Frame_t *pReqFrame);

int HostSetMultipleRegs2Buf(uint8_t    devaddress, uint16_t staraddress_h, uint16_t staraddress_l, uint16_t value_h,uint8_t *buf);
extern int HostSetCoil2Buf(uint8_t slaveAddr, uint16_t regAddr, uint16_t value, uint8_t *buf);

extern uint16_t crc16(uint8_t *buffer, uint16_t buffer_length);
extern  bool buf2ModbusRTU_Frame(uint8_t *pReqFrameBuf, uint16_t reqFrameBufSize, ModbusRTU_Frame_t *pReqFrame);
#endif

