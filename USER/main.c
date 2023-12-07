#include "stm32f10x.h"
#include "delay.h"
#include "oled.h"
#include "oledfont.h"
#include "adc.h"
#include "dac.h"
#include "usart.h"
#include "timer.h"
#include "stdio.h"
#include "def.h"
#include "fft.h"

static uint16_t i = 0; // 需要循环 256 次（含）以上，不要用 uint8_t，否则会卡死在循环里面

uint8_t oled_x = 0;
uint16_t ADC_value = 0;
uint16_t ADC_Value_Previous = 0;
uint8_t ADC_YPosition = 0;
uint8_t ADC_YPosition_Previous = 0;

enum NOW_TICK_IRQ NowTickIRQ;

extern uint8_t Serial_RxData;

// 波形的相关参数设置
uint8_t ADC_Y_Offset = 0; // 波形纵向位置的偏移
uint16_t ADC_Value_Trigger = 0; // 边沿触发的值

// 频率
uint16_t adcx[NPT]; // adc 数值缓存
extern int long fftin[NPT]; // FFT 输入
uint16_t Frequency = 0;
uint8_t FrequencyInString[7] = "000000";

int main(void)
{
	// 获取信号，初始化 AD
	AD_Init();

	// 发出信号，初始化 DA
	DA_Init();
	setVoltage(0.0f);
	InitTIM3(); // 初始化定时器
	EnableTIM3(); // 启用定时器

	// 接受 USB 信息，初始化串口
	Serial_Init();

	// 在 OLED 上显示内容，初始化 OLED 屏幕
	SPI_GPIO_Init();
	OLED_Init();

	// 测量频率，初始化频率测量定时器
	InitTIM2();
	EnableTIM2();

	// 定义默认功能模式
	NowTickIRQ = NowTickIRQ_Null;

	while(1){
		// 先处理串口输入和定时器的中断
		if(NowTickIRQ & NowTickIRQ_TIM2)
		{
			// 测量频率，采样 1024 次准备 fft
			for(i=0; i<NPT; i++)
			{
				adcx[i] = AD_GetValue();
				fftin[i] = 0;
				fftin[i] = adcx[i] << 16;
			}

			// 执行 fft
			GetPowerMag();
		}
		if(NowTickIRQ & NowTickIRQ_USART1)
		{
			// 检测到串口输入字符串
			switch(Serial_RxData)
			{
				case 0:
					ADC_Y_Offset -= 1; // 这里还缺少一些超限检查
				break;
				case 1:
					ADC_Y_Offset += 1;
				break;
				case 2:
					ADC_Value_Trigger -= 100;
				break;
				case 3:
					ADC_Value_Trigger += 100;
				break;
			}
		}

		// 执行过任何一个中断应执行的代码后，就将「该执行的中断代码」清空，避免重复执行
		NowTickIRQ = NowTickIRQ_Null;

		// 先向缓冲区中添加背景
		OLED_FillBackgroundInBuffer(OLED_Background_Buffer);

		// 实现边沿触发
		ADC_value = AD_GetValue();
		if(ADC_Value_Previous <= ADC_Value_Trigger && ADC_value >= ADC_Value_Trigger)
		{
			// 测量，并画点
			for(oled_x=2; oled_x<128; oled_x++)
			{
				// 上面检测边沿触发的时候，已经获取过一次 ADC_Value，所以第一次循环就不再测一次了
				if(oled_x > 2)
					ADC_value = AD_GetValue();

				ADC_YPosition = 63 - (int)((float)ADC_value / 4095 * 48) + ADC_Y_Offset; // 最下面一行是 63，蓝色显示部分高度为 48，此外还实现了波形纵向位置的偏移

				// 如果超限，那就不显示了
				if(ADC_YPosition < 16 || ADC_YPosition > 63)
				{
					ADC_YPosition_Previous = 0;
					continue;
				}

				OLED_ChangeOnePixelInBuffer(oled_x, ADC_YPosition, 1);

				// 尝试将上一个点和当前点连起来
				// 操作的是本列和上一列，所以第一列不操作（因为没有上一列）；
				// 如果上一列超限了，那就不连了
				if(oled_x >0 && ADC_YPosition_Previous != 0)
				{
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
				}

				// 每画一个点，就将本点设置为上一个点，因为马上就要画新的了。
				ADC_YPosition_Previous = ADC_YPosition;

				delay_ms(1);
			}

			// 只有完成一次测量后，才需要将储存的图像刷新在屏幕上
			OLED_ApplyBuffer();

			// 显示测量的频率
			sprintf(FrequencyInString, "%06d", Frequency);
			LCD_P8x16Str(39, 0, FrequencyInString);
		}

		// 每完成一次测量，或者跳过一次测量，就将本次 ADC 最终获取的值，设置为上一次 ADC 获取的值。
		ADC_Value_Previous = ADC_value;
	}
}
