#include "stm32f10x.h"

void DA_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef DAC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 开启 DAC 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;

    DAC_Init(DAC_Channel_1,&DAC_InitStructure);

    DAC_Cmd(DAC_Channel_1,ENABLE);
}

void setVoltage(double voltage)
{
    uint16_t temp;

    if(voltage < 0.0 || voltage > 4.0f)
        return;

    temp = (uint16_t)(4095 * voltage / 4.0f) & 0x0fff;

    DAC_SetChannel1Data(DAC_Align_12b_R,temp);     // 电压输出：12位右对齐
}
