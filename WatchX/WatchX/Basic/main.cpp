#include "BSP/BSP.h"
#include "GUI/DisplayPrivate.h"
#include "MillisTaskManager/MillisTaskManager.h"

/*任务调度器*/
static MillisTaskManager mtmMain;

/*CPU占用率(DEBUG用)*/
float CPU_Usage;

/**
  * @brief  CPU占用率更新
  * @param  无
  * @retval 无
  */
static void CPU_UsageUpdate()
{
    CPU_Usage = mtmMain.GetCPU_Usage();
}

/**
  * @brief  系统初始化
  * @param  无
  * @retval 无
  */
static void setup()
{
    LED_Init();
    LED_SetEnable(true);

    /*设备初始化*/
    Power_Init();
    RTCx_Init();
    Backlight_Init();
    Button_Init();
    Display_Init();
    BMP_Init();
    ADC_DMA_Init();

    LED_SetEnable(false);

    /*任务注册*/
    mtmMain.Register(Display_Update, 1);                //屏幕刷新
    mtmMain.Register(Button_Update, 10);                //按键事件监控
    mtmMain.Register(Power_AutoShutdownUpdate, 100);    //自动关机监控
    mtmMain.Register(CPU_UsageUpdate, 1000);            //CPU占用率监控
}

/**
  * @brief  系统主循环
  * @param  无
  * @retval 无
  */
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
