#include "common.h"

#if ARDUBOY_LIB_VER != 10101
#error
#endif // It may work even if you use other version.

/*  Defines  */

enum MODE {
    LOGO_MODE = 0,
    TITLE_MODE,
    GAME_MODE
};

/*  Typedefs  */

typedef struct {
    void(*initFunc)(void);
    bool(*updateFunc)(void);
    void(*drawFunc)(void);
} MODULE_FUNCS;

/*  Global Variables  */

MyArduboy arduboy;

/*  Local Variables  */

static const MODULE_FUNCS moduleTable[] = {
    { initLogo,  updateLogo,  drawLogo },
    { initTitle, updateTitle, drawTitle },
    { initGame,  updateGame,  drawGame },
};

static MODE mode = LOGO_MODE;

/*  For Debugging  */

#ifdef DEBUG
bool    dbgPrintEnabled = true;
char    dbgRecvChar = '\0';
uint8_t dbgCaptureMode = 0;

static void dbgCheckSerialRecv(void) {
    int recv;
    while ((recv = Serial.read()) != -1) {
        switch (recv) {
        case 'd':
            dbgPrintEnabled = !dbgPrintEnabled;
            Serial.print("Debug output ");
            Serial.println(dbgPrintEnabled ? "ON" : "OFF");
            break;
        case 'c':
            dbgCaptureMode = 1;
            break;
        case 'C':
            dbgCaptureMode = 2;
            break;
        }
        if (recv >= ' ' && recv <= '~') dbgRecvChar = recv;
    }
}

static void dbgScreenCapture() {
    if (dbgCaptureMode) {
        Serial.write((const uint8_t *)arduboy.getBuffer(), WIDTH * HEIGHT / 8);
        if (dbgCaptureMode == 1) dbgCaptureMode = 0;
    }
}
#endif

/*---------------------------------------------------------------------------*/

static void setup()
{
#ifdef DEBUG
    Serial.begin(115200);
#endif
    arduboy.beginNoLogo();
    arduboy.setFrameRate(60);
    moduleTable[LOGO_MODE].initFunc();
}

static void loop()
{
#ifdef DEBUG
    dbgCheckSerialRecv();
#endif
    if (!(arduboy.nextFrame())) return;
    bool isDone = moduleTable[mode].updateFunc();
    moduleTable[mode].drawFunc();
#ifdef DEBUG
    dbgScreenCapture();
    dbgRecvChar = '\0';
#endif
    arduboy.display();
    if (isDone) {
        mode = (mode == TITLE_MODE) ? GAME_MODE : TITLE_MODE;
        moduleTable[mode].initFunc();
        dprint("mode=");
        dprintln(mode);
    }
}

static void exit()
{
    
}

#include "Game/GamePrivate.h"

void GameRegister_chrbocchi(uint8_t gameID)
{
    GamePage.PageRegister(gameID, setup, loop, exit, NULL);
}
