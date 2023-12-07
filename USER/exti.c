#include "stm32f10x.h"
#include "exti.h"

// 初始化外部中断
void InitEXTI(void)
{
    /*-------------------- 初始化外部中断 2 --------------------*/

	GPIO_InitTypeDef currentGPIOInitStrut;
	EXTI_InitTypeDef currentEXTIInitStruct;
	NVIC_InitTypeDef currentNVICInitStruct;

	// 配置外部中断对应 IO 的参数
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	currentGPIOInitStrut.GPIO_Pin = GPIO_Pin_1;
	currentGPIOInitStrut.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &currentGPIOInitStrut);

	// 配置外部中断的参数
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	/*
		STM32 支持 16 根中断线，但 STM32 的 IO 口不止 16 个，所以做出了这样的映射：
		每个引脚（0-15），对应了下面的 GPIO_PinSourceX
		每个 GPIO 口，对应了下面的 GPIO_PortSourceGPIOX

		在这里，我们需要为按键所在 GPIOE，Pin 2 做出中断响应，就应该按照相应的 GPIO 口和引脚对外部中断进行配置

		下面这行代码的意思「应该」是，为外部中断线 2，定义其 GPIO 口为 E，这样第 2 根中断线就会相应来自 GPIOE 的信息了。
	*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
	/*
		接下来为创建的外部中断初始化结构体实例设置相应的值
	*/
	currentEXTIInitStruct.EXTI_Line = EXTI_Line1; //设置第 2 根中断线
	currentEXTIInitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	currentEXTIInitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	currentEXTIInitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&currentEXTIInitStruct);

	// 配置中断向量
	/*
		什么是中断向量，为什么叫做「中断向量」？
		因为 STM32 单片机内部包含一个中断向量表（数组），编译过程中会将中断服务函数的地址放在这个数组里面
		数组的下标和中断优先级相对应，为中断设置相应的「一一对应关系」，就形成了一种「向量表」

		而终端服务函数在所谓「驱动」中已经被固定的名称定义完成了，只不过是一种弱定义，所以只需要在本 main.c 里面重新实现对应的中断服务函数，就可以接替「那个空的」
		这就是为什么中断服务函数能够被执行的原理
	*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级（暂时不管了）
	/*
		这个中断向量，设置的是哪个中断线呢？（在这里，是中断线 2）
		在 stm32f10x.h 中定义了 EXTI0_IRQn，EXTI1_IRQn 等，后面 5-9，10-15 是 EXTI9_5_IRQn 和 EXTI15_10_IRQn
	*/
	currentNVICInitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	currentNVICInitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	currentNVICInitStruct.NVIC_IRQChannelSubPriority = 1;
	currentNVICInitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&currentNVICInitStruct);
}

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}
