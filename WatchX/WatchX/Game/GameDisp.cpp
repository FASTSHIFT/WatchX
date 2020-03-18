#include "GamePrivate.h"
#include "GameDisp.h"
#include "GUI/DisplayPrivate.h"

static volatile bool GameDispUpdating = false;

static const uint8_t HEIGHT = 64, WIDTH = 128;

static uint8_t GameDispBuff[HEIGHT*WIDTH/8];
static int16_t GameDispBaseX = 0;
static int16_t GameDispBaseY = 0;
static int16_t GameDispClear = false;

void Game_DispSetBasePoint(int16_t x, int16_t y)
{
    GameDispBaseX = x;
    GameDispBaseY = y;
}

bool Game_DispIsBusy()
{
    return GameDispUpdating;
}

bool Game_DispReqUpdate(bool clear)
{
    if(GameDispUpdating)
    {
        return false;
    }
    
    GameDispClear = clear;
    GameDispUpdating = true;
    return true;
}

void Game_DispTaskUpdate()
{
    if(!GameDispUpdating)
        return;
    
    DISP_CS_CLR;
    int16_t x1 = GameDispBaseX + WIDTH - 1;
    int16_t y1 = GameDispBaseY + HEIGHT - 1;
    DISP_SET_WINDOW(GameDispBaseX, GameDispBaseY, x1, y1);
    DISP_RS_SET;
    for(int16_t y = 0; y < HEIGHT; y++)
    {
        for(int16_t x = 0; x < WIDTH; x++)
        {
            uint8_t row = y / 8;
            uint8_t bit_position = y % 8;
#define IsWhite ((GameDispBuff[(row*WIDTH) + x] & _BV(bit_position)) >> bit_position)
            uint16_t color = IsWhite ? screen.White : screen.Black;

            DISP_SEND(color);
            //screen.fillRect(BaseX + x * 2, BaseY + y * 2, 2, 2, IsWhite ? screen.White : screen.Black);
        }
    }
    DISP_CS_SET;
    
    if(GameDispClear)
    {
        memset(GameDispBuff, 0, sizeof(GameDispBuff));
    }

    GameDispUpdating = false;
}

uint8_t* Game_DispGetBuffer()
{
    return GameDispBuff;
}
