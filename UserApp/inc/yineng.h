#ifndef __YINENG_H
#define __YINENG_H
#if 1
#include "stdint.h"

//解析接收数据结构体
typedef struct tagRS232_Frame_t
{
	uint8_t addr;
	uint16_t head;		
	uint16_t len;		
	uint16_t info;		
	uint16_t serial_number;
	uint16_t CMD;
	uint8_t data[256];
	
	uint16_t crc;			
}RS232_Frame_t;

//临时储存数据结构体
typedef struct{
	uint32_t req_vol;//需求电压
	uint32_t req_cur;//需求电流
	uint32_t charge_status;//PLC通讯状态
	uint32_t inside_status;//内部状态
	uint32_t volt_charge_plc;//桩端充电电压
	uint32_t curr_charge_plc;//桩端充电电流
	uint32_t charge_time;//充电时间
	uint32_t soc_now;//当前SOC
	uint32_t slac;//信号衰减信息
	uint32_t cro;//桩充电准备就绪状态
	uint32_t chargetime_remain;//充电剩余时间
	uint32_t volt_max_ev;//车端最大允许充电电压
	uint32_t curr_max_ev;//车端最大允许充电电流
	uint32_t power_max_ev;//车端最大功率限制
	uint32_t bat_volt_be_charge;//充电前电池电压
	uint32_t soc_start;//充电前起始SOC
	uint32_t bro;//车辆准备就绪
	uint32_t charge_stop;//充电剩余时间
	uint32_t charge_allow;//充电允许
	uint32_t err_code_ev;//充满SOC
	uint32_t remain_chargetime;//充电剩余时间
	uint32_t stop_start_charge;
	uint32_t R2_Contol;

}g_232data;

typedef enum tag232_FunCode_e{
	tag232_CMD1 = 1,	
	tag232_CMD5 = 5,
	tag232_CMD101 = 101,
	tag232_CMD2 = 2,
	tag232_CMD6 = 6,
	tag232_CMD102 = 102,
	tag232_CMD104 = 104,

}rs232_funcode;

typedef enum RecYiNeng{
		YNreq_vol=1,
		YNreq_cur=2,
		YNvolt_max_ev=3,
		YNcurr_max_ev=4,
		YNpower_max_ev=5,
		YNbat_volt_be_charge=6,
		YNsoc_start=7,
		YNbat_capa_ev=8,
		YNbat_capa_req=9,
		YNchargetime_remain=10,
		YNbro=11,
		YNcharge_allow=12,
		YNcharge_stop=13,
		YNerr_code_ev=14,
		YNcharge_status=15,
		YNcp_plc=16,
		YNfreq_plc=17,
		YNvolt_charge_plc=18,
		YNcurr_charge_plc=19,
		YNcharge_time=20,
		YNsoc_now=21,
		YNslac=22,
		YNcro=23,
		YNremain_chargetime=24,
		YNstatus_inside=25,
		YNstop_start_charge=26,
		
		rs232_MAX=200,
}g_232;

//extern volatile uint8_t execute_flag;
extern uint16_t startgetflag1;
extern volatile uint8_t yinengflag;
extern volatile uint8_t sendAmmeterFlag;
extern volatile uint8_t sendAmmeterFlag1;
extern volatile uint8_t sendAmmeterFlag2;

extern void FirstSaend(void);
extern void ChangeData(uint32_t addr,uint32_t data);
extern void ChangeChargeStatus(uint32_t data);
extern void ReadAll(uint32_t addr);
extern void ProRS232_Frame(uint8_t *pReqFrameBuf, uint16_t reqFrameBufSize);
extern void meter_server_repones();

extern void FirstSend(void);

extern void YINENG(void const * argument);

void StartYiNengTask();

#endif
#endif 