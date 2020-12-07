#include "BSP.h"
#include "GUI/DisplayPrivate.h"

/*实例化按键对象*/
ButtonEvent btUP;              //上键
ButtonEvent btOK;              //选择键
ButtonEvent btDOWN;            //下键
static ButtonEvent btPOWER;    //电源键，不共享对象

/**
  * @brief  按键事件回调处理
  * @param  btn:按键对象地址
  * @param  event:事件类型
  * @retval 无
  */
static void Button_EventHandler(ButtonEvent* btn, int event)
{
    /*自动关机时间更新*/
    Power_HandleTimeUpdate();
    
    /*传递至页面事件*/
    page.PageEventTransmit(btn, event);
}

/**
  * @brief  电源按键事件回调处理
  * @param  btn:按键对象地址
  * @param  event:事件类型
  * @retval 无
  */
static void ButtonPower_EventHandler(ButtonEvent* btn, int event)
{
    if(btn == &btPOWER)
    {
        /*长按关机*/
        if(event == ButtonEvent::EVENT_ButtonLongPressed)
        {
            Power_Shutdown();
        }
    }
}

/**
  * @brief  按键初始化
  * @param  无
  * @retval 无
  */
void Button_Init()
{
    /*上拉输入*/
    pinMode(KEY_UP_Pin, INPUT_PULLUP);
    pinMode(KEY_OK_Pin, INPUT_PULLUP);
    pinMode(KEY_DOWN_Pin, INPUT_PULLUP);
    
    /*关联事件*/
    btUP.EventAttach(Button_EventHandler);
    btOK.EventAttach(Button_EventHandler);
    btDOWN.EventAttach(Button_EventHandler);
    btPOWER.EventAttach(ButtonPower_EventHandler);
}

/**
  * @brief  按键监控更新
  * @param  无
  * @retval 无
  */
void Button_Update()
{
    btUP.EventMonitor(!digitalRead(KEY_UP_Pin));
    btOK.EventMonitor(!digitalRead(KEY_OK_Pin));
    btDOWN.EventMonitor(!digitalRead(KEY_DOWN_Pin));
    btPOWER.EventMonitor(digitalRead(POWER_EN_Pin));
}
