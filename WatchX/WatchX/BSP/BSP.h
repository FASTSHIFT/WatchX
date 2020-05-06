#ifndef __BSP_H
#define __BSP_H

#include "Basic/SysConfig.h"
#include "Arduino.h"

/*BMP*/
#include "Adafruit_BMP180/Adafruit_BMP085.h"
extern Adafruit_BMP085 BMP180;

/*Backlight*/
uint16_t Backlight_GetValue();
void Backlight_SetValue(int16_t val);
void Backlight_SetGradual(uint16_t target, uint16_t time = 500);

/*Buttons*/
#include "ButtonEvent/ButtonEvent.h"
extern ButtonEvent btUP;
extern ButtonEvent btOK;
extern ButtonEvent btDOWN;

/*Power*/
void Power_EnterLowPowerMode();
void Power_HandleTimeUpdate();
void Power_SetAutoLowPowerTimeout(int16_t ms);
void Power_SetAutoLowPowerEnable(bool en);
void Task_AutoPowerDown();

#endif
