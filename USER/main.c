#include "stm32f10x.h"
#include "delay.h"
#include "oled.h"
#include "user_iic.h"
#include "sht30.h"
#include "stdio.h"
#include "def.h"
#include "usart.h"

static uint8_t i;

float Temperatures[126] = {0};
int NowPointToTemperatures = -1;

uint8_t TemperatureInString[5] = "00.0";

extern uint8_t IsTemperatureCalculated;
extern float temperatureC;

extern uint8_t Serial_RxData;
enum NOW_TICK_IRQ NowTickIRQ;

float SetTemperature = 35.0;

int main(void)
{
	//初始化OLED所用到的IO口
	SPI_GPIO_Init();
	//初始化OLED屏
	OLED_Init();
	//清屏操作
	OLED_Fill(0x00);

	IIC_GPIO_Init();
	SHT3X_Init();

	// 初始化串口
	Serial_Init();

	// 定义默认功能模式
	NowTickIRQ = NowTickIRQ_Null;

	while(1){
		if((NowTickIRQ & NowTickIRQ_USART1) && 1)
		{
			switch(Serial_RxData)
			{
				case 1:
					SetTemperature += 0.5;
				break;
				case 2:
					SetTemperature -= 0.5;
				break;
			}
		}
		NowTickIRQ = NowTickIRQ_Null;

		OLED_FillBackgroundInBuffer(OLED_Background_Buffer);
		SHT3X_TEST();

		// 将测量的温度放在数组中
		if(IsTemperatureCalculated)
		{
			if(NowPointToTemperatures < 125)
			{
				NowPointToTemperatures++;
				Temperatures[NowPointToTemperatures] = temperatureC;
			}
			else
			{
				for(i=0; i<125; i++)
				{
					Temperatures[i] = Temperatures[i + 1];
				}
				Temperatures[125] = temperatureC;
			}
		}

		// 将数组的内容绘制（避开左边两个像素的坐标轴）
		for(i=2; i<NowPointToTemperatures + 2; i++)
		{
			OLED_ChangeOnePixelInBuffer(i, 63 - (Temperatures[i - 2] - 20) * (float)(48 / (float)(20)), 1);
		}

		OLED_ApplyBuffer();

		// 显示温度数值
		sprintf(TemperatureInString, "%.1f", temperatureC);
		LCD_P8x16Str(32, 0, TemperatureInString);
		sprintf(TemperatureInString, "%.1f", SetTemperature);
		LCD_P8x16Str(96, 0, TemperatureInString);

		sw_delay_ms(5);
	}
}
