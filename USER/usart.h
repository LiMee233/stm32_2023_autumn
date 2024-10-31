#include "stm32f10x.h"

#ifndef _USART_H
#define _USART_H

#define BUFFER_SIZE 64

void USART1_Init(void);
void USART1_Send(uint8_t *data, uint16_t len);
void USART1_IRQHandler(void);
void USART2_Init(void);
void USART2_Send(uint8_t *data, uint16_t len);
void USART2_IRQHandler(void);

#endif
