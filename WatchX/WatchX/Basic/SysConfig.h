#ifndef __SYSCONFIG_H
#define __SYSCONFIG_H

#define WX_NAME             "WatchX"
#define WX_HARDWARE_VERSION "v2.0"
#define WX_SOFTWARE_VERSION "v0.7"

#define TFT_CS_Pin  PA4
#define TFT_DC_Pin  PA8
#define TFT_RST_Pin PA1 
#define TFT_LED_Pin PA10

#define LED_Pin     PC13

#define KEY_UP_Pin   PB0
#define KEY_OK_Pin   PB1
#define KEY_DOWN_Pin PB2

#define BAT_DET_Pin PA6
#define BAT_CHG_Pin PA15

#define IMU_SCL_Pin PB6
#define IMU_SDA_Pin PB7
#define IMU_INT_Pin PB5

#define BMP_SCL_Pin PB8
#define BMP_SDA_Pin PB9

#define STOPWATCH_TIM TIM2

#endif
