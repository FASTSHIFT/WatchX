#include "softserial.h"

#define SoftSerial_MaxFrameLength       16

void SoftSerial_Write(Pin_TypeDef pin, uint32_t frameDuration, uint8_t data)
{
    uint8_t buffPos = 0;
    static uint8_t buff[SoftSerial_MaxFrameLength];
    static uint32_t timepoints[SoftSerial_MaxFrameLength];
    buff[buffPos++] = 0; /* start bit */
    for(int i = 0; i < 8; i++)
    {
        buff[buffPos++] = (data >> i) & 0x01;
    }
    buff[buffPos++] = 1; /* end bit */
    for(int i = 0; i < buffPos; i++)
    {
        timepoints[i] = frameDuration * i / buffPos;
    }
    timepoints[buffPos] = 0x1fffffff;
    uint8_t crtBit = 0;
    for(int i = 0; i < frameDuration; i++)
    {
        if(i >= timepoints[crtBit])
        {
            digitalWrite(pin, buff[crtBit]);
            crtBit ++;
        }
    }
}

