#include "stm32f10x.h"
#include "oled.h"
#include "delay.h"
#include "exti_init.h"
#include "exti_handler.h"
#include "timer_init.h"

#define true 1
#define false 0

uint8_t ScrollMarginLeft = 0;
int TIM2TickMilliseconds = 100;
uint8_t i = 0;
uint8_t j = 0;

enum
{
	WorkingMode_Null = 0,
	WorkingMode_TwoScreen,
	WorkingMode_ScrollScreen,
} WorkingMode;

enum
{
	NowScreen_HEBUT = 0,
	NowScreen_Own
} NowScreen;

void Warn(void);
void ShowTwoScreenOnce(void);
void ShowScrollScreenOnce(void);

int main(void /* 给予函数 void 类型参数后，此函数被调用时不能传入任何参数，应该是为了代码安全考虑 */)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// 定义默认功能模式
	WorkingMode = WorkingMode_Null;
	NowScreen = NowScreen_HEBUT;

	// 初始化 OLED
	SPI_GPIO_Init();
	OLED_Init();
	OLED_Fill(0x00);

	// 初始化外部中断
	InitEXTI();

	// 初始化用户时钟
	InitTIM3();
	InitTIM2();
	InitTIM4();

	// 初始化 LED 灯和蜂鸣器
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	GPIO_SetBits(GPIOG, GPIO_Pin_6);
	GPIO_ResetBits(GPIOG, GPIO_Pin_7);

	while(true)
	{
		// Wating for EXTI.
	}
}

void Warn(void)
{
	GPIO_ResetBits(GPIOG, GPIO_Pin_6);
	GPIO_SetBits(GPIOG, GPIO_Pin_7);
	EnableTIM4();
}

void ShowTwoScreenOnce(void)
{
	if(NowScreen == NowScreen_HEBUT)
	{
		OLED_Fill(0x00);

		// 第一行：名字
		for(i=0; i<3; i++)
		{
			LCD_P16x16Ch(i*16 + 40, 2, i+8);
		}
		// 第二行：学号
		LCD_P8x16Str(40, 4, "200385");

		NowScreen = NowScreen_Own;
	}
	else if(NowScreen == NowScreen_Own)
	{
		OLED_Fill(0x00);

		// 第一行：河里工业大学生日快乐
		for(i=0; i<8; i++)
		{
			LCD_P16x16Ch(i*16, 2, i);
		}

		NowScreen = NowScreen_HEBUT;
	}
}

void ShowScrollScreenOnce(void)
{
	if(ScrollMarginLeft >= 128)
	{
		Warn();
		ScrollMarginLeft = 0;
	}
	else
	{
		ScrollMarginLeft += 16;
	}

	OLED_Fill(0x00);
	
	// 第一行：名字
	for(i=0; i<3; i++)
	{
		LCD_P16x16Ch(i*16 + ScrollMarginLeft + 3, 2, i+8);
	}
	// 第二行：学号
	LCD_P8x16Str(ScrollMarginLeft, 4, "200385");
}

// 中断服务程序
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) == SET)
	{
		ShowTwoScreenOnce();
		EnableTIM3();
		DisableTIM2();
		WorkingMode = WorkingMode_TwoScreen;
		Warn();
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) == SET)
	{
		ShowScrollScreenOnce();
		EnableTIM2();
		DisableTIM3();
		WorkingMode = WorkingMode_ScrollScreen;
		Warn();
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) == SET)
	{
		TIM2TickMilliseconds = TIM2TickMilliseconds > 10 ? TIM2TickMilliseconds - 10 : 10;
		TIM2ChangeTime(TIM2TickMilliseconds);
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) == SET)
	{
		TIM2TickMilliseconds = TIM2TickMilliseconds < 1000 ? TIM2TickMilliseconds + 10 : 1000;
		TIM2ChangeTime(TIM2TickMilliseconds);
	}
	EXTI_ClearITPendingBit(EXTI_Line5);
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
	{
		if(WorkingMode == WorkingMode_TwoScreen)
		{
			ShowTwoScreenOnce();
		}
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	}
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		if(WorkingMode == WorkingMode_ScrollScreen)
		{
			ShowScrollScreenOnce();
		}
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	}
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET)
	{
		GPIO_SetBits(GPIOG, GPIO_Pin_6);
		GPIO_ResetBits(GPIOG, GPIO_Pin_7);
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
		DisableTIM4();
	}
}
