#ifndef _TYPE_DEF_H
#define _TYPE_DEF_H

#include "stdint.h"
#include "stdbool.h"

#define  TRUE true
#define  FALSE false
#define  OS_TRUE TRUE
#define  OS_FALSE FALSE
#define NULL  0

 
typedef uint8_t 	uint8;
typedef uint16_t uint16;
typedef uint32_t 	uint32;

typedef  int8_t 	int8;
typedef  int16_t int16;
typedef  int32_t 	int32;

typedef bool BOOL;




#define DelayMs(t) osDelay(t)
		 
#if 1
		 
typedef enum tagMcgsChannel_e{
	reg1=1,reg2=2,reg3=3,reg4=4,reg5=5,reg6=6,reg7=7,	reg8=8,	reg9=9,	reg10=10,
	reg11=11,reg12=12,reg13=13,reg14=14,reg15=15,reg16=16,reg17=17,	reg18=18,	reg19=19,	reg20=20,		
	reg21=21,reg22=22,reg23=23,reg24=24,reg25=25,reg26=26,reg27=27,	reg28=28,	reg29=29,	reg30=30,	
	reg31=31,reg32=32,reg33=33,reg34=34,reg35=35,reg36=36,reg37=37,	reg38=38,	reg39=39,	reg40=40,	
	reg41=41,reg42=42,reg43=43,reg44=44,reg45=45,reg46=46,reg47=47,	reg48=48,	reg49=49,	reg50=50,	
		
	eth_MAX =20000,

}Channel_t;// McgsChannel_e;


#endif



#endif

