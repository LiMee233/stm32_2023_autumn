#include "stm32f10x.h"
#include "delay.h"
#include "oled.h"
#include "adc.h"
#include "usart.h"

static uint8_t i = 0;

uint8_t oled_x = 0;
uint16_t ADC_value = 0;
uint8_t ADC_YPosition = 0;
uint8_t ADC_YPosition_Previous = 0;

int main(void)
{
	// 初始化 AD
	AD_Init();

	// 初始化串口
	Serial_Init();

	// 初始化 OLED 所用到的 IO 口
	SPI_GPIO_Init();
	// 初始化 OLED 屏
	OLED_Init();
	// 清屏操作
	OLED_Fill(0x00);

	while(1){
		OLED_ClearBuffer();
		for(oled_x=0; oled_x<128; oled_x++)
		{
			ADC_value = AD_GetValue();
			ADC_YPosition = 64 - (int)((float)ADC_value / 4095 * 64);

			OLED_ChangeOnePixelInBuffer(oled_x, ADC_YPosition, 1);

			// 尝试将上一个点和当前点连起来
			if(ADC_YPosition > ADC_YPosition_Previous)
			{
				for(i=0; i<(ADC_YPosition - ADC_YPosition_Previous) / 2; i++)
				{
					OLED_ChangeOnePixelInBuffer(oled_x - 1, ADC_YPosition_Previous + i + 1, 1);
				}
				for(i=0; i<(ADC_YPosition - ADC_YPosition_Previous) / 2 + 1; i++)
				{
					OLED_ChangeOnePixelInBuffer(oled_x, ADC_YPosition - i - 1, 1);
				}
			}
			else
			{
				for(i=0; i<(ADC_YPosition_Previous - ADC_YPosition) / 2; i++)
				{
					OLED_ChangeOnePixelInBuffer(oled_x - 1, ADC_YPosition_Previous - i - 1, 1);
				}
				for(i=0; i<(ADC_YPosition_Previous - ADC_YPosition) / 2 + 1; i++)
				{
					OLED_ChangeOnePixelInBuffer(oled_x, ADC_YPosition + i + 1, 1);
				}
			}
			ADC_YPosition_Previous = ADC_YPosition;
			delay_ms(5);
		}
		OLED_ApplyBuffer();
	}
}
