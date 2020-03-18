/**
 * @file Arduboy2Core.cpp
 * \brief
 * The Arduboy2Core class for Arduboy hardware initilization and control.
 */

#include "Arduboy2Core.h"
#include "APP/Game/GamePrivate.h"
#include "Basic/TasksManage.h"

//#include <avr/wdt.h>

Arduboy2Core::Arduboy2Core() { }

void Arduboy2Core::boot()
{
    bootPins();
    bootSPI();
    bootOLED();
    bootPowerSaving();
}

// Pins are set to the proper modes and levels for the specific hardware.
// This routine must be modified if any pins are moved to a different port
void Arduboy2Core::bootPins()
{
}

void Arduboy2Core::bootOLED()
{
}

void Arduboy2Core::LCDDataMode()
{
}

void Arduboy2Core::LCDCommandMode()
{
}

// Initialize the SPI interface for the display
void Arduboy2Core::bootSPI()
{
}

// Write to the SPI bus (MOSI pin)
void Arduboy2Core::SPItransfer(uint8_t data)
{
}

void Arduboy2Core::safeMode()
{
    if (buttonsState() == UP_BUTTON)
    {
        while (true) { }
    }
}


/* Power Management */

void Arduboy2Core::idle()
{
}

void Arduboy2Core::bootPowerSaving()
{
}

// Shut down the display
void Arduboy2Core::displayOff()
{
}

// Restart the display after a displayOff()
void Arduboy2Core::displayOn()
{
    bootOLED();
}

uint8_t Arduboy2Core::width()
{
    return WIDTH;
}

uint8_t Arduboy2Core::height()
{
    return HEIGHT;
}


/* Drawing */

void Arduboy2Core::paint8Pixels(uint8_t pixels)
{

}

void Arduboy2Core::paintScreen(const uint8_t *image)
{
    Game_DispReqUpdate();
}

// For reference, this is the "closed loop" C++ version of paintScreen()
// used prior to the above version.
void Arduboy2Core::paintScreen(uint8_t image[], bool clear)
{
    Game_DispReqUpdate(clear);
}

void Arduboy2Core::blank()
{

}

void Arduboy2Core::sendLCDCommand(uint8_t command)
{
}

// invert the display or set to normal
// when inverted, a pixel set to 0 will be on
void Arduboy2Core::invert(bool inverse)
{
}

// turn all display pixels on, ignoring buffer contents
// or set to normal buffer display
void Arduboy2Core::allPixelsOn(bool on)
{
}

// flip the display vertically or set to normal
void Arduboy2Core::flipVertical(bool flipped)
{
}

// flip the display horizontally or set to normal
void Arduboy2Core::flipHorizontal(bool flipped)
{
}

/* RGB LED */

void Arduboy2Core::setRGBled(uint8_t red, uint8_t green, uint8_t blue)
{
    Game_SetLEDBright(GAME_LED_RED, 0);
    Game_SetLEDBright(GAME_LED_GREEN, 0);
    Game_SetLEDBright(GAME_LED_BLUE, 0);
}

void Arduboy2Core::setRGBled(uint8_t color, uint8_t val)
{
    if (color == RED_LED)
    {
        Game_SetLEDBright(GAME_LED_RED, val);
    }
    else if (color == GREEN_LED)
    {
        Game_SetLEDBright(GAME_LED_GREEN, val);
    }
    else if (color == BLUE_LED)
    {
        Game_SetLEDBright(GAME_LED_BLUE, val);
    }
}

void Arduboy2Core::freeRGBled()
{
    Game_SetLEDState(GAME_LED_RED, 0);
    Game_SetLEDState(GAME_LED_GREEN, 0);
    Game_SetLEDState(GAME_LED_BLUE, 0);
}

void Arduboy2Core::digitalWriteRGB(uint8_t red, uint8_t green, uint8_t blue)
{
    Game_SetLEDState(GAME_LED_RED, !red);
    Game_SetLEDState(GAME_LED_GREEN, !green);
    Game_SetLEDState(GAME_LED_BLUE, !blue);
}

void Arduboy2Core::digitalWriteRGB(uint8_t color, uint8_t val)
{
    val = val == RGB_ON ? true : false;
    if (color == RED_LED)
    {
        Game_SetLEDState(GAME_LED_RED, val);
    }
    else if (color == GREEN_LED)
    {
        Game_SetLEDState(GAME_LED_GREEN, val);
    }
    else if (color == BLUE_LED)
    {
        Game_SetLEDState(GAME_LED_BLUE, val);
    }
}

/* Buttons */

uint8_t Arduboy2Core::buttonsState()
{
    uint8_t buttons = 0;

    if(Game_GetButtonPressed(GAME_BUTTON_A))
    {
        buttons |= A_BUTTON;
    }
    if(Game_GetButtonPressed(GAME_BUTTON_B))
    {
        buttons |= B_BUTTON;
    }
    if(Game_GetButtonPressed(GAME_BUTTON_UP))
    {
        buttons |= UP_BUTTON;
    }
    if(Game_GetButtonPressed(GAME_BUTTON_DOWN))
    {
        buttons |= DOWN_BUTTON;
    }
    if(Game_GetButtonPressed(GAME_BUTTON_LEFT))
    {
        buttons |= LEFT_BUTTON;
    }
    if(Game_GetButtonPressed(GAME_BUTTON_RIGHT))
    {
        buttons |= RIGHT_BUTTON;
    }

    return buttons;
}

// delay in ms with 16 bit duration
void Arduboy2Core::delayShort(uint16_t ms)
{
    vTaskDelay((unsigned long) ms);
}

void Arduboy2Core::exitToBootloader()
{
    cli();
    while (true) { }
}

// Replacement main() that eliminates the USB stack code.
// Used by the ARDUBOY_NO_USB macro. This should not be called
// directly from a sketch.

void Arduboy2Core::mainNoUSB()
{
}

