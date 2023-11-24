#include "stm32f10x.h"

uint16_t GetPrescalerFromMillisecond(TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct, int millisecond);
void InitTIM3(void);
void EnableTIM3(void);
void DisableTIM3(void);

void InitTIM2(void);
void EnableTIM2(void);
void DisableTIM2(void);
void TIM2ChangeTime(int);

void InitTIM4(void);
void EnableTIM4(void);
void DisableTIM4(void);
