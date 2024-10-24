#include "stm32f10x.h"
#include "oled.h"
#include "delay.h"

void SPI_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

// 向 OLED 写入 1 字节数据
// dat：数据
// mode：0 为命令；1 为数据
void OLED_WrByte(uint8_t dat, uint8_t mode)
{
    uint8_t i=8, temp=0;

    if(mode == OLED_WR_CMD)
        OLED_DC_0;
    else
        OLED_DC_1;

    for(i=0; i<8; i++) // 发送一个 8 位数据
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

// 清屏
void OLED_Fill(uint8_t dat)
{
    uint8_t y, x;
    for(y=0; y<8; y++)
    {
        OLED_WrByte(0xb0+y, OLED_WR_CMD); // 设置页地址（0~7）
        OLED_WrByte(0x02, OLED_WR_CMD); // 设置显示位置—列低地址
        OLED_WrByte(0x10, OLED_WR_CMD); // 设置显示位置—列高地址
        for(x=0; x<X_WIDTH; x++)
		{
			OLED_WrByte(dat,OLED_WR_DAT);
		}
    }
}

// 指定位置显示一个字符
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr)
{
	uint8_t c=0, i=0;

	c = chr-' '; // 得到偏移后的值
	if(x > Max_Column-1)
	{
		x = 0;
		y = y+2;
	}

	OLED_Set_Pos(x, y);
	for(i=0; i<8; i++)
	{
		OLED_WrByte(codeF8X16[c*16+i],OLED_WR_DAT);
	}

	OLED_Set_Pos(x, y+1);
	for(i=0; i<8; i++)
	{
		OLED_WrByte(codeF8X16[c*16+i+8],OLED_WR_DAT);
	}
}

// 显示 8*16 一组标准 ASCII 字符串
void LCD_P8x16Str(uint8_t x, uint8_t y,uint8_t ch[])
{
	uint8_t c=0, i=0, j=0;

	while (ch[j] != '\0')
	{
		c = ch[j] - 32;
		if(x>120)
		{
			x = 0;
			y++;
		}

		OLED_Set_Pos(x,y);
		for(i=0; i<8; i++)
		{
			OLED_WrByte(codeF8X16[c*16+i], OLED_WR_DAT);
		}

		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		{
			OLED_WrByte(codeF8X16[c*16+i+8], OLED_WR_DAT);
		}
		x += 8;
		j++;
	}
}

// OLED 初始化
void OLED_Init(void)
{
	OLED_RES_0;    		// OLED 复位引脚拉低
    OLED_CS_1;     		// OLED 使能引脚置高电平
	delay_ms(10);       // 该延时必不可少,是给初始化足够的时间
	OLED_RES_1;    		// OLED 复位引脚拉高
    OLED_CS_0;     		// OLED 使能引脚置低电平
    delay_ms(200);

    OLED_WrByte(0xae,OLED_WR_CMD); // Turn off oled panel
    OLED_WrByte(0x00,OLED_WR_CMD); // Set low column address
    OLED_WrByte(0x10,OLED_WR_CMD); // Set high column address
    OLED_WrByte(0x40,OLED_WR_CMD); // Set start line address; Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WrByte(0x81,OLED_WR_CMD); // Set contrast control register
    OLED_WrByte(0xcf,OLED_WR_CMD); // Set SEG Output Current Brightness

    OLED_WrByte(0xa1,OLED_WR_CMD); // Set SEG/Column Mapping     0xa0 左右反置 0xa1 正常
    OLED_WrByte(0xc8,OLED_WR_CMD); // Set COM/Row Scan Direction   0xc0 上下反置 0xc8 正常
    OLED_WrByte(0xa6,OLED_WR_CMD); // Set normal display
    OLED_WrByte(0xa8,OLED_WR_CMD); // Set multiplex ratio(1 to 64)
    OLED_WrByte(0x3f,OLED_WR_CMD); // 1/64 duty

    OLED_WrByte(0xd3,OLED_WR_CMD); // Set display offset; Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WrByte(0x00,OLED_WR_CMD); // Not offset

    OLED_WrByte(0xd5,OLED_WR_CMD); // Set display clock divide ratio/oscillator frequency
    OLED_WrByte(0x80,OLED_WR_CMD); // Set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WrByte(0xd9,OLED_WR_CMD); // Set pre-charge period

    OLED_WrByte(0xf1,OLED_WR_CMD); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	//OLED_WrByte(0x22,OLED_WR_CMD);
    OLED_WrByte(0xda,OLED_WR_CMD); // Set com pins hardware configuration
    OLED_WrByte(0x12,OLED_WR_CMD);
    OLED_WrByte(0xdb,OLED_WR_CMD); // Set vcomh

    OLED_WrByte(0x40,OLED_WR_CMD); // Set VCOM Deselect Level

    OLED_WrByte(0x20,OLED_WR_CMD); // Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WrByte(0x02,OLED_WR_CMD);
    OLED_WrByte(0x8d,OLED_WR_CMD); // Set Charge Pump enable/disable
    OLED_WrByte(0x14,OLED_WR_CMD); // Set(0x10) disable
    OLED_WrByte(0xa4,OLED_WR_CMD); // Disable Entire Display On (0xa4/0xa5)
    OLED_WrByte(0xa6,OLED_WR_CMD); // Disable Inverse Display On (0xa6/a7)
    OLED_WrByte(0xaf,OLED_WR_CMD); // Turn on oled panel

    OLED_Fill(0xff); // 初始清屏
    OLED_Set_Pos(0,0);
}
