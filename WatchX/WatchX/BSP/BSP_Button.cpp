#include "Basic/FileGroup.h"
#include "BSP.h"
#include "GUI/DisplayPrivate.h"

ButtonEvent btUP;
ButtonEvent btOK;
ButtonEvent btDOWN;

static ButtonEvent btPOWER;

static void Button_EventHandler(ButtonEvent* btn, int event)
{
    Power_HandleTimeUpdate();
    page.PageEventTransmit(event, btn);
}

static void ButtonPower_EventHandler(ButtonEvent* btn, int event)
{
    if(btn == &btPOWER)
    {
        if(event == ButtonEvent::EVENT_ButtonLongPressed)
        {
            Power_Shutdown();
        }
    }
}

void Button_Init()
{
    pinMode(KEY_UP_Pin, INPUT_PULLUP);
    pinMode(KEY_OK_Pin, INPUT_PULLUP);
    pinMode(KEY_DOWN_Pin, INPUT_PULLUP);
    
    btUP.EventAttach(Button_EventHandler);
    btOK.EventAttach(Button_EventHandler);
    btDOWN.EventAttach(Button_EventHandler);
    btPOWER.EventAttach(ButtonPower_EventHandler);
}

void Button_Update()
{
    btUP.EventMonitor(!digitalRead(KEY_UP_Pin));
    btOK.EventMonitor(!digitalRead(KEY_OK_Pin));
    btDOWN.EventMonitor(!digitalRead(KEY_DOWN_Pin));
    btPOWER.EventMonitor(digitalRead(POWER_EN_Pin));
}
