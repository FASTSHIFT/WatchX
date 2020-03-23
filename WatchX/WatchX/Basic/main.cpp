#include "FileGroup.h"
#include "MillisTaskManager/MillisTaskManager.h"

static MillisTaskManager mtmMain(5);
float CPU_Usage;

static void Task_CPU_UsageUpdate()
{
    CPU_Usage = mtmMain.GetCPU_Usage();
}

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
    
    mtmMain.TaskRegister(0, Task_Display, 1);
    mtmMain.TaskRegister(1, Task_ButtonMonitor, 10);
    mtmMain.TaskRegister(2, Task_AutoPowerDown, 100);
    mtmMain.TaskRegister(3, Task_CPU_UsageUpdate, 1000);
}

static void loop()
{
    mtmMain.Running(millis());
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
