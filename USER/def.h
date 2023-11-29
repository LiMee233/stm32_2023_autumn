#ifndef DEF_H
#define DEF_H

enum NOW_TICK_IRQ
{
	NowTickIRQ_Null     = 0, // 0000,0000
	NowTickIRQ_TIM2     = 1, // 0000,0001
	NowTickIRQ_TIM3     = 2, // 0000,0010
	NowTickIRQ_TIM4     = 4, // 0000,0100
	NowTickIRQ_USART1   = 8, // 0000,1000
};

#endif
