#include "BSP.h"
#include "Basic/FileGroup.h"

static void LED_Init()
{
    pinMode(LED_Pin, OUTPUT);
}

void LED_SetState(bool en)
{
    __ExecuteOnce(LED_Init());
    digitalWrite(LED_Pin, en);
}
