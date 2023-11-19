#include "stm32f10x.h"
#include "timer_init.h"

// 当且仅当 TIM_Period = 9999，TimeClockFren = 72M 时
uint16_t GetPrescalerFromMillisecond(int millisecond)
{
	return (uint16_t)(7.2 * millisecond - 1);
}

void InitTIM3(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //打开外设TIM3时钟

	TIM_DeInit(TIM3); //将外设TIM3寄存器重设为缺省值

	// TIM3是通用定时器，需配置TIM_ClockDivision、TIM_CounterMode、TIM_Period和TIM_Prescaler，无需配置TIM_RepetitionCounter
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // 设置了时钟分割(Tck_tim) 
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; // 选择了计数器模式(TIM向上计数模式) 
	TIM_TimeBaseInitStruct.TIM_Period = 9999; // 设定计数器自动重装值,取值范围0x0000~0xFFFF
	TIM_TimeBaseInitStruct.TIM_Prescaler = GetPrescalerFromMillisecond(1000); // 设置用来作为TIM3时钟频率除数的预分频值为(7199+1),取值范围0x0000~0xFFFF
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // NVIC_Group:先占优先级2位，从优先级2位
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; // 配置为TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 先占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; // 从优先级为2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能中断通道
	NVIC_Init(&NVIC_InitStructure);
}

void EnableTIM3(void)
{
	TIM_ClearFlag(TIM3, TIM_FLAG_Update); // 清除TIM3的待处理标志位
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // 使能TIM3中断
	TIM_Cmd(TIM3, ENABLE); // 使能TIM3外设
}

void DisableTIM3(void)
{
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
	TIM_Cmd(TIM3, DISABLE);
}

void InitTIM2(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_DeInit(TIM2);

	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 9999;
	TIM_TimeBaseInitStruct.TIM_Prescaler = GetPrescalerFromMillisecond(100);
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void EnableTIM2(void)
{
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

void DisableTIM2(void)
{
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
	TIM_Cmd(TIM2, DISABLE);
}

void InitTIM4(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_DeInit(TIM4);

	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 9999;
	TIM_TimeBaseInitStruct.TIM_Prescaler = GetPrescalerFromMillisecond(100);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void EnableTIM4(void)
{
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

void DisableTIM4(void)
{
	TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
	TIM_Cmd(TIM4, DISABLE);
}
