#include "stm32f10x.h"
#include "exti_init.h"

// 初始化外部中断
void InitEXTI(void)
{
    /*-------------------- 初始化外部中断 2 --------------------*/

	GPIO_InitTypeDef currentGPIOInitStrut;
	EXTI_InitTypeDef currentEXTIInitStruct;
	NVIC_InitTypeDef currentNVICInitStruct;

	// 配置外部中断对应 IO 的参数
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	currentGPIOInitStrut.GPIO_Pin = GPIO_Pin_2;
	currentGPIOInitStrut.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOE, &currentGPIOInitStrut);

	// 配置外部中断的参数
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	/*
		STM32 支持 16 根中断线，但 STM32 的 IO 口不止 16 个，所以做出了这样的映射：
		每个引脚（0-15），对应了下面的 GPIO_PinSourceX
		每个 GPIO 口，对应了下面的 GPIO_PortSourceGPIOX

		在这里，我们需要为按键所在 GPIOE，Pin 2 做出中断响应，就应该按照相应的 GPIO 口和引脚对外部中断进行配置

		下面这行代码的意思「应该」是，为外部中断线 2，定义其 GPIO 口为 E，这样第 2 根中断线就会相应来自 GPIOE 的信息了。
	*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);
	/*
		接下来为创建的外部中断初始化结构体实例设置相应的值
	*/
	currentEXTIInitStruct.EXTI_Line = EXTI_Line2; //设置第 2 根中断线
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
	currentNVICInitStruct.NVIC_IRQChannel = EXTI2_IRQn;
	currentNVICInitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	currentNVICInitStruct.NVIC_IRQChannelSubPriority = 1;
	currentNVICInitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&currentNVICInitStruct);

    /*-------------------- 初始化外部中断 3 --------------------*/
    /*
        为了方便日后理解上述中断初始化的含义，此处不再将两个中断的初始化杂糅到一起
        仅仅复用了前面定义过的三个结构体
    */

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	currentGPIOInitStrut.GPIO_Pin = GPIO_Pin_3; // 疑问：如果在这里设置 3，后面再执行 GPIO_Init，那是会覆盖旧值，还是合并呢？（答案：合并）
	currentGPIOInitStrut.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOE, &currentGPIOInitStrut);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);
	currentEXTIInitStruct.EXTI_Line = EXTI_Line3;
	currentEXTIInitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	currentEXTIInitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	currentEXTIInitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&currentEXTIInitStruct);

	currentNVICInitStruct.NVIC_IRQChannel = EXTI3_IRQn;
	currentNVICInitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	currentNVICInitStruct.NVIC_IRQChannelSubPriority = 1;
	currentNVICInitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&currentNVICInitStruct);
}
