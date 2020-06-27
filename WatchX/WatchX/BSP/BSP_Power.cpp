#include "BSP.h"
#include "GUI/DisplayPrivate.h"
#include "rtc.h"

static const uint8_t PinGrpLow[] =
{
    TFT_CS_Pin,
    TFT_DC_Pin,
    TFT_RST_Pin,
    TFT_LED_Pin,
    
    LED_Pin,

    KEY_UP_Pin,
//    KEY_OK_Pin,
    KEY_DOWN_Pin
};

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

static void Power_AwakeCallback()
{
    digitalWrite(LED_Pin, HIGH);
    NVIC_SystemReset();
}

static void Power_EnterLowPowerMode()
{
    for(uint8_t i = 0; i < sizeof(PinGrpLow); i++)
    {
        pinMode(PinGrpLow[i], OUTPUT);
        digitalWrite(PinGrpLow[i], LOW);
    }
    for(uint8_t i = 0; i < sizeof(PinGrpFloating); i++)
    {
        pinMode(PinGrpFloating[i], INPUT);
    }
    ADC_Cmd(ADC1, DISABLE);
    ADC_Cmd(ADC2, DISABLE);
    DMA_Cmd(DMA2_Stream0, DISABLE);
    TIM_Cmd(TIM1, DISABLE);
    TIM_Cmd(TIM2, DISABLE);
    TIM_Cmd(TIM3, DISABLE);
    TIM_Cmd(TIM4, DISABLE);
    
    while (digitalRead(POWER_EN_Pin)) __NOP();//等待松开电源键
    digitalWrite(POWER_ON_Pin, LOW);

    attachInterrupt(KEY_OK_Pin, Power_AwakeCallback, FALLING);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
    while(1);
}

static uint32_t Power_LastHandleTime = 0;
static uint16_t Power_AutoLowPowerTimeout = 60;
static bool Power_AutoLowPowerEnable = true;

static uint32_t Power_GetBKP()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);   //使能PWR和BKP外设时钟
    PWR_BackupAccessCmd(ENABLE);  //使能后备寄存器访问
    
    uint16_t value = RTC_ReadBackupRegister(RTC_BKP_DR3);
    
    if(value == 0 || value > 300)
    {
        value = 60;
    }
    
    return value;
}

static void Power_UpdateBKP()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);   //使能PWR和BKP外设时钟
    PWR_BackupAccessCmd(ENABLE);  //使能后备寄存器访问
    RTC_WaitForSynchro();   //等待最近一次对RTC寄存器的写操作完成
    RTC_WriteBackupRegister(RTC_BKP_DR3, Power_GetAutoLowPowerTimeout()); //写入后备寄存器
    RTC_WaitForSynchro();   //等待最近一次对RTC寄存器的写操作完成
}

void Power_Init()
{
    Power_SetAutoLowPowerTimeout(Power_GetBKP());
    Power_HandleTimeUpdate();
}

void Power_HandleTimeUpdate()
{
    Power_LastHandleTime = millis();
}

void Power_SetAutoLowPowerTimeout(uint16_t ms)
{
    Power_AutoLowPowerTimeout = ms;
}

uint16_t Power_GetAutoLowPowerTimeout()
{
    return Power_AutoLowPowerTimeout;
}

void Power_SetAutoLowPowerEnable(bool en)
{
    Power_AutoLowPowerEnable = en;
    Power_HandleTimeUpdate();
}

void Power_Shutdown()
{
    Backlight_UpdateBKP();
    Backlight_SetGradual(0, 500);
    Power_UpdateBKP();
    PageDelay(500);
    Power_EnterLowPowerMode();
}

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
