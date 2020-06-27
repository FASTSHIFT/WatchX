#include "FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "MillisTaskManager/MillisTaskManager.h"

static MillisTaskManager mtmMain(5);
float CPU_Usage;

static void CPU_UsageUpdate()
{
    CPU_Usage = mtmMain.GetCPU_Usage();
}

static void setup()
{
   /*LED*/
    pinMode(LED_Pin, OUTPUT);
    digitalWrite(LED_Pin, HIGH);
    
    /*电源保持*/
    pinMode(POWER_ON_Pin, OUTPUT);
    digitalWrite(POWER_ON_Pin, HIGH);
    
    /*电源按键*/
    pinMode(POWER_EN_Pin, INPUT);
    
    /*电池电压检测*/
    pinMode(BAT_DET_Pin, INPUT_ANALOG_DMA);
    ADC_DMA_Init();
    
    /*电池充电检测*/
    pinMode(BAT_CHG_Pin, INPUT_PULLUP);

    /*设备初始化*/
    RTCx_Init();
    Power_Init();
    Backlight_Init();
    Button_Init();
    Display_Init();
    BMP_Init();

    digitalWrite(LED_Pin, LOW);

    mtmMain.TaskRegister(0, Display_Update, 1);
    mtmMain.TaskRegister(1, Button_Update, 10);
    mtmMain.TaskRegister(2, Power_AutoShutdownUpdate, 100);
    mtmMain.TaskRegister(3, CPU_UsageUpdate, 1000);
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
    setup();
    for(;;)loop();
}
