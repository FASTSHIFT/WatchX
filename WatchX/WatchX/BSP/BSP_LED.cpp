#include "BSP.h"

void LED_Init()
{
    pinMode(LED_Pin, OUTPUT);
}

void LED_SetEnable(bool en)
{
    digitalWrite(LED_Pin, en);
}
