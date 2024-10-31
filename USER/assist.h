#include "stm32f10x.h"

#ifndef _ASSIST_H
#define _ASSIST_H

char* RemoveLastChar(char* str);
char* AppendChar(char* str, char ch, uint8_t max_len);
char* Uint8_t2String(uint8_t value);
char* Double2String(double value, uint8_t decimal_places);

#endif