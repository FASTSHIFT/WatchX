#include "Basic/FileGroup.h"
#include "BSP.h"
#include "Wire.h"

TwoWire WireBMP(BMP_SCL_Pin, BMP_SDA_Pin, SOFT_FAST);
Adafruit_BMP085 BMP180;

static void BMP_Init()
{
    while(!BMP180.begin());
}

void Task_BMP_Update()
{
    __ExecuteOnce(BMP_Init());
    BMP180.updateAll();
}
