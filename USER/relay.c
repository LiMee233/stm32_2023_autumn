#include "stm32f10x.h"

uint8_t IsRelayEnabled = 0;

void Relay_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOF, GPIO_Pin_14); // 默认不启动
}

void Relay_Enable(void)
{
    if(IsRelayEnabled == 0)
    {
        GPIO_SetBits(GPIOF, GPIO_Pin_14);
        IsRelayEnabled = 1;
    }
}

void Relay_Disable(void)
{
    if(IsRelayEnabled == 1)
    {
        GPIO_ResetBits(GPIOF, GPIO_Pin_14);
        IsRelayEnabled = 0;
    }
}
