#include "stm32f10x.h"

uint16_t GetPrescalerFromMillisecond(int millisecond);

void InitTIM2(void);
void EnableTIM2(void);
void DisableTIM2(void);
void TIM2_IRQHandler(void);

void InitTIM3(void);
void EnableTIM3(void);
void DisableTIM3(void);
void TIM3_IRQHandler(void);
