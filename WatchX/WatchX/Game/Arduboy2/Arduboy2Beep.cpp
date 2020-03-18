/**
 * @file Arduboy2Beep.cpp
 * \brief
 * Classes to generate simple square wave tones on the Arduboy speaker pins.
 */

#include <Arduino.h>
#include "Arduboy2Beep.h"

#ifndef AB_DEVKIT

// Speaker pin 1, Timer 3A, Port C bit 6, Arduino pin 5

uint8_t BeepPin1::duration = 0;

#ifndef __AVR__
static uint8_t TCCR3A, TCCR3B;
static uint8_t OCR3A, OCR4A, OCR4C;
static uint8_t TCCR4A, TCCR4B, TCCR4D;
static uint8_t WGM32;
static uint8_t CS31;
static uint8_t TC4H;
#define bit(n) 0
#define AVR_F_CPU 16000000UL
#define COUNT_TO_FREQ(count) ((AVR_F_CPU / 128.0f) / (2.0f * (count) + 1))
#include "BSP/BSP.h"
#endif

void BeepPin1::begin()
{
    TCCR3A = 0;
    TCCR3B = (bit(WGM32) | bit(CS31)); // CTC mode. Divide by 8 clock prescale
}

void BeepPin1::tone(uint16_t count)
{
    //tone(count, 0);
    Audio_Tone(COUNT_TO_FREQ(count));
}

void BeepPin1::tone(uint16_t count, uint8_t dur)
{
    duration = dur;
    Audio_Tone(COUNT_TO_FREQ(count));
    TCCR3A = bit(COM3A0); // set toggle on compare mode (which connects the pin)
    OCR3A = count; // load the count (16 bits), which determines the frequency
}

void BeepPin1::timer()
{
    if (duration && (--duration == 0))
    {
        noTone();
        TCCR3A = 0; // set normal mode (which disconnects the pin)
    }
}

void BeepPin1::noTone()
{
    Audio_NoTone();
    duration = 0;
    TCCR3A = 0; // set normal mode (which disconnects the pin)
}


// Speaker pin 2, Timer 4A, Port C bit 7, Arduino pin 13

uint8_t BeepPin2::duration = 0;

void BeepPin2::begin()
{
    TCCR4A = 0; // normal mode. Disable PWM
    TCCR4B = bit(CS43); // divide by 128 clock prescale
    TCCR4D = 0; // normal mode
    TC4H = 0;  // toggle pin at count = 0
    OCR4A = 0; //  "
}

void BeepPin2::tone(uint16_t count)
{
    tone(count, 0);
}

void BeepPin2::tone(uint16_t count, uint8_t dur)
{
    duration = dur;
    TCCR4A = bit(COM4A0); // set toggle on compare mode (which connects the pin)
    TC4H = highByte(count); // load the count (10 bits),
    OCR4C = lowByte(count); //  which determines the frequency
}

void BeepPin2::timer()
{
    if (duration && (--duration == 0))
    {
        TCCR4A = 0; // set normal mode (which disconnects the pin)
    }
}

void BeepPin2::noTone()
{
    duration = 0;
    TCCR4A = 0; // set normal mode (which disconnects the pin)
}


#else /* AB_DEVKIT */

// *** The pins used for the speaker on the DevKit cannot be directly
// controlled by a timer/counter. The following "dummy" functions will
// compile and operate properly but no sound will be produced

uint8_t BeepPin1::duration = 0;

void BeepPin1::begin()
{
}

void BeepPin1::tone(uint16_t count)
{
    tone(count, 0);
}

void BeepPin1::tone(uint16_t count, uint8_t dur)
{
    (void) count; // parameter not used

    duration = dur;
}

void BeepPin1::timer()
{
    if (duration)
    {
        --duration;
    }
}

void BeepPin1::noTone()
{
    duration = 0;
}


uint8_t BeepPin2::duration = 0;

void BeepPin2::begin()
{
}

void BeepPin2::tone(uint16_t count)
{
    tone(count, 0);
}

void BeepPin2::tone(uint16_t count, uint8_t dur)
{
    (void) count; // parameter not used

    duration = dur;
}

void BeepPin2::timer()
{
    if (duration)
    {
        --duration;
    }
}

void BeepPin2::noTone()
{
    duration = 0;
}

#endif
