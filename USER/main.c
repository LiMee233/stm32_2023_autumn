#include "stm32f10x.h"
#include "delay.h"
#include "keyboard.h"
#include "oled.h"
#include "timer.h"
#include "usart.h"
#include "adc.h"
#include "sensor.h"

uint32_t secondsInDay = 0;

extern uint16_t adc_values[3];

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_0 |
									GPIO_Pin_5 |
									GPIO_Pin_6 |
									GPIO_Pin_7 |
									GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// 初始化键盘
	KeyBoardGPIOInit();

	AD_Init();

	while(1)
	{
		switch(GetKeyBoardValue())
		{
			case 1:
				GPIO_SetBits(GPIOA, GPIO_Pin_0);
			break;
			case 2:
				GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			break;
			case 3:
				GPIO_ResetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8);
			break;
		}

		if(adc_values[0] > 512)
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
		if(adc_values[0] > 1024)
			GPIO_SetBits(GPIOA, GPIO_Pin_6);
		if(adc_values[0] > 2048)
			GPIO_SetBits(GPIOA, GPIO_Pin_7);
		if(adc_values[0] > 3072)
			GPIO_SetBits(GPIOA, GPIO_Pin_8);
	}
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		// Update clock
		secondsInDay = secondsInDay > 86400 ? 0 : secondsInDay + 1;

		// Tick sensor
		RefreshSN3002ECN01();
		RefreshSN300BYHM();

		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	}
}
