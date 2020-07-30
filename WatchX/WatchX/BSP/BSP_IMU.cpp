#include "BSP.h"
#include "Wire.h"

/*实例化软件I2C对象，快速模式*/
TwoWire Wire(IMU_SCL_Pin, IMU_SDA_Pin, SOFT_FAST);
