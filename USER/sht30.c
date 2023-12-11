#include "stm32f10x.h"
#include "sht30.h"
#include "user_iic.h"	
#include <string.h>
#include "delay.h"
#include "stdio.h"
#include "oled.h"

 float temperatureC;
 float humidityRH;

uint8_t IsTemperatureCalculated = 0;

uint8_t buffer[6];

/**************************************************************************************
* 描  述 : 向SHT30写寄存器命令
* 入  参 : cmd: 两字节命令
* 返回值 :   1---成功写入,     0---失败
**************************************************************************************/
FunctionalState SHT3X_WriteCMD(uint16_t cmd)
{
	if(!IIC_Start())   return DISABLE;
	IIC_Send_Byte(i2cAddWrite_8bit);
	if(!IIC_Wait_Ack()){IIC_Stop(); return DISABLE;}
	IIC_Send_Byte(cmd>>8);
	IIC_Wait_Ack();
	IIC_Send_Byte(cmd);
	IIC_Wait_Ack();
	IIC_Stop();
	return ENABLE;
}

 /**************************************************************************************
 * 描  述 : 设置测量周期
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void SHT3X_SetPeriodicMeasurement(void)
{
    SHT3X_WriteCMD(CMD_MEAS_PERI_2_H);
}

 /**************************************************************************************
 * 描  述 : 初始化SHT3X传感器
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void SHT3X_Init(void)
{
    sw_delay_ms(100); 
    SHT3X_SetPeriodicMeasurement();
    sw_delay_ms(100); 
}

 /**************************************************************************************
 * 描  述 : 从SHT30读取状态信息
 * 入  参 : temp: 待读出地址
 * 返回值 :   1---成功读取,     0---失败
 **************************************************************************************/
FunctionalState SHT3X_ReadState(uint8_t *temp)
{
   if(!IIC_Start())return DISABLE;
	IIC_Send_Byte(i2cAddWrite_8bit);
	if(!IIC_Wait_Ack())		{IIC_Stop(); return DISABLE;}
	IIC_Send_Byte(0xf3);
	IIC_Wait_Ack();
	IIC_Send_Byte(0X2d);
	IIC_Wait_Ack();
   IIC_Stop(); 
	sw_delay_ms(100);
  if(!IIC_Start())return DISABLE;
	IIC_Send_Byte(i2cAddRead_8bit);
	IIC_Wait_Ack();
	temp[0] = IIC_Read_Byte(0x00);
	IIC_Ack();
	temp[1] = IIC_Read_Byte(0x00);
	IIC_Ack();
	temp[2] = IIC_Read_Byte(0x00);
	IIC_NAck(); 
  IIC_Stop(); 
	return ENABLE;
}

 /**************************************************************************************
 * 描  述 : 从SHT30读取测量结果
 * 入  参 : cmd: 待配置命令    p：待读出地址
 * 返回值 :   1---成功读取,     0---失败
 **************************************************************************************/
FunctionalState SHX3X_ReadResults(uint16_t cmd,  uint8_t *p)
{
	if(!IIC_Start())return DISABLE;
	IIC_Send_Byte(0x88);
	if(!IIC_Wait_Ack())		{IIC_Stop(); return DISABLE;}
	IIC_Send_Byte(cmd>>8);
	IIC_Wait_Ack();
	IIC_Send_Byte(cmd);
	IIC_Wait_Ack();
	IIC_Stop();
	
   if(!IIC_Start())return DISABLE;
	IIC_Send_Byte(0x89);
	if(!IIC_Wait_Ack())		{IIC_Stop(); return DISABLE;}

	p[0] = IIC_Read_Byte(0x00);
	IIC_Ack();
	p[1] = IIC_Read_Byte(0x00);
	IIC_Ack();
	p[2] = IIC_Read_Byte(0x00);
	IIC_Ack();
	p[3] = IIC_Read_Byte(0x00);
	IIC_Ack();
	p[4] = IIC_Read_Byte(0x00);
	IIC_Ack();
	p[5] =IIC_Read_Byte(0x00);
	IIC_NAck();
	IIC_Stop();
	return ENABLE;
}

 /**************************************************************************************
 * 描  述 : SHT30校验函数
 * 入  参 : data[]: 等待检验的数据   startByte: 开始数组的标号
            number: 检验个数         checksum: 接收到的CRC数据
 * 返回值 :   1---成功检测,     0---失败
 **************************************************************************************/
FunctionalState SHT3x_CheckCrc(uint8_t data[],uint8_t startBytes,uint8_t number, uint8_t checksum)
{
   uint8_t bit=0;
	uint8_t crc = 0xFF;	
   uint8_t byteCtr;

  for (byteCtr = startBytes; byteCtr < startBytes+number; byteCtr++)
  { crc ^= (data[byteCtr]);   //先与需要计算的数异或，再指向下一个数
    for (bit = 8; bit > 0; --bit)
    { if (crc & 0x80)    //判断最高位是否为1  
       crc = (crc << 1) ^ POLYNOMIAL;  //最高位为1，左移一位与多项式异或
      else 
       crc = (crc << 1);      //最高位为0，直接左移一位
    }
  }
  if (crc ==checksum) 
	return ENABLE;
  else
	return DISABLE;
}

 /**************************************************************************************
 * 描  述 : 计算温度函数
 * 入  参 : dat：读取的温度原始值
 * 返回值 : 计算出的温度值
 **************************************************************************************/
float SHT3X_CalcTemperature(uint16_t dat)
{
  float temp;
  temp = (175 * (float)dat / 65535 - 45) ;    //单位 [℃]
  return temp;
}

 /**************************************************************************************
 * 描  述 : 计算湿度函数
 * 入  参 : dat：读取的湿度原始值
 * 返回值 : 计算出的湿度值
 **************************************************************************************/
float SHT3X_CalcRH(uint16_t dat)
{
  float temp;
  temp = (100 * (float)dat / 65535) ;  //单位 [%RH]
  return temp;
}

 /**************************************************************************************
 * 描  述 : SHT30测试函数
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void SHT3X_TEST(void)
{
	uint8_t p[6], Temp1, Temp2;
	float cTemp, rhTemp;

	// 先设定温度没有计算完成
	IsTemperatureCalculated = 0;

	// 清零数组 p
	memset(p, 0, 6);

	// 周期模式读出温湿度原始值，将原始值存放数组 p，存放数组对应信息格式如下：
	// 温度高 8 位 + 温度低 8 位 + 温度校验 8 位 + 湿度高 8 位 + 湿度低 8 位 + 湿度校验 8 位
	if(SHX3X_ReadResults(CMD_FETCH_DATA, buffer))
	{
		p[0] = buffer[0];
		p[1] = buffer[1];
		p[2] = buffer[2];
		p[3] = buffer[3];
		p[4] = buffer[4];
		p[5] = buffer[5];
	}
	 
   //校验读出的数据
   Temp1=SHT3x_CheckCrc(p,0,2, p[2]);
	Temp2=SHT3x_CheckCrc(p,3,2, p[5]);
	 
	// 读取温度信息
	if(Temp1 && Temp2)
	{
		// 此时，温度计算完成
		IsTemperatureCalculated = 1;

		Temp1=0;
		Temp2=0;

		// 计算温度原始信息，并带入算法计算出实际温度值
		cTemp = (p[0] * 256.0) + p[1];
		temperatureC = SHT3X_CalcTemperature(cTemp);
	 
		// 计算湿度原始信息，并带入算法计算出实际湿度值
		rhTemp = (p[3] * 256.0) + p[4];
		humidityRH = SHT3X_CalcRH(rhTemp);
	}
}
