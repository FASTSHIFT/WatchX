#ifndef __SYSCONFIG_H
#define __SYSCONFIG_H

#define WX_NAME             "WatchX"     //手表名称
#define WX_HARDWARE_VERSION "v2.0"       //硬件版本
#define WX_SOFTWARE_VERSION "v1.0"       //软件版本

/*屏幕*/
#define TFT_CS_Pin  PA4     //片选
#define TFT_DC_Pin  PA8     //数据/命令选择
#define TFT_RST_Pin PA1     //复位
#define TFT_BLK_Pin PA10    //背光

/*指示灯*/
#define LED_Pin     PC13

/*电源控制(ONLY Watch Max)*/
#define POWER_ON_Pin PA3    //电源保持
#define POWER_EN_Pin PA2    //电源使能

/*按键*/
#define KEY_UP_Pin   PB0    //上键
#define KEY_OK_Pin   PB1    //选择键
#define KEY_DOWN_Pin PB2    //下键

/*电池检测*/
#define BAT_DET_Pin PA6     //电压检测
#define BAT_CHG_Pin PA15    //充电检测

/*运动传感器*/
#define IMU_SCL_Pin PB6     //I2C时钟
#define IMU_SDA_Pin PB7     //I2C数据
#define IMU_INT_Pin PB5     //中断脚

/*气压计*/
#define BMP_SCL_Pin PB8     //I2C时钟
#define BMP_SDA_Pin PB9     //I2C数据

/*秒表功能使用的定时器*/
#define STOPWATCH_TIM TIM2

#endif
