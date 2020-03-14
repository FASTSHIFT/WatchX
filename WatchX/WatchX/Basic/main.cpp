#include "FileGroup.h"
#include "BSP/BSP.h"

static void setup()
{
    pinMode(LED_Pin, OUTPUT);
    digitalWrite(LED_Pin, HIGH);
    
    pinMode(BAT_DET_Pin, INPUT_ANALOG_DMA);
    ADC_DMA_Init();
    pinMode(BAT_CHG_Pin, INPUT);
    
    RTCx_Init();
    Display_Init();
    
    digitalWrite(LED_Pin, LOW);
}

static void loop()
{
    Task_Display();
    __IntervalExecute(Task_ButtonMonitor(), 10);
}

/**
  * @brief  Main Function
  * @param  None
  * @retval None
  */
int main(void)
{
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
    Delay_Init();
    //ADCx_Init(ADC1);
    setup();
    for(;;)loop();
}
