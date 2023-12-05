#include "stm32f10x.h"
#include "oled.h"
#include "delay.h"

uint8_t OLED_Buffer[8][128] = {0}; // 按照页，8 行，128 列

uint8_t oled_i, oled_j;

// 清空 OLED 缓冲区
void OLED_ClearBuffer(void)
{
	for(oled_i=0; oled_i<8; oled_i++)
	{
		for(oled_j=0; oled_j<128; oled_j++)
		{
			OLED_Buffer[oled_i][oled_j] = 0x00;
		}
	}
}

// 改变一个像素的状态
// x：0~127；y：0~63；state：0 不亮，1 亮
// 注意：实际上当 x=0 的时候，是不亮的，其他都亮
void OLED_ChangeOnePixelInBuffer(uint8_t x, uint8_t y, uint8_t state)
{
	if(state)
		OLED_Buffer[y / 8][x] |= (1 << (y % 8));
	else
		OLED_Buffer[y / 8][x] &= ~(1 << (y % 8));
}

// 将 OLED 缓冲区内的内容显示在屏幕上（需要较长时间，此段代码可能存在性能问题，但至少功能是有的）
void OLED_ApplyBuffer(void)
{
	for(oled_i=0; oled_i<8; oled_i++)
	{
		for(oled_j=0; oled_j<128; oled_j++)
		{
			OLED_Set_Pos(oled_j, oled_i);
			OLED_WrByte(OLED_Buffer[oled_i][oled_j], OLED_WR_DAT);
		}
	}
}

// 初始化 OLED 所用到的 IO 口
void SPI_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// 打开所用 GPIO 的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG, ENABLE); 

	// 配置的 IO 是 PB5, PB13, PB15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_13 | GPIO_Pin_15;                
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// 配置的 IO 是 PG4, PG11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_11;                
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

// 向 OLED 写入 1 字节数据
// dat: 数据
// mode：0 写入命令，1 写入数据
void OLED_WrByte(uint8_t dat,uint8_t mode)
{
	uint8_t i=8, temp=0;

	if(mode == OLED_WR_CMD)
		OLED_DC_0;
	else
		OLED_DC_1;

	for(i=0; i<8; i++) // 发送一个八位数据
	{
		OLED_SCK_0;  

		temp = dat&0x80;
		if (temp == 0)
			OLED_MOSI_0;
		else
			OLED_MOSI_1;
		dat <<= 1;
		OLED_SCK_1;
	}
}

// 设置坐标
void OLED_Set_Pos(uint8_t x, uint8_t y) 
{ 
	OLED_WrByte((0xb0+y), OLED_WR_CMD);
	OLED_WrByte(((x&0xf0)>>4)|0x10, OLED_WR_CMD);
	OLED_WrByte((x&0x0f)|0x01, OLED_WR_CMD);
} 

// OLED 清屏函数
void OLED_Fill(uint8_t dat)
{
	uint8_t y,x;
	for(y=0; y<8; y++)
	{
		OLED_WrByte(0xb0 + y, OLED_WR_CMD); // 设置页地址（0~7）
		OLED_WrByte(0x02, OLED_WR_CMD); // 设置显示位置—列低地址
		OLED_WrByte(0x10, OLED_WR_CMD); // 设置显示位置—列高地址
		for(x=0; x<X_WIDTH; x++)
		{
			OLED_WrByte(dat, OLED_WR_DAT);
		}
	}
}

// 指定位置显示一个字符
// x：列 0~127
// y：页地址 0~7
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr)
{      	
	uint8_t c=0, i=0;

	c = chr-' '; // 得到偏移后的值			
	if(x > Max_Column - 1)
	{
		x = 0;
		y = y + 2;
	}

	OLED_Set_Pos(x, y);	
	for(i=0; i<8; i++)
	{
		OLED_WrByte(codeF8X16[c * 16 + i], OLED_WR_DAT);
	} 
	OLED_Set_Pos(x,y+1);
	for(i=0; i<8; i++)
	{
		OLED_WrByte(codeF8X16[c * 16 + i + 8], OLED_WR_DAT);
	}
}

// 显示 8*16 一组标准 ASCII 字符串
// x：列 0~127
// y：页地址 0~7
void LCD_P8x16Str(uint8_t x, uint8_t y, uint8_t ch[])
{
	uint8_t c=0, i=0, j=0;

	while (ch[j] != '\0')
	{    
		c = ch[j] - 32;
		if(x > 120)
		{
			x = 0;
			y++;
		}
    	OLED_Set_Pos(x, y);
    	for(i=0; i<8; i++)
		{
			OLED_WrByte(codeF8X16[c * 16 + i], OLED_WR_DAT);
		}
		OLED_Set_Pos(x, y + 1);
    	for(i=0; i<8; i++)
		{
			OLED_WrByte(codeF8X16[c * 16 + i + 8], OLED_WR_DAT);
		}         
    	x += 8;
    	j++;
  }
}

// 显示汉字
// x：列 0~127
// y：页地址 0~7
void LCD_P16x16Ch(uint8_t x, uint8_t y, uint8_t N)
{
	uint8_t wm=0;
	unsigned int adder=32 * N;
	OLED_Set_Pos(x, y);
	for(wm=0; wm<16; wm++)
	{
		OLED_WrByte(codeF16x16[adder], OLED_WR_DAT);    
		adder += 1;
	}      
	OLED_Set_Pos(x, y + 1); 
	for(wm=0; wm<16; wm++)
	{
		OLED_WrByte(codeF16x16[adder], OLED_WR_DAT);
		adder += 1;
	}
}

 // 显示 BMP 图片 128×64
 // 起始点坐标 (x,y), x 的范围 0～127，y 为页的范围 0～7
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t BMP[])
{ 	
	uint16_t j=0;
	uint8_t x, y;

	if((y1 % 8) == 0)
		y = y1 / 8;      
	else
		y = y1 / 8 + 1;

	for(y=y0; y<y1; y++)
	{
		OLED_Set_Pos(x0, y);
		for(x=x0; x<x1; x++)
		{
			OLED_WrByte(BMP[j++], OLED_WR_DAT);
		}
	}
} 

// OLED 初始化
void OLED_Init(void)     
{    
	OLED_RES_0; // OLED 复位引脚拉低
	OLED_CS_1; // OLED 使能引脚置高电平
	delay_ms(10); // 该延时必不可少,是给初始化足够的时间
	OLED_RES_1; // OLED 复位引脚拉高
	OLED_CS_0; // OLED 使能引脚置低电平
	delay_ms(200);

	OLED_WrByte(0xae, OLED_WR_CMD); // Turn off oled panel
	OLED_WrByte(0x00, OLED_WR_CMD); // Set low column address
	OLED_WrByte(0x10, OLED_WR_CMD); // Set high column address
	OLED_WrByte(0x40, OLED_WR_CMD); // Set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrByte(0x81, OLED_WR_CMD); // Set contrast control register
	OLED_WrByte(0xcf, OLED_WR_CMD); // Set SEG Output Current Brightness

	OLED_WrByte(0xa1, OLED_WR_CMD); // Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WrByte(0xc8, OLED_WR_CMD); // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WrByte(0xa6, OLED_WR_CMD); // Set normal display
	OLED_WrByte(0xa8, OLED_WR_CMD); // Set multiplex ratio(1 to 64)
	OLED_WrByte(0x3f, OLED_WR_CMD); // 1/64 duty

	OLED_WrByte(0xd3, OLED_WR_CMD); // Set display offset    Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrByte(0x00, OLED_WR_CMD); // Not offset
	
	OLED_WrByte(0xd5, OLED_WR_CMD); // Set display clock divide ratio/oscillator frequency
	OLED_WrByte(0x80, OLED_WR_CMD); // Set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrByte(0xd9, OLED_WR_CMD); // Set pre-charge period

	OLED_WrByte(0xf1, OLED_WR_CMD); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	//OLED_WrByte(0x22, OLED_WR_CMD);
	OLED_WrByte(0xda, OLED_WR_CMD); // Set com pins hardware configuration
	OLED_WrByte(0x12, OLED_WR_CMD);
	OLED_WrByte(0xdb, OLED_WR_CMD); // Set vcomh

	OLED_WrByte(0x40, OLED_WR_CMD); // Set VCOM Deselect Level

	OLED_WrByte(0x20, OLED_WR_CMD); // Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrByte(0x02, OLED_WR_CMD);
	OLED_WrByte(0x8d, OLED_WR_CMD); // Set Charge Pump enable/disable
	OLED_WrByte(0x14, OLED_WR_CMD); // Set(0x10) disable
	OLED_WrByte(0xa4, OLED_WR_CMD); // Disable Entire Display On (0xa4/0xa5)
	OLED_WrByte(0xa6, OLED_WR_CMD); // Disable Inverse Display On (0xa6/a7) 
	OLED_WrByte(0xaf, OLED_WR_CMD); // Turn on oled panel
	OLED_Fill(0xff); // 初始清屏
	OLED_Set_Pos(0,0);     
}
