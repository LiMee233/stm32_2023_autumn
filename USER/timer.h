#include "stm32f10x.h"

uint16_t GetPrescalerFromMillisecond(TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct, int millisecond);

/************************ TIM3 ************************/
void InitTIM3(void);
void EnableTIM3(void);
void DisableTIM3(void);
void TIM3_IRQHandler(void);

/************************ TIM2 ************************/
void InitTIM2(void);
void EnableTIM2(void);
void DisableTIM2(void);
void TIM2ChangeTime(int);
void TIM2_IRQHandler(void);

/************************ TIM4 ************************/
void InitTIM4(void);
void EnableTIM4(void);
void DisableTIM4(void);
void TIM4_IRQHandler(void);
