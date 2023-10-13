#include "stm32f10x.h"
void delay_ms(__IO uint32_t ms);

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	while(1){
		GPIO_SetBits(GPIOB,GPIO_Pin_8);
		GPIO_SetBits(GPIOG,GPIO_Pin_7);
		delay_ms(500);
		GPIO_ResetBits(GPIOB,GPIO_Pin_8);
		GPIO_ResetBits(GPIOG,GPIO_Pin_7);
	  	delay_ms(500);
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
