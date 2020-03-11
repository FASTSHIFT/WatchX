#ifndef __ADAFRUIT_ST7789_H
#define __ADAFRUIT_ST7789_H

#include "Adafruit_GFX_Library/Adafruit_GFX.h"

#define ST7789_TFTWIDTH  135
#define ST7789_TFTHEIGHT 240

#if defined(__STM32__)
typedef volatile uint32_t PortReg_t;
#define USE_FAST_IO
#elif defined(__AVR__)
typedef volatile uint8_t PortReg_t;
#define USE_FAST_IO
#endif

class Adafruit_ST7789 : public Adafruit_GFX
{
public:
    Adafruit_ST7789(uint8_t cs, uint8_t rs, uint8_t rst);
    Adafruit_ST7789(uint8_t cs, uint8_t rs, uint8_t rst, uint8_t clk, uint8_t mosi);

    typedef enum
    {
        White = 0xFFFF,
        Black = 0x0000,
        Blue = 0x001F,
        Blue2 = 0x051F,
        Red = 0xF800,
        Magenta = 0xF81F,
        Green = 0x07E0,
        Cyan = 0x7FFF,
        Yellow = 0xFFE0
    } Color_Type;

    void begin();
    void writeCommond(uint8_t cmd);
    void writeData16(uint16_t data);
    void writeData(uint8_t data);
    void spiWrite(uint8_t data);

    virtual void drawPixel(int16_t x, int16_t y, uint16_t color);
    virtual void setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
    virtual void setCursor(int16_t x, int16_t y);
    virtual void setRotation(uint8_t r);
    virtual void fillScreen(uint16_t color);
    virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    virtual void drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);
    virtual void drawFastRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);
    virtual void inline pushColor(uint16_t color)
    {
        writeData16(color);
    }

    void setOpacityX(uint8_t opacity);

private:
    uint8_t RST, CS, RS, SCK, MOSI;
    uint8_t colstart, rowstart, xstart, ystart;
    bool hwSPI;

#if defined(USE_FAST_IO)
#if defined(__STM32__)
    GPIO_TypeDef *rstport, *csport, *rsport, *sckport, *mosiport;
    uint16_t  rstpinmask, cspinmask, rspinmask, sckpinmask, mosipinmask;
#endif
#endif

};

#endif
