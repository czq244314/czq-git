#ifndef _QUEUE_GROUP_H
#define _QUEUE_GROUP_H
#include "JCHAppConfig.h"
#include "stdint.h"
typedef struct{
		uint16_t len;										//数据长度
		uint8_t data[BUFFER_SIZE];		//数据
}queue_data_t;


typedef struct{
		uint16_t len;										//数据长度
		uint8_t Ext;
		uint8_t data[BUFFER_SIZE];		//数据
}queue_mcgs_t;


extern void QueueInit(void);

#endif
