#include "stm32f10x.h"
#ifndef _USART_H
#define _USART_H

void Serial_Init(void);
void Serial_SendByte(uint16_t Byte);
uint16_t Serial_GetRxFlag(void);

#endif
