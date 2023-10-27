#include "delay.h"
#include "stm32f10x.h"

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
