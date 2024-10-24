#include "stm32f10x.h"
#include "delay.h"
#include "keyboard.h"
#include "oled.h"
#include "timer.h"
#include "usart.h"

uint32_t secondsInDay = 0;

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// 初始化键盘
	KeyBoardGPIOInit();

	// 初始化 OLED 显示屏
	SPI_GPIO_Init();
	OLED_Init();
	OLED_Fill(0x00);

	// 初始化时钟
	InitTIM2();
	EnableTIM2();

	// 初始化串口
	USART1_Init();
	USART2_Init();

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
		}
	}
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		secondsInDay = secondsInDay > 86400 ? 0 : secondsInDay + 1;
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	}
}
