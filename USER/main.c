#include "stm32f10x.h"
#include "oled.h"
#include "delay.h"
#include "exti_init.h"
#include "exti_handler.h"

#define true 1
#define false 0

void ShowTwoScreen(void);
void ShowScrollScreen(void);

int IsOtherEXTISetExpectEXTI_Line2(void);
int IsOtherEXTISetExpectEXTI_Line3(void);

uint8_t i = 0;
uint8_t j = 0;

uint8_t IsManualMoveModeEnabled = 0;
uint8_t ManualMoveModeMarginLeft = 0; // 最大是 128，此时完全移出。

enum MoveDirection
{
	MoveDirection_Left = 0,
	MoveDirection_Right,
	MoveDirection_None
};

int main(void /* 给予函数 void 类型参数后，此函数被调用时不能传入任何参数，应该是为了代码安全考虑 */)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // Or GPIO_Mode_IPU?
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	delay_ms(250);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	delay_ms(250);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);

	//初始化OLED所用到的IO口
	SPI_GPIO_Init();
	//初始化OLED屏
	OLED_Init();
	//清屏操作
	OLED_Fill(0x00);

	// 初始化外部中断
	InitEXTI();

	while(true)
	{
		// Do nothing.
	}
}

// 循环显示两屏内容
void ShowTwoScreen(void)
{
	while(true)
	{
		OLED_Fill(0x00);

		// 第一行：河里工业大学生日快乐
		for(i=0; i<8; i++)
		{
			LCD_P16x16Ch(i*16, 2, i);
		}

		if(IsOtherEXTISetExpectEXTI_Line2() == true)
			return;
		delay_ms(500);

		OLED_Fill(0x00);

		// 第一行：名字
		for(i=0; i<3; i++)
		{
			LCD_P16x16Ch(i*16 + 40, 2, i+8);
		}
		// 第二行：学号
		LCD_P8x16Str(40, 4, "200385");

		if(IsOtherEXTISetExpectEXTI_Line2() == true)
			return;
		delay_ms(500);
	}
}

// 滚动显示内容
void ShowScrollScreen(void)
{
	while(true)
	{
		for(j=0; j<128; j++)
		{
			if(IsOtherEXTISetExpectEXTI_Line3() == true)
				return;
			delay_ms(10);
			OLED_Fill(0x00);
			
			// 第一行：名字
			for(i=0; i<3; i++)
			{
				LCD_P16x16Ch(i*16 + j + 3, 2, i+8);
			}
			// 第二行：学号
			LCD_P8x16Str(j, 4, "200385");
		}
	}
}

// 单次显示内容（包含偏移）
void ShowScreenOnce(enum MoveDirection currentMoveDirection)
{	
	int currentManualMoveModeMarginLeft = ManualMoveModeMarginLeft;
	if(currentMoveDirection == MoveDirection_Left)
		currentManualMoveModeMarginLeft = 
			(currentManualMoveModeMarginLeft <= 16) ? 128 : currentManualMoveModeMarginLeft - 16;
	else
		currentManualMoveModeMarginLeft =
			(currentManualMoveModeMarginLeft >= 128 - 16) ? 0 : currentManualMoveModeMarginLeft + 16;

	OLED_Fill(0x00);

	// 第一行：名字
	for(i=0; i<3; i++)
	{
		LCD_P16x16Ch(i*16 + 3 + currentManualMoveModeMarginLeft, 2, i+8);
	}
	// 第二行：学号
	LCD_P8x16Str(currentManualMoveModeMarginLeft, 4, "200385");
	ManualMoveModeMarginLeft = currentManualMoveModeMarginLeft;
}

int IsOtherEXTISetExpectEXTI_Line2(void)
{
	return 	EXTI_GetITStatus(EXTI_Line3) == SET ||
			EXTI_GetITStatus(EXTI_Line0) == SET ||
			EXTI_GetITStatus(EXTI_Line4) == SET ||
			EXTI_GetITStatus(EXTI_Line5) == SET;
}

int IsOtherEXTISetExpectEXTI_Line3(void)
{
	return 	EXTI_GetITStatus(EXTI_Line2) == SET ||
			EXTI_GetITStatus(EXTI_Line0) == SET ||
			EXTI_GetITStatus(EXTI_Line4) == SET ||
			EXTI_GetITStatus(EXTI_Line5) == SET;
}

// 中断服务程序
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) == SET)
	{
		IsManualMoveModeEnabled = false;
		ShowTwoScreen();
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) == SET)
	{
		IsManualMoveModeEnabled = false;
		ShowScrollScreen();
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
		delay_ms(500);
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		delay_ms(500);
		GPIO_SetBits(GPIOB, GPIO_Pin_8);

		IsManualMoveModeEnabled = true;
		ShowScreenOnce(MoveDirection_None);
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) == SET)
	{
		ShowScreenOnce(MoveDirection_Left);
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) == SET)
	{
		ShowScreenOnce(MoveDirection_Right);
	}
	EXTI_ClearITPendingBit(EXTI_Line5);
}
