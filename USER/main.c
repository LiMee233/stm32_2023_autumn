#include "stm32f10x.h"
#include "oled.h"
#include "delay.h"
#include "timer.h"
#include "usart.h"
#include "def.h"
#include "stdio.h"

uint8_t ScrollMarginLeft = 0;
uint8_t ManualMoveModeMarginLeft = 0;
int TIM2TickMilliseconds = 100;
uint8_t WritingCustomIDIndex = 6;
char num2chartable[11] = "0123456789";
char currentID[7] = "000000";

uint8_t i = 0;
uint8_t j = 0;

enum
{
	NowScreen_HEBUT = 0,
	NowScreen_Own
} NowScreen;

enum MoveDirection
{
	MoveDirection_Left = 0,
	MoveDirection_Right,
	MoveDirection_None
};

enum NOW_TICK_IRQ NowTickIRQ;

extern uint8_t Serial_RxData;

void Warn(void);
void ShowTwoScreenOnce(void);
void ShowScrollScreenOnce(void);
void ShowScreenOnce(enum MoveDirection);

int main(void /* 给予函数 void 类型参数后，此函数被调用时不能传入任何参数，应该是为了代码安全考虑 */)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// 定义默认功能模式
	NowTickIRQ = NowTickIRQ_Null;
	NowScreen = NowScreen_HEBUT;

	// 初始化 OLED
	SPI_GPIO_Init();
	OLED_Init();
	OLED_Fill(0x00);

	// 初始化串口
	Serial_Init();

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

	while(1)
	{
		if(NowTickIRQ & NowTickIRQ_TIM2)
			ShowScrollScreenOnce(); // 滚动一下屏幕
		else if(NowTickIRQ & NowTickIRQ_TIM3)
			ShowTwoScreenOnce(); // 双屏显示，切换下一屏

		if(NowTickIRQ & NowTickIRQ_TIM4)
		{
			// 把灯和蜂鸣器关了，之后再把自己关了
			GPIO_SetBits(GPIOG, GPIO_Pin_6);
			GPIO_ResetBits(GPIOG, GPIO_Pin_7);
			DisableTIM4();
		}
		if(NowTickIRQ & NowTickIRQ_USART1)
		{
			if(WritingCustomIDIndex < 6)
			{
				currentID[WritingCustomIDIndex] = num2chartable[Serial_RxData];
				WritingCustomIDIndex ++;
			}
			else
			{
				switch(Serial_RxData)
				{
					case 1:
						ShowTwoScreenOnce();
						EnableTIM3();
						DisableTIM2();
						Warn();
					break;
					case 2:
						ShowScrollScreenOnce();
						EnableTIM2();
						DisableTIM3();
						Warn();
					break;
					case 3:
						TIM2TickMilliseconds = TIM2TickMilliseconds > 10 ? TIM2TickMilliseconds - 10 : 10;
						TIM2ChangeTime(TIM2TickMilliseconds);
					break;
					case 4:
						TIM2TickMilliseconds = TIM2TickMilliseconds < 500 ? TIM2TickMilliseconds + 10 : 500;
						TIM2ChangeTime(TIM2TickMilliseconds);
					break;
					case 5:
						WritingCustomIDIndex = 0;
					case 6:
						DisableTIM2();
						DisableTIM3();
						OLED_Fill(0x00);
					break;
					case 7:
						DisableTIM2();
						DisableTIM3();
						ShowScreenOnce(MoveDirection_Left);
					break;
					case 8:
						DisableTIM2();
						DisableTIM3();
						ShowScreenOnce(MoveDirection_Right);
					break;
				}
			}
		}

		// 执行过任何一个中断应执行的代码后，就将「该执行的中断代码」清空，避免重复执行
		NowTickIRQ = NowTickIRQ_Null;

		// 添加一个延时，给处理器一点响应时间，否则会特别卡
		delay_ms(10);
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
		LCD_P8x16Str(40, 4, currentID);

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
	LCD_P8x16Str(ScrollMarginLeft, 4, currentID);
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
	LCD_P8x16Str(currentManualMoveModeMarginLeft, 4, currentID);
	ManualMoveModeMarginLeft = currentManualMoveModeMarginLeft;
}
