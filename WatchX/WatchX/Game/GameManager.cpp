#include "GamePrivate.h"

PageManager GamePage(GAME_MAX);

static bool ButtonState_Grp[GAME_BUTTON_MAX];

bool Game_GetButtonPressed(uint8_t btn_id)
{
    return (btn_id >= GAME_BUTTON_MAX) ? false : ButtonState_Grp[btn_id];
}

void Game_SetButtonState(uint8_t btn_id, bool state)
{
    if(btn_id < GAME_BUTTON_MAX)
    {
        ButtonState_Grp[btn_id] = state;
    }
}

void Game_ClearButtonState()
{
    memset(ButtonState_Grp, 0, sizeof(ButtonState_Grp));
}

static const char* GameNameList[GAME_MAX];
    
String Game_GetNameList()
{
    String list = "";
    for(int i = 0; i < GAME_MAX; i++)
    {
        if(GameNameList[i])
        {
            if(list != "")
            {
                list += "\n";
            }
            list += (String(GameNameList[i]));
        }
    }
    return list;
}

int16_t Game_GetNameID(const char* name)
{
    for(int16_t i = 0; i < GAME_MAX; i++)
    {
        if(GameNameList[i])
        {
            if(strcmp(name, GameNameList[i]) == 0)
            {
                return i;
            }
        }
    }
    return -1;
}

const char* Game_GetName(uint8_t id)
{
    return (id < GAME_MAX) ? GameNameList[id] : NULL;
}

static void Game_AddName(uint8_t id, const char* name)
{
    GameNameList[id] = name;
}

#define GAME_REG(name)\
do{\
    extern void GameRegister_##name(uint8_t gameID);\
    GameRegister_##name(GAME_##name);\
    Game_AddName(GAME_##name,#name);\
}while(0)

void Game_Begin()
{
    GAME_REG(chrbocchi);
    GamePage.PageChangeTo(GAME_chrbocchi);
}
