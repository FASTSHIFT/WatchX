#include "BSP.h"
#include "Wire.h"

/*实例化软件I2C对象，快速模式*/
TwoWire WireBMP(BMP_SCL_Pin, BMP_SDA_Pin, SOFT_FAST);

/*实例化气压计对象*/
Adafruit_BMP085 BMP180;

/**
  * @brief  气压计初始化
  * @param  无
  * @retval 无
  */
void BMP_Init()
{
    while(!BMP180.begin());
}

/**
  * @brief  气压计状态更新
  * @param  无
  * @retval 无
  */
void BMP_Update()
{
    BMP180.updateAll();
}
