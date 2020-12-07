#ifndef __BSP_H
#define __BSP_H

#include "Basic/SysConfig.h"
#include "Arduino.h"

/*BMP*/
#include "Adafruit_BMP180/Adafruit_BMP085.h"
extern Adafruit_BMP085 BMP180;
void BMP_Init();
void BMP_Update();

/*Backlight*/
void Backlight_Init();
uint16_t Backlight_GetValue();
void Backlight_SetValue(int16_t val);
void Backlight_SetGradual(uint16_t target, uint16_t time = 500);
void Backlight_UpdateBKP(void);
uint16_t Backlight_GetBKP(void);

/*Buttons*/
#include "ButtonEvent/ButtonEvent.h"
extern ButtonEvent btUP;
extern ButtonEvent btOK;
extern ButtonEvent btDOWN;
void Button_Init();
void Button_Update();

/*Power*/
void Power_Init();
void Power_Shutdown();
void Power_HandleTimeUpdate();
void Power_SetAutoLowPowerTimeout(uint16_t sec);
uint16_t Power_GetAutoLowPowerTimeout();
void Power_SetAutoLowPowerEnable(bool en);
void Power_AutoShutdownUpdate();

/*LED*/
void LED_Init();
void LED_SetEnable(bool en);

#endif
