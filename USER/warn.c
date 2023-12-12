#include "stm32f10x.h"

void InitWarn(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	GPIO_SetBits(GPIOG, GPIO_Pin_6);
	GPIO_ResetBits(GPIOG, GPIO_Pin_7);
}

void EnableWarn(void)
{
	GPIO_ResetBits(GPIOG, GPIO_Pin_6);
	GPIO_SetBits(GPIOG, GPIO_Pin_7);
}

void DisableWarn(void)
{
	GPIO_SetBits(GPIOG, GPIO_Pin_6);
	GPIO_ResetBits(GPIOG, GPIO_Pin_7);
}
