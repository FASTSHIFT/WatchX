#include "ButtonEvent.h"

using namespace ButtonEvent_Type;

#ifdef ARDUINO
#include "Arduino.h"
#define ButtonEvent_Millis millis()
#else
#error "Please set the system clock used by ButtonEvent"
#endif

/**
  * @brief  按键事件构造函数
  * @param  NoPressState: 按键未按下时的状态
  * @param  LongPressTimeMs_Set: 按键长按触发超时设置
  * @param  LongPressTimeRepeatMs_Set: 长按重复触发时间
  * @retval 无
  */
ButtonEvent::ButtonEvent(
        bool NoPressState, 
        uint16_t LongPressTimeMs_Set, 
        uint16_t LongPressTimeRepeatMs_Set
)
{
    Button_NoPressState = NoPressState;
    LongPressTimeMs = LongPressTimeMs_Set;
    LongPressTimeRepeatMs = LongPressTimeRepeatMs_Set;
    
    LastLongPress_TimePoint = LastClick_TimePoint = LastPress_TimePoint = 0;
    IS_LongPressed = IS_Pressed = false;

    CallbackFunc = 0;
}

/**
  * @brief  按键事件绑定
  * @param  Function: 回调函数指针
  * @retval 无
  */
void ButtonEvent::EventAttach(ButtonEvent_FuncCallBack_t function)
{
    CallbackFunc = function;
}

/**
  * @brief  监控事件，建议扫描周期10ms
  * @param  NowState: 当前按键引脚状态
  * @retval 无
  */
void ButtonEvent::EventMonitor(uint8_t NowState)
{
    if(!CallbackFunc)
        return;
    
    if (!IS_Pressed && (NowState != Button_NoPressState))
    {
        IsPressed = IS_Pressed = true;
        Button_NowState = Press;
        LastPress_TimePoint = ButtonEvent_Millis;

        CallbackFunc(this, EVENT_ButtonPress);
        CallbackFunc(this, EVENT_ButtonChange);
    }
    else if (IS_Pressed && ButtonEvent_Millis - LastPress_TimePoint >= LongPressTimeMs && (NowState != Button_NoPressState))
    {
        Button_NowState = LongPress;
        if(!IS_LongPressed)
        {
            CallbackFunc(this, EVENT_ButtonLongPressed);
            LastLongPress_TimePoint = ButtonEvent_Millis;
            IsLongPressed = IS_LongPressed = true;
        }
        else if(ButtonEvent_Millis - LastLongPress_TimePoint >= LongPressTimeRepeatMs)
        {
            LastLongPress_TimePoint = ButtonEvent_Millis;
            CallbackFunc(this, EVENT_ButtonLongPressRepeat);
        }
    }
    else if (IS_Pressed && (NowState == Button_NoPressState))
    {
        IS_LongPressed = IS_Pressed = false;
        IsClicked = true;
        Button_NowState = NoPress;
        LastClick_TimePoint = ButtonEvent_Millis;

        if(ButtonEvent_Millis - LastPress_TimePoint < LongPressTimeMs)
        {
            CallbackFunc(this, EVENT_ButtonClick);
        }
        CallbackFunc(this, EVENT_ButtonRelease);
        CallbackFunc(this, EVENT_ButtonChange);
    }

    if(IS_Pressed && ButtonEvent_Millis - LastClick_TimePoint < 200)
    {
        Click_Cnt++;
        CallbackFunc(this, EVENT_ButtonDoubleClick);
    }
}

