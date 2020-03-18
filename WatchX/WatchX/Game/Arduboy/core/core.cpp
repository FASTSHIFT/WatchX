#include "core.h"
#include "Game/GamePrivate.h"

ArduboyCore::ArduboyCore() {}

void ArduboyCore::boot()
{
//  SPI.begin();
    bootPins();
    bootLCD();

#ifdef SAFE_MODE
    if (buttonsState() == (LEFT_BUTTON | UP_BUTTON))
        safeMode();
#endif

    saveMuchPower();
}

void ArduboyCore::bootPins()
{

}

void ArduboyCore::bootLCD()
{

}

void ArduboyCore::LCDDataMode()
{

}

void ArduboyCore::LCDCommandMode()
{

}



void ArduboyCore::safeMode()
{
    blank(); // too avoid random gibberish
    while (true)
    {

    }
}


/* Power Management */

void ArduboyCore::idle()
{

}

void ArduboyCore::saveMuchPower()
{

}

uint8_t ArduboyCore::width()
{
    return WIDTH;
}

uint8_t ArduboyCore::height()
{
    return HEIGHT;
}


/* Drawing */

void ArduboyCore::paint8Pixels(uint8_t pixels)
{

}

void ArduboyCore::paintScreen(const unsigned char *image)
{
    Game_DispReqUpdate();
}

// paint from a memory buffer, this should be FAST as it's likely what
// will be used by any buffer based subclass
void ArduboyCore::paintScreen(unsigned char image[])
{
    Game_DispReqUpdate();
}

void ArduboyCore::blank()
{

}

void ArduboyCore::sendLCDCommand(uint8_t command)
{

}

// invert the display or set to normal
// when inverted, a pixel set to 0 will be on
void ArduboyCore::invert(boolean inverse)
{

}

// turn all display pixels on, ignoring buffer contents
// or set to normal buffer display
void ArduboyCore::allPixelsOn(boolean on)
{

}

// flip the display vertically or set to normal
void ArduboyCore::flipVertical(boolean flipped)
{

}

// flip the display horizontally or set to normal
void ArduboyCore::flipHorizontal(boolean flipped)
{

}

/* RGB LED */

void ArduboyCore::setRGBled(uint8_t red, uint8_t green, uint8_t blue)
{
    Game_SetLEDBright(GAME_LED_RED, red);
    Game_SetLEDBright(GAME_LED_GREEN, green);
    Game_SetLEDBright(GAME_LED_BLUE, blue);
}

/* Buttons */

uint8_t ArduboyCore::getInput()
{
    return buttonsState();
}


uint8_t ArduboyCore::buttonsState()
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
