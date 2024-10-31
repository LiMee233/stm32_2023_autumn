#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"

extern uint8_t usart1_rx_buffer[BUFFER_SIZE];
extern volatile uint16_t usart1_rx_index;

extern uint8_t usart2_rx_buffer[BUFFER_SIZE];
extern volatile uint16_t usart2_rx_index;

extern uint16_t adc_values[3];

uint8_t ECSendFrame[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0xc4, 0x0b};
double ECResult = 0;

uint8_t T_HSendFrame[] = {0x03, 0x03, 0x01, 0xF4, 0x00, 0x02, 0x85, 0xE7};
double TResult = 0;
double HResult = 0;

uint8_t CO2SendFrame[] = {0x03, 0x03, 0x01, 0xF7, 0x00, 0x00, 0x85, 0xE7};
uint8_t CO2Result = 0;

uint8_t APSendFrame[] = {0x03, 0x03, 0x01, 0xF9, 0x00, 0x00, 0x85, 0xE7};
double APResult = 0;

uint8_t LUXSendFrame[] = {0x03, 0x03, 0x01, 0xFA, 0x00, 0x02, 0x85, 0xE7};
uint16_t LUXResult = 0;

void RefreshSN3002ECN01()
{
    // Set RX index to 0
    usart1_rx_index = 0;

    // Send frame
    USART1_Send(ECSendFrame, 8);
    delay_ms(100);

    // Receive frame
    ECResult = ((uint16_t)((usart1_rx_buffer[3] << 8) | usart1_rx_buffer[4])) / 10.0;
}

void RefreshSN300BYHM()
{
    // Humidity & Temperature
    usart2_rx_index = 0;
    USART2_Send(T_HSendFrame, 8);
    delay_ms(100);
    HResult = ((uint16_t)((usart1_rx_buffer[3] << 8) | usart1_rx_buffer[4])) / 10.0;
    TResult = ((int16_t)((usart1_rx_buffer[5] << 8) | usart1_rx_buffer[6])) / 10.0;

    // CO2
    usart2_rx_index = 0;
    USART2_Send(CO2SendFrame, 8);
    delay_ms(100);
    CO2Result = usart1_rx_buffer[3];

    // Atmospheric pressure
    usart2_rx_index = 0;
    USART2_Send(CO2SendFrame, 8);
    delay_ms(100);
    APResult = usart1_rx_buffer[3] / 10.0;

    // LUX
    usart2_rx_index = 0;
    USART2_Send(LUXSendFrame, 8);
    delay_ms(100);
    APResult = ((uint16_t)((usart1_rx_buffer[3] << 8) | usart1_rx_buffer[4]));
}

double getpH()
{
    return (adc_values[0] / 4096) * 4.24 + (adc_values[1] / 4096) / 298.0;
}

// LiquidLevel (m)
double getLiquidLevel()
{
    return ((adc_values[2] / 4096) * 3.3 - 0.6) / 2.7;
}
