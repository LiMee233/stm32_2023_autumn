#include "stm32f10x.h"
void delay_ms(__IO uint32_t ms);

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);

	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1) == 1)
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);
			GPIO_SetBits(GPIOG, GPIO_Pin_7);
			delay_ms(25);

			GPIO_SetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);
			GPIO_ResetBits(GPIOG, GPIO_Pin_7);
			delay_ms(25);
		}
	}
}

void delay_ms(__IO uint32_t ms)
{
    uint32_t i;
    SysTick_Config(SystemCoreClock / 1000);
    for(i = 0; i < ms; i++)
    {
        while( !((SysTick->CTRL) & SysTick_CTRL_COUNTFLAG_Msk) );
    }
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
