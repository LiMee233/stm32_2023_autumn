#include "stm32f10x.h"
#include "keyboard.h"
#include "delay.h"

void KeyBoardGPIOInit()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    KEYBOARD_GPIO_CLK_FUN(KEYBOARD_GPIO_CLK, ENABLE);

    // Row
    GPIO_InitStruct.GPIO_Pin =  KEYBOARD_ROW0_GPIO_PIN |
                                KEYBOARD_ROW1_GPIO_PIN |
                                KEYBOARD_ROW2_GPIO_PIN |
                                KEYBOARD_ROW3_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEYBOARD_GPIO_PORT, &GPIO_InitStruct);

    // Column
    GPIO_InitStruct.GPIO_Pin =  KEYBOARD_COLUMN0_GPIO_PIN |
                                KEYBOARD_COLUMN1_GPIO_PIN |
                                KEYBOARD_COLUMN2_GPIO_PIN |
                                KEYBOARD_COLUMN3_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEYBOARD_GPIO_PORT, &GPIO_InitStruct);
}

uint16_t GetKeyBoardValue()
{
    uint16_t keyBoardValue = 0;
    uint16_t _temp = 0;

    // Scan Row 1
    GPIOA -> ODR = 0x00;
    GPIOA -> ODR = 0xFE;

    if((GPIOA -> IDR & 0xF0) != 0xF0)
    {
        delay_ms(50);

        if((GPIOA -> IDR & 0xF0) != 0xF0)
        {
            _temp = GPIOA -> IDR & 0xFE;
            switch(_temp)
            {
                case 0xEE:  keyBoardValue = 1; break;
                case 0xDE:  keyBoardValue = 2; break;
                case 0xBE:  keyBoardValue = 3; break;
                case 0x7E:  keyBoardValue = 4; break;
                default:    keyBoardValue = 0; break;
            }
        }
    }

    // Scan Row 2
    GPIOA -> ODR = 0x00;
    GPIOA -> ODR = 0xFD;

    if((GPIOA -> IDR & 0xF0) != 0xF0)
    {
        delay_ms(50);

        if((GPIOA -> IDR & 0xF0) != 0xF0)
        {
            _temp = GPIOA -> IDR & 0xFD;
            switch(_temp)
            {
                case 0xED:  keyBoardValue = 5; break;
                case 0xDD:  keyBoardValue = 6; break;
                case 0xBD:  keyBoardValue = 7; break;
                case 0x7D:  keyBoardValue = 8; break;
                default:    keyBoardValue = 0; break;
            }
        }
    }

    // Scan Row 3
    GPIOA -> ODR = 0x00;
    GPIOA -> ODR = 0xFB;

    if((GPIOA -> IDR & 0xF0) != 0xF0)
    {
        delay_ms(50);

        if((GPIOA -> IDR & 0xF0) != 0xF0)
        {
            _temp = GPIOA -> IDR & 0xFB;
            switch(_temp)
            {
                case 0xEB:  keyBoardValue = 9; break;
                case 0xDB:  keyBoardValue = 10; break;
                case 0xBB:  keyBoardValue = 11; break;
                case 0x7B:  keyBoardValue = 12; break;
                default:    keyBoardValue = 0; break;
            }
        }
    }

    // Scan Row 4
    GPIOA -> ODR = 0x00;
    GPIOA -> ODR = 0xF7;

    if((GPIOA -> IDR & 0xF0) != 0xF0)
    {
        delay_ms(50);

        if((GPIOA -> IDR & 0xF0) != 0xF0)
        {
            _temp = GPIOA -> IDR & 0xF7;
            switch(_temp)
            {
                case 0xE7:  keyBoardValue = 13; break;
                case 0xD7:  keyBoardValue = 14; break;
                case 0xB7:  keyBoardValue = 15; break;
                case 0x77:  keyBoardValue = 16; break;
                default:    keyBoardValue = 0; break;
            }
        }
    }

    return keyBoardValue;
}
