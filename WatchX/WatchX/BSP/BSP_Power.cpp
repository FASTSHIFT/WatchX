#include "BSP.h"
#include "GUI/DisplayPrivate.h"
#include "rtc.h"

/*在进低功耗模式时，被配置为拉低的引脚*/
static const uint8_t PinGrpLow[] =
{
    TFT_CS_Pin,
    TFT_DC_Pin,
    TFT_RST_Pin,
    TFT_BLK_Pin,
    
    LED_Pin,

    KEY_UP_Pin,
//    KEY_OK_Pin,   //使用OK键唤醒
    KEY_DOWN_Pin
};

/*在进低功耗模式时，被配置为浮空输入的引脚*/
static const uint8_t PinGrpFloating[] =
{
    IMU_SCL_Pin,
    IMU_SDA_Pin,
    IMU_INT_Pin,
    BMP_SCL_Pin,
    BMP_SDA_Pin,
    BAT_DET_Pin,
    BAT_CHG_Pin
};

/**
  * @brief  低功耗唤醒事件，被外部中断触发
  * @param  无
  * @retval 无
  */
static void Power_AwakeCallback()
{
    digitalWrite(LED_Pin, HIGH);
    NVIC_SystemReset();           //软件重启
}

/**
  * @brief  进入低功耗模式
  * @param  无
  * @retval 无
  */
static void Power_EnterLowPowerMode()
{
    /*遍历引脚，配置模式*/
    for(uint8_t i = 0; i < sizeof(PinGrpLow); i++)
    {
        pinMode(PinGrpLow[i], OUTPUT);
        digitalWrite(PinGrpLow[i], LOW);
    }
    for(uint8_t i = 0; i < sizeof(PinGrpFloating); i++)
    {
        pinMode(PinGrpFloating[i], INPUT);
    }
    
    /*关外设*/
    ADC_Cmd(ADC1, DISABLE);
    ADC_Cmd(ADC2, DISABLE);
    DMA_Cmd(DMA2_Stream0, DISABLE);
    TIM_Cmd(TIM1, DISABLE);
    TIM_Cmd(TIM2, DISABLE);
    TIM_Cmd(TIM3, DISABLE);
    TIM_Cmd(TIM4, DISABLE);
    
    /*等待松开电源键*/
    while (digitalRead(POWER_EN_Pin)) __NOP();
    digitalWrite(POWER_ON_Pin, LOW);

    /*注册外部中断回调*/
    attachInterrupt(KEY_OK_Pin, Power_AwakeCallback, FALLING);
    
    /*进STOP模式*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
    while(1);
}

/*上一次操作时间(ms)*/
static uint32_t Power_LastHandleTime = 0;

/*自动关机时间(秒)*/
static uint16_t Power_AutoLowPowerTimeout = 60;

/*自动关机功能使能*/
static bool Power_AutoLowPowerEnable = true;

/**
  * @brief  从后备寄存器获取自动关机时间
  * @param  无
  * @retval 时间(秒)
  */
static uint32_t Power_GetBKP()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);   //使能PWR和BKP外设时钟
    PWR_BackupAccessCmd(ENABLE);                          //使能后备寄存器访问
    
    uint16_t value = RTC_ReadBackupRegister(RTC_BKP_DR3);
    
    if(value == 0 || value > 300)
    {
        value = 60;
    }
    
    return value;
}

/**
  * @brief  将当前自动关机时间更新至后备寄存器
  * @param  无
  * @retval 无
  */
static void Power_UpdateBKP()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);   //使能PWR和BKP外设时钟
    PWR_BackupAccessCmd(ENABLE);  //使能后备寄存器访问
    RTC_WaitForSynchro();   //等待最近一次对RTC寄存器的写操作完成
    RTC_WriteBackupRegister(RTC_BKP_DR3, Power_GetAutoLowPowerTimeout()); //写入后备寄存器
    RTC_WaitForSynchro();   //等待最近一次对RTC寄存器的写操作完成
}

/**
  * @brief  电源初始化
  * @param  无
  * @retval 无
  */
void Power_Init()
{
    /*电源保持*/
    pinMode(POWER_ON_Pin, OUTPUT);
    digitalWrite(POWER_ON_Pin, HIGH);
    
    /*电源按键*/
    pinMode(POWER_EN_Pin, INPUT_PULLDOWN);
    
    /*电池充电检测*/
    pinMode(BAT_CHG_Pin, INPUT_PULLUP);
    
    /*电池电压检测*/
    pinMode(BAT_DET_Pin, INPUT_ANALOG_DMA);
    
    Power_SetAutoLowPowerTimeout(Power_GetBKP());
    Power_HandleTimeUpdate();
}

/**
  * @brief  更新操作时间
  * @param  无
  * @retval 无
  */
void Power_HandleTimeUpdate()
{
    Power_LastHandleTime = millis();
}

/**
  * @brief  设置自动关机时间
  * @param  sec:时间(秒)
  * @retval 无
  */
void Power_SetAutoLowPowerTimeout(uint16_t sec)
{
    Power_AutoLowPowerTimeout = sec;
}

/**
  * @brief  获取自动关机时间
  * @param  无
  * @retval sec:时间(秒)
  */
uint16_t Power_GetAutoLowPowerTimeout()
{
    return Power_AutoLowPowerTimeout;
}

/**
  * @brief  设置自动关机功能使能
  * @param  en:使能
  * @retval 无
  */
void Power_SetAutoLowPowerEnable(bool en)
{
    Power_AutoLowPowerEnable = en;
    Power_HandleTimeUpdate();
}

/**
  * @brief  执行关机
  * @param  无
  * @retval 无
  */
void Power_Shutdown()
{
    Backlight_UpdateBKP();
    Backlight_SetGradual(0, 500);
    Power_UpdateBKP();
    PageDelay(500);
    Power_EnterLowPowerMode();
}

/**
  * @brief  自动关机监控
  * @param  无
  * @retval 无
  */
void Power_AutoShutdownUpdate()
{
    if(!Power_AutoLowPowerEnable)
        return;
    
    if(Power_AutoLowPowerTimeout == 0)
        return;
    
    if(millis() - Power_LastHandleTime >= (Power_AutoLowPowerTimeout * 1000))
    {
        Power_Shutdown();
    }
}
