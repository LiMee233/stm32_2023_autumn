#include "stm32f10x.h"
#include "usart.h"

uint8_t usart1_rx_buffer[BUFFER_SIZE];  // USART1 接收缓冲区
volatile uint16_t usart1_rx_index = 0;  // 当前 USART1 缓冲区索引

uint8_t usart2_rx_buffer[BUFFER_SIZE];  // USART2 接收缓冲区
volatile uint16_t usart2_rx_index = 0;  // 当前 USART2 缓冲区索引

void USART1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA |
                            RCC_APB2Periph_AFIO |
                            RCC_APB2Periph_USART1 ,
                            ENABLE);

    // PA9 & PA10 是 USART1 的 TX 和 RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // usart 作为输出 TX 发送
    USART_InitStructure.USART_BaudRate = 4800;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStructure);

    // usart 需要作为输入 RX 接收加上 ITconfig 和 NVIC
    // USART_IT_RXNE 此标志位一旦置 1 就会向 NVIC 申请中断，之后可以在中断函数里接收数据
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    // 打开 USART
    USART_Cmd(USART1, ENABLE);
}

void USART1_Send(uint8_t *data, uint16_t len) {
    uint16_t i;
    for (i = 0; i < len; i++) {
        // 发送一个字节
        USART_SendData(USART1, data[i]);

        // 等待发送寄存器空闲
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}

void USART1_IRQHandler(void) {
    // 检查是否为接收中断
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        // 读取接收到的数据
        uint8_t data = USART_ReceiveData(USART1);

        // 存入 USART1 接收缓冲区
        if (usart1_rx_index < BUFFER_SIZE) {
            usart1_rx_buffer[usart1_rx_index++] = data;
        }

        // 清除接收中断标志位
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

void USART2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB |
                            RCC_APB2Periph_AFIO ,
                            ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    // PB10 & PB11 是 USART3 的 TX 和 RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // usart 作为输出 TX 发送
    USART_InitStructure.USART_BaudRate = 4800;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART3, &USART_InitStructure);

    // usart 需要作为输入 RX 接收加上 ITconfig 和 NVIC
    // USART_IT_RXNE 此标志位一旦置 1 就会向 NVIC 申请中断，之后可以在中断函数里接收数据
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    // 打开 USART
    USART_Cmd(USART1, ENABLE);
}

void USART2_Send(uint8_t *data, uint16_t len) {
    uint16_t i;
    for (i = 0; i < len; i++) {
        // 发送一个字节
        USART_SendData(USART2, data[i]);

        // 等待发送寄存器空闲
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }
}

void USART2_IRQHandler(void) {
    // 检查是否为接收中断
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        // 读取接收到的数据
        uint8_t data = USART_ReceiveData(USART2);

        // 存入 USART2 接收缓冲区
        if (usart2_rx_index < BUFFER_SIZE) {
            usart2_rx_buffer[usart2_rx_index++] = data;
        }

        // 清除接收中断标志位
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}
