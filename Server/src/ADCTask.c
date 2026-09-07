#include "adctask.h"
#include "string.h"
#include "includes.h"
#include "mcgs.h"
#include "main.h"
#include "jad.h"
#include "tim.h"
#include "yineng.h"
#if 1
#include "metterprocess.h"
extern g_232data YN_data;
float pwm_f;			//pwm频率
float pwm_duty_f;			//pwm占空比

#define PWMCOUNT	10

float arrayFrequency[PWMCOUNT] = {0};
float arrayDuty[PWMCOUNT] = {0};
float arrayAdc[PWMCOUNT]={0};

unsigned char countPwmIndex = 0;
uint16_t upEdge=0;
uint16_t dowmEdge=0;
float DutyCycle = 0;
float Frequency = 0;


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	if(htim == &htim4 && htim->Channel ==HAL_TIM_ACTIVE_CHANNEL_3){
		upEdge=HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_3);
		dowmEdge=HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_4);
		TIM4->CNT =0;
		Frequency =(float)(60000000.0/(120.0*upEdge)) + 1.0;
		DutyCycle = ((float)dowmEdge /upEdge) *100.0 +1.0;
		if (++countPwmIndex >= PWMCOUNT)
		{
			countPwmIndex=0;
		}
		arrayDuty[countPwmIndex] = DutyCycle;
		arrayFrequency[countPwmIndex] = Frequency;
	}	
	
}

void PP_Handle (void)
{
	//g_CHANNEL.PP_Volt=(float)((values[0]/4096.0)*18.0);
	g_CHANNEL.PP_Volt=(float)((GetPPVoltValue()/4096.0)*18.0);
	if (g_CHANNEL.PP_Volt < 4.5)
	{
		g_CHANNEL.ChargeStatus = 1;
	}
	else
	{
		g_CHANNEL.ChargeStatus = 0;
	}

	//连接指示灯状态
	if ( 1 == g_CHANNEL.ChargeStatus )
	{
		HAL_GPIO_WritePin(LED_CONNECT_GPIO_Port,LED_CONNECT_Pin,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(LED_CONNECT_GPIO_Port,LED_CONNECT_Pin,GPIO_PIN_RESET);
	}	

}

void Pwm_Handle(void)
{
	float temp = 0.0;
	if (HAL_GPIO_ReadPin(VR_SW_GPIO_Port,VR_SW_Pin) == false)
	{
		//g_CHANNEL.CP_Volt_data =(float)(((GetCPVoltValue()/4096.0)*18.0)*(400+g_CHANNEL.R3_data)*7.8/6.8/400/1.2/2 + 0.2 + 0.5);
		g_CHANNEL.CP_Volt_data=GetCPVoltValue ()*(400+g_CHANNEL.R3_data)*7.6/6.8/4096/1.2/20;// + 0.34 + 0.2;

	}
	else
	{
		//g_CHANNEL.CP_Volt_data =(float)((GetCPVoltValue()/4096.0)*18.0);
		g_CHANNEL.CP_Volt_data = GetCPVoltValue () * 2730*7.6/6.8/4096/1.2/20;// + 0.34 + 0.2;//换了肖特基二极管,补偿改动
		if(g_CHANNEL.CP_Volt_data<2)
		{
			g_CHANNEL.CP_Volt_data=0;//  在没有输入情况下能显示零
		}
		
	}

	//运行指示灯
	if (5.2 <= g_CHANNEL.CP_Volt_data && g_CHANNEL.CP_Volt_data <= 6.8 && YN_data.charge_status!=0)
	{
		HAL_GPIO_WritePin(LED_RUN_GPIO_Port,LED_RUN_Pin,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(LED_RUN_GPIO_Port,LED_RUN_Pin,GPIO_PIN_RESET);
		
	}
	
	if(g_CHANNEL.ammeter_current>0.5)
	{
		HAL_GPIO_WritePin(LED_OUTPUT_GPIO_Port,LED_OUTPUT_Pin,GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(LED_OUTPUT_GPIO_Port,LED_OUTPUT_Pin,GPIO_PIN_RESET);
	}

	
	if ((pwm_duty_f > 0.1 && pwm_duty_f < 3) || pwm_duty_f > 97)
	{
		//不允许充电
	}

	else if (pwm_duty_f >= 3 && pwm_duty_f <= 7)
	{
		//需要数字通信
	}

	else if (pwm_duty_f > 7 && pwm_duty_f < 8)
	{
		//不允许充电
	}

	else if (pwm_duty_f >= 8 && pwm_duty_f <1)
	{
		//电流限值 6A
		g_CHANNEL.pwm_max_curr = 6;
	}

	else if (pwm_duty_f >= 10 && pwm_duty_f <= 85)
	{
		g_CHANNEL.pwm_max_curr = pwm_duty_f*0.6;
	}

	else if (pwm_duty_f >85 && pwm_duty_f <= 90)
	{
		temp = (pwm_duty_f - 64) *2.5;
//		g_CHANNEL.pwm_max_curr = temp <= 63 ? temp : 63.0;
		if(temp<=63.0)
		{
			g_CHANNEL.pwm_max_curr=63.0;
		}
		else
		{
			g_CHANNEL.pwm_max_curr=temp;
		}
	}

	else if (pwm_duty_f > 90 && pwm_duty_f <= 97)
	{
		//预留
	}

	else
	{
		//错误值
	}
}

float GetPwmDuty(void)
{
	float result=0;
	for (unsigned char i=0; i<PWMCOUNT; ++i)
	{
		result+=arrayDuty[i];
	}
	return result/PWMCOUNT;
}

float GetPwmFrequency(void)
{
	float result=0;
	for (unsigned char i=0; i<PWMCOUNT; ++i)
	{
		result+=arrayFrequency[i];
	}
	return result/PWMCOUNT;
}

void Get_PWM(void)
{
	pwm_f=GetPwmFrequency()-7.0;
	pwm_duty_f=GetPwmDuty()-1.1;
	PP_Handle();
	Pwm_Handle();
}

#endif