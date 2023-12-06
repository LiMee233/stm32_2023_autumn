#include "stm32f10x.h"

uint16_t GetPrescalerFromMillisecond(int millisecond);

void InitTIM3(void);
void EnableTIM3(void);
void DisableTIM3(void);
void TIM3_IRQHandler(void);
