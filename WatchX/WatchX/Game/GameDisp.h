#ifndef __GAMEDISP_H
#define __GAMEDISP_H

#include "Basic/SysConfig.h"

#define DISP_CS_SET digitalWrite_HIGH(TFT_CS_Pin)
#define DISP_CS_CLR digitalWrite_LOW(TFT_CS_Pin)

#define DISP_RS_SET digitalWrite_HIGH(TFT_DC_Pin)
#define DISP_RS_CLR digitalWrite_LOW(TFT_DC_Pin)

#define DISP_CMD_SET_X     0x2A
#define DISP_CMD_SET_Y     0x2B
#define DISP_CMD_WRITE_RAM 0x2C
#define DISP_CMD_READ_RAM  0x2D

#define __SPIx SPI1
#define __SPI_I2S_GET_FLAG(SPI_I2S_FLAG) (__SPIx->SR & SPI_I2S_FLAG)
#define __SPI_I2S_TXDATA(data)           (__SPIx->DR = data)

#define DISP_SEND8(n)\
do{\
    while (__SPI_I2S_GET_FLAG(SPI_SR_TXE) == 0);\
    __SPI_I2S_TXDATA((n));\
}while(0)

#define DISP_SEND(n)\
do{\
    while (__SPI_I2S_GET_FLAG(SPI_SR_TXE) == 0);\
    __SPI_I2S_TXDATA((n) >> 8);\
    while (__SPI_I2S_GET_FLAG(SPI_SR_TXE) == 0);\
    __SPI_I2S_TXDATA((n));\
}while(0)

#define DISP_SEND_WAIT()\
do{\
    while (!__SPI_I2S_GET_FLAG(SPI_SR_TXE));\
    while (__SPI_I2S_GET_FLAG(SPI_SR_BSY));\
}while(0)

#define DISP_SET_WINDOW(x0,y0,x1,y1) \
do{\
    DISP_RS_CLR;\
    DISP_SEND8(DISP_CMD_SET_X);\
    DISP_SEND_WAIT();\
    \
    DISP_RS_SET;\
    DISP_SEND(x0 + 52);\
    DISP_SEND(x1 + 52);\
    DISP_SEND_WAIT();\
    \
    DISP_RS_CLR;\
    DISP_SEND8(DISP_CMD_SET_Y);\
    DISP_SEND_WAIT();\
    \
    DISP_RS_SET;\
    DISP_SEND(y0 + 40);\
    DISP_SEND(y1 + 40);\
    DISP_SEND_WAIT();\
    \
    DISP_RS_CLR;\
    DISP_SEND8(DISP_CMD_WRITE_RAM);\
    DISP_SEND_WAIT();\
}while(0)

#endif
