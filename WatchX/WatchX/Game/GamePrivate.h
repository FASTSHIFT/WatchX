#ifndef __GAMEPRIVATE_H
#define __GAMEPRIVATE_H

#include <stdint.h>
#include "WString.h"
#include "Basic/CommonMacro.h"
#include "PageManager/PageManager.h"       

extern PageManager GamePage;

#define GAME_FILE_PATH "/Game/"
#define GAME_SETUP_ONCE()\
do{\
    static bool isSetup = false;\
    if(isSetup)return;\
    isSetup=true;\
}while(0)

typedef enum {
    GAME_BUTTON_A,
    GAME_BUTTON_B,
    GAME_BUTTON_UP,
    GAME_BUTTON_DOWN,
    GAME_BUTTON_LEFT,
    GAME_BUTTON_RIGHT,
    GAME_BUTTON_MAX
}GameButton_Type;

typedef enum {
    GAME_LED_RED,
    GAME_LED_BLUE,
    GAME_LED_GREEN,
    GAME_LED_MAX
}GameLED_Type;

enum {
    GAME_NONE,
    GAME_chrbocchi,
    GAME_MAX
};

void Game_Begin();
String Game_GetNameList();
const char* Game_GetName(uint8_t id);
int16_t Game_GetNameID(const char* name);
bool Game_GetButtonPressed(uint8_t btn_id);
void Game_SetButtonState(uint8_t btn_id, bool state);
void Game_ClearButtonState();

void Game_SetLEDState(uint8_t led_id, bool val);
void Game_SetLEDBright(uint8_t led_id, uint8_t val);

void Game_DispGetContPos(int16_t* x, int16_t* y);
void Game_DispSetBasePoint(int16_t x, int16_t y);
bool Game_DispIsBusy();
bool Game_DispReqUpdate(bool clear = false);
void Game_DispTaskUpdate();
uint8_t* Game_DispGetBuffer();

#endif
