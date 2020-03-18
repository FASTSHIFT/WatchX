#ifndef MYARDUBOY_H
#define MYARDUBOY_H

#include "Game/Arduboy/Arduboy.h"
#include "WMath.h"

class MyArduboy : public Arduboy
{
public:
    bool    nextFrame(void);
    bool    buttonDown(uint8_t buttons);
    bool    buttonPressed(uint8_t buttons);
    bool    buttonUp(uint8_t buttons);
    void    setTextColor(uint8_t color);
    void    setTextColor(uint8_t color, uint8_t bg);
    size_t  printEx(int16_t x, int16_t y, const char *p);
    size_t  printEx(int16_t x, int16_t y, const __FlashStringHelper *p);
    virtual size_t write(uint8_t);
    using Print::write;
    void    drawRect2(int16_t x, int16_t y, uint8_t w, int8_t h, uint8_t color);
    void    drawFastVLine2(int16_t x, int16_t y, int8_t h, uint8_t color);
    void    drawFastHLine2(int16_t x, int16_t y, uint8_t w, uint8_t color);
    void    fillRect2(int16_t x, int16_t y, uint8_t w, int8_t h, uint8_t color);
    void    playScore2(const byte *score, uint8_t priority);
    void    tone2(unsigned int frequency, unsigned long duration);
    void    eepSeek(uint16_t pos);
    uint8_t eepRead8(void);
    uint16_t eepRead16(void);
    uint32_t eepRead32(void);
    void    eepWrite8(uint8_t val);
    void    eepWrite16(uint16_t val);
    void    eepWrite32(uint32_t val);

private:
    void    myDrawChar(int16_t x, int16_t y, unsigned char c, uint8_t color, uint8_t bg, uint8_t size);
    void    fillBeltBlack(unsigned char *p, unsigned char d, uint8_t w);
    void    fillBeltWhite(unsigned char *p, unsigned char d, uint8_t w);
    uint8_t textcolor = WHITE;
    uint8_t textbg = BLACK;
    uint8_t lastButtonState;
    uint8_t currentButtonState;
    uint8_t playScorePriority;
    uint8_t *eepAddr = (uint8_t *) EEPROM_STORAGE_SPACE_START;
};

#endif // MYARDUBOY_H
