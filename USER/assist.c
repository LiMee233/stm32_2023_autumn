#include "stm32f10x.h"

char* RemoveLastChar(char* str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }

    if(len == 0)
        return str;

    str[len - 1] = '\0';
    return str;
}

char* AppendChar(char* str, char ch, uint8_t max_len)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }

    if(len < max_len - 1)
    {
        str[len] = ch;
        str[len + 1] = '\0';
    }

    return str;
}

char* Uint8_t2String(uint8_t value)
{
    static char str[4];
    int i = 0;
    int j = 0;

    do
    {
        str[i++] = (value % 10) + '0';
        value /= 10;
    } while (value > 0);

    str[i] = '\0';

    for (j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }

    return str;
}

char* Double2String(double value, uint8_t decimal_places) {
    static char str[50];
    int integer_part = (int)value;
    double fraction_part = value - integer_part;

    int i = 0;
    int j = 0;
    int digit = 0;

    if (integer_part == 0) {
        str[i++] = '0';
    } else {
        int temp = integer_part;
        while (temp > 0) {
            str[i++] = (temp % 10) + '0';
            temp /= 10;
        }

        for (j = 0; j < i / 2; j++) {
            char tmp = str[j];
            str[j] = str[i - j - 1];
            str[i - j - 1] = tmp;
        }
    }

    str[i++] = '.';

    for (j = 0; j < decimal_places; j++) {
        fraction_part *= 10;
        digit = (int)fraction_part;
        str[i++] = digit + '0';
        fraction_part -= digit;
    }

    str[i] = '\0';

    return str;
}
