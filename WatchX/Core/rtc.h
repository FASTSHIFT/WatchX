#ifndef __RTC_H
#define __RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mcu_type.h"

void RTCx_Init(void);
ErrorStatus RTCx_SetTime(uint8_t hour,uint8_t min,uint8_t sec,uint8_t ampm);
ErrorStatus RTCx_SetDate(uint8_t year,uint8_t month,uint8_t date,uint8_t week);
void RTCx_SetAlarmA(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec);
void RTCx_SetWakeUp(uint32_t wksel,uint16_t cnt);
void RTCx_GetWeekDay(RTC_DateTypeDef *RTC_DateStruct);
    
#ifdef __cplusplus
}
#endif

#endif /* __RTC_H */
