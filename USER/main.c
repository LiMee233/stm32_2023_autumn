#include "stm32f10x.h"
#include "delay.h"
#include "oled.h"
#include "user_iic.h"
#include "sht30.h"

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

	while(1){
		OLED_FillBackgroundInBuffer(OLED_Background_Buffer);
		OLED_ApplyBuffer();
		SHT3X_TEST(); 
		sw_delay_ms(500);
		
	}
}
