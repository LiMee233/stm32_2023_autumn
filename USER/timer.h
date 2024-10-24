#include "stm32f10x.h"

#ifndef _TIMER_H_
#define _TIMER_H_

uint16_t GetPrescalerFromMillisecond(int);

void InitTIM2(void);
void EnableTIM2(void);
void DisableTIM2(void);

#endif
