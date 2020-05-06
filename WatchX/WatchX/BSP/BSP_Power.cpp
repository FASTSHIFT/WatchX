#include "BSP.h"
#include "GUI/DisplayPrivate.h"

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

void Power_EnterLowPowerMode()
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

    attachInterrupt(KEY_OK_Pin, Power_AwakeCallback, FALLING);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
    while(1);
}

static uint32_t Power_LastHandleTime = 0;
static int16_t Power_AutoLowPowerTimeout = 60;
static bool Power_AutoLowPowerEnable = true;

void Power_HandleTimeUpdate()
{
    Power_LastHandleTime = millis();
}

void Power_SetAutoLowPowerTimeout(int16_t ms)
{
    Power_AutoLowPowerTimeout = ms;
}

void Power_SetAutoLowPowerEnable(bool en)
{
    Power_AutoLowPowerEnable = en;
    Power_HandleTimeUpdate();
}

void Task_AutoPowerDown()
{
    if(!Power_AutoLowPowerEnable)
        return;
    
    if(Power_AutoLowPowerTimeout < 0)
        return;
    
    if(millis() - Power_LastHandleTime >= (Power_AutoLowPowerTimeout * 1000))
    {
        Backlight_SetGradual(0, 500);
        PageDelay(500);
        Power_EnterLowPowerMode();
    }
}
