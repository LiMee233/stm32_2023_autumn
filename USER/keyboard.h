#include "stm32f10x.h"

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

// GPIO Port
#define KEYBOARD_GPIO_PORT              GPIOA
#define KEYBOARD_GPIO_CLK_FUN           RCC_APB2PeriphClockCmd
#define KEYBOARD_GPIO_CLK               RCC_APB2Periph_GPIOA

// Row
#define KEYBOARD_ROW0_GPIO_PIN          GPIO_Pin_0
#define KEYBOARD_ROW1_GPIO_PIN          GPIO_Pin_1
#define KEYBOARD_ROW2_GPIO_PIN          GPIO_Pin_2
#define KEYBOARD_ROW3_GPIO_PIN          GPIO_Pin_3

// Column
#define KEYBOARD_COLUMN0_GPIO_PIN       GPIO_Pin_4
#define KEYBOARD_COLUMN1_GPIO_PIN       GPIO_Pin_5
#define KEYBOARD_COLUMN2_GPIO_PIN       GPIO_Pin_6
#define KEYBOARD_COLUMN3_GPIO_PIN       GPIO_Pin_7

void KeyBoardGPIOInit(void);
uint16_t GetKeyBoardValue(void);

#endif
