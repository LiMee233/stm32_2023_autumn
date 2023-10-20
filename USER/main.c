#include "stm32f10x.h"
void delay_ms(__IO uint32_t ms);
void BrightAll(int bright);

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOE,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	while(1)
	{
		int i;
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1) == 1)
		{
			while(1)
			{
				BrightAll(1);
				delay_ms(500);
				BrightAll(0);
				delay_ms(500);
				if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 0 ||
					GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0 ||
					GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0 ||
					GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5) == 0)
					{
						break;
					}
			}
		}
		else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 0)
		{
			for(i=0; i <= 1; i++)
			{
				GPIO_ResetBits(GPIOG, GPIO_Pin_6);
				delay_ms(500);
				GPIO_SetBits(GPIOG, GPIO_Pin_6);
				delay_ms(500);
			}
		}
		else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0)
		{
			for(i=0; i <= 2; i++)
			{
				GPIO_ResetBits(GPIOG, GPIO_Pin_7);
				delay_ms(500);
				GPIO_SetBits(GPIOG, GPIO_Pin_7);
				delay_ms(500);
			}
		}
		else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0)
		{
			for(i=0; i <= 3; i++)
			{
				GPIO_ResetBits(GPIOG, GPIO_Pin_8);
				delay_ms(500);
				GPIO_SetBits(GPIOG, GPIO_Pin_8);
				delay_ms(500);
			}
		}
		else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5) == 0)
		{
			for(i=0; i <= 4; i++)
			{
				GPIO_ResetBits(GPIOG, GPIO_Pin_9);
				delay_ms(500);
				GPIO_SetBits(GPIOG, GPIO_Pin_9);
				delay_ms(500);
			}
		}
	}
}

void BrightAll(int bright)
{
	if(bright == 1)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_8 | GPIO_Pin_9);
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		GPIO_ResetBits(GPIOG,GPIO_Pin_6);
	}
	else
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_8 | GPIO_Pin_9);
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
		GPIO_SetBits(GPIOG,GPIO_Pin_6);
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
