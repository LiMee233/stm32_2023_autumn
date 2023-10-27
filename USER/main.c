#include "stm32f10x.h"
#include "oled.h"
#include "delay.h"

int main(void)
{
	uint8_t	i;
	uint8_t j;

	// 初始化按键
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOE,&GPIO_InitStructure);

	//初始化OLED所用到的IO口
	SPI_GPIO_Init();
	//初始化OLED屏
	OLED_Init();
	//清屏操作
	OLED_Fill(0x00);

	while(1)
	{
		// 直接进入循环切换显示两屏内容
		while(1)
		{
			// 显示两屏内容逻辑
			OLED_Fill(0x00);

			// 第一行：河里工业大学生日快乐
			for(i=0; i<8; i++)
			{
				LCD_P16x16Ch(i*16, 2, i);
			}

			delay_ms(1000);

			// 按下按键 1 需要跳出循环（之后直接进入下面的滚动显示）
			if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 0)
				break;

			OLED_Fill(0x00);

			// 第一行：名字
			for(i=0; i<3; i++)
			{
				LCD_P16x16Ch(i*16 + 40, 2, i+8);
			}
			// 第二行：学号
			LCD_P8x16Str(40, 4, "200385");

			delay_ms(1000);
		}

		while(1)
		{
			int isUserBreak = 0;
			// 滚动显示逻辑
			for(j=0; j<128; j++)
			{
				delay_ms(10);
				OLED_Fill(0x00);
				
				// 第一行：名字
				for(i=0; i<3; i++)
				{
					LCD_P16x16Ch(i*16 + j + 3, 2, i+8);
				}
				// 第二行：学号
				LCD_P8x16Str(j, 4, "200385");

				// 滚动显示只有在按下按键的时候才需要退出
				if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0)
				{
					isUserBreak = 1;
					break;
				}
			}

			if(isUserBreak == 1)
				break;
		}
	}
}
