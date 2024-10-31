#include "stm32f10x.h"
#include "delay.h"
#include "keyboard.h"
#include "oled.h"
#include "timer.h"
#include "usart.h"
#include "adc.h"
#include "sensor.h"
#include "assist.h"

uint8_t MAX_PAGE_INDEX = 9;
uint8_t MAX_NOW_INPUTING_LENGHT = 8;

uint32_t secondsInDay = 0;
uint8_t pageIndex = 0;
char nowInputing[8] = '\0';

uint8_t isWarning = 0;

extern uint16_t adc_values[3];

extern double ECResult;
extern double TResult;
extern double HResult;
extern uint8_t CO2Result;
extern double APResult;
extern uint16_t LUXResult;

void CleanNowInputing();

int len = 0;

int main(void)
{
	// Initilize Keyboard
	KeyBoardGPIOInit();

	// Initilize A/D C
	AD_Init();

	// Initilize usart
	USART1_Init();
	USART2_Init();

	// Initilize & Enable Timer
	InitTIM2();
	EnableTIM2();

	// Initilize OLED
	OLED_Init();

	while(1)
	{
		switch(GetKeyBoardValue())
		{
			case 1:
				AppendChar(nowInputing, '1', MAX_NOW_INPUTING_LENGHT);
			break;

			case 2:
				AppendChar(nowInputing, '2', MAX_NOW_INPUTING_LENGHT);
			break;

			case 3:
				AppendChar(nowInputing, '3', MAX_NOW_INPUTING_LENGHT);
			break;

			case 4:
				CleanNowInputing();
				pageIndex = 0;
			break;

			case 5:
				AppendChar(nowInputing, '4', MAX_NOW_INPUTING_LENGHT);
			break;

			case 6:
				AppendChar(nowInputing, '5', MAX_NOW_INPUTING_LENGHT);
			break;

			case 7:
				AppendChar(nowInputing, '6', MAX_NOW_INPUTING_LENGHT);
			break;

			case 8:
				CleanNowInputing();
				pageIndex = pageIndex == 0 ? MAX_PAGE_INDEX : pageIndex - 1;
			break;

			case 9:
				AppendChar(nowInputing, '7', MAX_NOW_INPUTING_LENGHT);
			break;

			case 10:
				AppendChar(nowInputing, '8', MAX_NOW_INPUTING_LENGHT);
			break;

			case 11:
				AppendChar(nowInputing, '9', MAX_NOW_INPUTING_LENGHT);
			break;

			case 12:
				CleanNowInputing();
				pageIndex = pageIndex == MAX_PAGE_INDEX ? 0 : pageIndex + 1;
			break;

			case 13:
				AppendChar(nowInputing, '.', MAX_NOW_INPUTING_LENGHT);
			break;

			case 14:
				AppendChar(nowInputing, '0', MAX_NOW_INPUTING_LENGHT);
			break;

			case 15:
				RemoveLastChar(nowInputing);
			break;

			case 16:
				switch(pageIndex)
				{
					// CO2
					case 1:

					break;
				}
			break;

			default:
			break;
		}

		// Refresh OLED content
		switch(pageIndex)
		{
			// Main Page
			case 0:
				LCD_P8x16Str(0, 0, "MainWindow");

				LCD_P8x16Str(0, 1, "Status:");
				if(isWarning)
					LCD_P8x16Str(7, 1, "Warning");
				else
					LCD_P8x16Str(7, 1, "Normal");
			break;

			// CO2
			case 1:
				LCD_P8x16Str(0, 0, "CO2:");
				LCD_P8x16Str(0, 1, "Cur:");
				LCD_P8x16Str(4, 1, (char *)Uint8_t2String(CO2Result));
				LCD_P8x16Str(0, 2, "Set:");
				LCD_P8x16Str(4, 2, (char *)nowInputing);
			break;

			// Humidity
			case 2:
				LCD_P8x16Str(0, 0, "Humidity:");
				LCD_P8x16Str(0, 1, "Cur:");
				LCD_P8x16Str(4, 1, (char *)Double2String(HResult, 2));
				LCD_P8x16Str(0, 2, "Set:");
				LCD_P8x16Str(4, 2, (char *)nowInputing);
			break;

			// LiquidLevel
			case 3:
				LCD_P8x16Str(0, 0, "LLevel:");
				LCD_P8x16Str(0, 1, "Cur:");
				LCD_P8x16Str(4, 1, (char *)Double2String(getLiquidLevel(), 2));
				LCD_P8x16Str(0, 2, "Set:");
				LCD_P8x16Str(4, 2, (char *)nowInputing);
			break;

			// pH
			case 4:
				LCD_P8x16Str(0, 0, "pH:");
				LCD_P8x16Str(0, 1, "Cur:");
				LCD_P8x16Str(4, 1, (char *)Double2String(getpH(), 2));
				LCD_P8x16Str(0, 2, "Set:");
				LCD_P8x16Str(4, 2, (char *)nowInputing);
			break;

			// EC
			case 5:
				LCD_P8x16Str(0, 0, "EC:");
				LCD_P8x16Str(0, 1, "Cur:");
				LCD_P8x16Str(4, 1, (char *)Double2String(ECResult, 2));
				LCD_P8x16Str(0, 2, "Set:");
				LCD_P8x16Str(4, 2, (char *)nowInputing);
			break;

			// Temperature
			case 6:
				LCD_P8x16Str(0, 0, "Temperature:");
				LCD_P8x16Str(0, 1, "Cur:");
				LCD_P8x16Str(4, 1, (char *)Double2String(TResult, 2));
				LCD_P8x16Str(0, 2, "Set:");
				LCD_P8x16Str(4, 2, (char *)nowInputing);
			break;

			// LUX
			case 7:
				LCD_P8x16Str(0, 0, "LUX:");
				LCD_P8x16Str(0, 1, "Cur:");
				LCD_P8x16Str(4, 1, (char *)Uint8_t2String(CO2Result));
				LCD_P8x16Str(0, 2, "Set:");
				LCD_P8x16Str(4, 2, (char *)nowInputing);
			break;

			// Time
			case 8:
				LCD_P8x16Str(0, 0, "Set Time:");
				LCD_P8x16Str(2, 1, ":");
				LCD_P8x16Str(5, 1, ":");

				while (nowInputing[len] != '\0')
				{
					len++;
				}

				if(len >= 1)
					LCD_P8x16Str(0, 1, (char *)nowInputing[0]);
				else
					LCD_P8x16Str(0, 1, "0");

				if(len >= 2)
					LCD_P8x16Str(1, 1, (char *)nowInputing[1]);
				else
					LCD_P8x16Str(1, 1, "0");

				if(len >= 3)
					LCD_P8x16Str(3, 1, (char *)nowInputing[2]);
				else
					LCD_P8x16Str(3, 1, "0");

				if(len >= 4)
					LCD_P8x16Str(4, 1, (char *)nowInputing[3]);
				else
					LCD_P8x16Str(4, 1, "0");

				if(len >= 5)
					LCD_P8x16Str(6, 1, (char *)nowInputing[4]);
				else
					LCD_P8x16Str(6, 1, "0");

				if(len >= 6)
					LCD_P8x16Str(7, 1, (char *)nowInputing[5]);
				else
					LCD_P8x16Str(7, 1, "0");
			break;

			default:
			break;
		}

		if(pageIndex != 8)
		{
			// Show time in bottom
			LCD_P8x16Str(0, 3, "Time:");
			LCD_P8x16Str(5, 3, (char *)Uint8_t2String(secondsInDay / 60));
			LCD_P8x16Str(7, 3, ":");
			LCD_P8x16Str(8, 3, (char *)Uint8_t2String(secondsInDay % 60));
		}
	}
}

void CleanNowInputing()
{
	nowInputing[0] = '\0';
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		// Update clock
		secondsInDay = secondsInDay > 86400 ? 0 : secondsInDay + 1;

		// Tick sensor
		RefreshSN3002ECN01();
		RefreshSN300BYHM();

		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	}
}
