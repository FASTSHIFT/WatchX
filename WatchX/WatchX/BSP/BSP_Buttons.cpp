#include "Basic/FileGroup.h"
#include "BSP.h"
#include "GUI/DisplayPrivate.h"

ButtonEvent btUP;
ButtonEvent btOK;
ButtonEvent btDOWN;

static void ButtonEvent_Handler(ButtonEvent* btn, int event)
{
    Power_HandleTimeUpdate();
    page.PageEventTransmit(event, btn);
}

static void Buttons_Init()
{
    pinMode(KEY_UP_Pin, INPUT_PULLUP);
    pinMode(KEY_OK_Pin, INPUT_PULLUP);
    pinMode(KEY_DOWN_Pin, INPUT_PULLUP);
    
    btUP.EventAttach(ButtonEvent_Handler);
    btOK.EventAttach(ButtonEvent_Handler);
    btDOWN.EventAttach(ButtonEvent_Handler);
}

void Task_ButtonMonitor()
{
    __ExecuteOnce(Buttons_Init());
    btUP.EventMonitor(!digitalRead(KEY_UP_Pin));
    btOK.EventMonitor(!digitalRead(KEY_OK_Pin));
    btDOWN.EventMonitor(!digitalRead(KEY_DOWN_Pin));
}
