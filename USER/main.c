#include "stm32f10x.h"
#include "delay.h"
#include "keyboard.h"

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB,&GPIO_InitStructure);
	KeyBoardGPIOInit();

	while(1){
		switch(GetKeyBoardValue())
		{
			case 1:
				GPIO_SetBits(GPIOB,GPIO_Pin_8);
			break;
			case 2:
				GPIO_ResetBits(GPIOB,GPIO_Pin_8);
			break;
		}
	}
}
