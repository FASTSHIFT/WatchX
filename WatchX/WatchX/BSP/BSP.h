#ifndef __BSP_H
#define __BSP_H

#include "Basic/SysConfig.h"
#include "Arduino.h"

/*BMP*/
#include "Adafruit_BMP180/Adafruit_BMP085.h"
extern Adafruit_BMP085 BMP180;

/*Brightness*/
uint16_t Brightness_GetValue();
void Brightness_SetValue(int16_t val);
void Brightness_SetGradual(uint16_t target, uint16_t time = 500);

/*Buttons*/
#include "ButtonEvent/ButtonEvent.h"
extern ButtonEvent btUP;
extern ButtonEvent btOK;
extern ButtonEvent btDOWN;

#endif
