#include "stm32f10x.h"
#include "delay.h"

int main(void)
{
	//初始化OLED所用到的IO口
	SPI_GPIO_Init();
	//初始化OLED屏
	OLED_Init();
	//清屏操作
	OLED_Fill(0x00);

	while(1){
		
	}
}
