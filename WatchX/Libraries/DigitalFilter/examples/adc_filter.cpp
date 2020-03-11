#include "DigitalFilter.h"

/*均值滤波器，取最近13个数，去除最大值和最小值，剩余数求平均*/
FilterAverage<uint16_t> ADC_FA(13);

/*滞回滤波器，去除50以下的波动*/
FilterHysteresis<uint16_t> ADC_FH(50);

/*ADC采样引脚*/
#define Sampling_Pin PA0

void setup()
{
    pinMode(Sampling_Pin, INPUT_ANALOG);
    Serial.begin(115200);
}

void loop()
{
    uint16_t adc_val = analogRead(Sampling_Pin);
    Serial.printf(
        "ADC Val:%d, FilterAverage:%d, FilterHysteresis:%d\r\n", 
        adc_val, 
        ADC_FA.getNext(adc_val),
        ADC_FH.getNext(adc_val)
    );
}
