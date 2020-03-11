#ifndef __FILEGROUP_H
#define __FILEGROUP_H

/** 
  *@Illustrate: A Cross-platform migration Project | Arduino for STM32F4xx 
  *@Author: FASTSHIFT
  *@Website: https://github.com/FASTSHIFT/Arduino-For-Keil
  */

//************* Arduino Library **************//
#include "Arduino.h"
#include "HardwareSerial.h"
#include "Stream.h"
#include "SPI.h"
#include "Tone.h"
#include "WString.h"
#include "Wire.h"
#include "WMath.h"

#include "SysConfig.h"
#include "CommonMacro.h"
#include "BSP/BSP.h"

void Display_Init();

void Task_Display();
void Task_BMP_Update();
void Task_ButtonMonitor();

#endif
