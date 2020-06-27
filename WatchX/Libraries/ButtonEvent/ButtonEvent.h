/**
  ******************************************************************************
  * @file    ButtonEvent.h
  * @author  FASTSHIFT
  * @version V1.3.0
  * @date    2018-12-23
  * @brief   非阻塞式按键事件驱动库，支持短摁、长摁、释放、状态改变事件的识别。
  * @Upgrade 2019.6.18  添加按键双击事件、长按单次触发事件。
  * @Upgrade 2019.8.26  添加GetClicked、GetPressed、GetLongPressed非事件模式支持。
  * @Upgrade 2019.12.4  使用了新的事件回调设计，所有事件统一用EventAttach。
                        添加Click、LongPressed、LongPressRepeat事件。
  * @Upgrade 2020.6.12  合并ButtonEvent_Type至ButtonEvent类
  ******************************************************************************
  * @attention
  * 需要提供一个精确到毫秒级的系统时钟，用户需要在ButtonEvent.cpp里定义ButtonEvent_Millis
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BUTTONEVENT_H
#define __BUTTONEVENT_H

#include "stdint.h"

class ButtonEvent
{
    typedef void(*ButtonEvent_FuncCallBack_t)(ButtonEvent*, int);
public:
    ButtonEvent(
        bool NoPressState = 0,
        uint16_t LongPressTimeMs_Set = 500,
        uint16_t LongPressTimeRepeatMs_Set = 200
    );

    enum Event_Type
    {
        EVENT_ButtonNone,
        EVENT_ButtonPress,
        EVENT_ButtonLongPressed,
        EVENT_ButtonLongPressRepeat,
        EVENT_ButtonRelease,
        EVENT_ButtonChange,
        EVENT_ButtonClick,
        EVENT_ButtonDoubleClick,
    };

    bool IsPressed;
    bool IsClicked;
    bool IsLongPressed;

    void EventAttach(ButtonEvent_FuncCallBack_t function);
    void EventMonitor(uint8_t NowState);

    inline uint8_t GetClickCnt()
    {
        uint8_t cnt = Click_Cnt + 1;
        Click_Cnt = 0;
        return cnt;
    }
    inline bool GetClicked()
    {
        bool n = IsClicked;
        IsClicked = false;
        return n;
    }
    inline bool GetPressed()
    {
        bool n = IsPressed;
        IsPressed = false;
        return n;
    }
    inline bool GetLongPressed()
    {
        bool n = IsLongPressed;
        IsLongPressed = false;
        return n;
    }

    operator uint8_t()
    {
        return Button_NowState;
    };

private:
    enum ButtonState
    {
        NoPress,
        Press,
        LongPress
    };
    bool Button_NoPressState;
    uint8_t Button_NowState;
    uint16_t LongPressTimeMs;
    uint16_t LongPressTimeRepeatMs;
    uint32_t LastLongPress_TimePoint;
    uint32_t LastPress_TimePoint;
    uint32_t LastClick_TimePoint;
    uint8_t Click_Cnt;
    bool IS_Pressed;
    bool IS_LongPressed;

    ButtonEvent_FuncCallBack_t CallbackFunc;
};

#endif
