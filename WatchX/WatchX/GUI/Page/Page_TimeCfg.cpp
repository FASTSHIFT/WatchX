#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "Time/Time.h"

static RTC_TimeTypeDef RTC_Time;
static RTC_DateTypeDef RTC_Date;
static const char* WeekStr[7] = { "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};

static lv_obj_t * appWindow;

static lv_obj_t * labelTitle;
static lv_obj_t * lineTitle;

static lv_obj_t * contTime;
static lv_obj_t * labelTime;

static lv_obj_t * contDate;
static lv_obj_t * labelYear;
static lv_obj_t * labelDate;
static lv_obj_t * labelWeek;

static lv_obj_t * contTimeAnim;
static lv_obj_t * contDateAnim;

static int8_t CfgSelIndex = 0;
static bool CfgChanged = false;

static void TitleUpdate()
{
    const char* text_grp[] = 
    {
        "TimeCfg",
        "Hours",
        "Minutes",
        "Seconds", 
        "Year", 
        "Month", 
        "Date", 
        "WeekDay"
    };
    
    const char* text;
    
    if(CfgSelIndex == 0)
    {
        text = CfgChanged ? "Save?" : text_grp[CfgSelIndex];
    }
    else
    {
        text = text_grp[CfgSelIndex];
    }
    
    lv_label_set_static_text(labelTitle, text);
}

static void Creat_Title()
{
    LV_FONT_DECLARE(HandGotn_20);
    labelTitle = lv_label_create(appWindow, NULL);
    
    static lv_style_t style_label;
    style_label = *lv_label_get_style(labelTitle, LV_LABEL_STYLE_MAIN);
    style_label.text.font = &HandGotn_20;
    style_label.text.color = LV_COLOR_WHITE;
    lv_label_set_style(labelTitle, LV_LABEL_STYLE_MAIN, &style_label);
    
    lv_label_set_static_text(labelTitle, "TimeCfg");
    lv_obj_align(labelTitle, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_auto_realign(labelTitle, true);
    
    lineTitle = lv_line_create(appWindow, NULL);
    static lv_point_t line_points[] = { {0, 0}, {APP_WIN_WIDTH, 0}};
    lv_line_set_points(lineTitle, line_points, 2);
    static lv_style_t style_line;
    style_line = *lv_line_get_style(lineTitle, LV_LINE_STYLE_MAIN);
    style_line.line.color = LV_COLOR_RED;
    style_line.line.width = 2;
    style_line.line.rounded = 1;
    lv_line_set_style(lineTitle, LV_LINE_STYLE_MAIN, &style_line);
    lv_obj_align(lineTitle, labelTitle, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);
}

static void Creat_ContTimeDate()
{
    contTime = lv_cont_create(appWindow, NULL);
    lv_obj_set_size(contTime, 128, 42);
    lv_obj_align(contTime, lineTitle, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    
    contDate = lv_cont_create(appWindow, NULL);
    lv_obj_set_size(contDate, 128, 120);
    lv_obj_align(contDate, contTime, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    
    LV_FONT_DECLARE(HandGotn_14);
    static lv_style_t style_label;
    style_label = lv_style_plain_color;
    style_label.body.main_color = LV_COLOR_BLACK;
    style_label.body.grad_color = LV_COLOR_BLACK;
    style_label.text.font = &HandGotn_14;
    style_label.text.color = LV_COLOR_SILVER;
    
    lv_obj_t * label1 = lv_label_create(appWindow, NULL);
    lv_label_set_body_draw(label1, true);
    lv_label_set_static_text(label1, " TIME ");
    lv_obj_align(label1, contTime, LV_ALIGN_OUT_TOP_MID, 0, 10);
    lv_label_set_style(label1, LV_LABEL_STYLE_MAIN, &style_label);
    
    lv_obj_t * label2 = lv_label_create(appWindow, NULL);
    lv_label_set_body_draw(label2, true);
    lv_label_set_static_text(label2, " DATE ");
    lv_obj_align(label2, contDate, LV_ALIGN_OUT_TOP_MID, 0, 10);
    lv_label_set_style(label2, LV_LABEL_STYLE_MAIN, &style_label);
    
    static lv_style_t style_cont;
    style_cont = lv_style_plain;
    style_cont.body.opa = LV_OPA_0;
    style_cont.body.border.opa = LV_OPA_COVER;
    style_cont.body.border.width = 1;
    style_cont.body.border.color = LV_COLOR_RED;
    style_cont.body.radius = 10;
    lv_cont_set_style(contTime, LV_CONT_STYLE_MAIN, &style_cont);
    lv_cont_set_style(contDate, LV_CONT_STYLE_MAIN, &style_cont);
}

static void LabelTimeUpdate(uint8_t highLight = 0)
{
    if(highLight > 3)highLight = 0;
    const char* fmt[] = 
    {
        "%02d:%02d:%02d",
        "#FF0000 %02d#:%02d:%02d",
        "%02d:#FF0000 %02d#:%02d",
        "%02d:%02d:#FF0000 %02d#",
    };
    lv_label_set_text_fmt(labelTime, fmt[highLight], RTC_Time.RTC_Hours, RTC_Time.RTC_Minutes, RTC_Time.RTC_Seconds);
}

static void LabelWeekUpdate()
{
    RTCx_GetWeekDay(&RTC_Date);
    int8_t index = RTC_Date.RTC_WeekDay - 1;
    __LimitValue(index, 0, 6);
    lv_label_set_static_text(labelWeek, WeekStr[index]);
}

static void LabelYearUpdate(uint8_t highLight = 0)
{
    const char* fmt = highLight ? "#FF0000 20%02d#" : "20%02d";
    lv_label_set_text_fmt(labelYear, fmt, RTC_Date.RTC_Year);
    LabelWeekUpdate();
}

static void LabelDateUpdate(uint8_t highLight = 0)
{
    if(highLight > 2)highLight = 0;
    const char* fmt[] = 
    {
        "%02d-%02d",
        "#FF0000 %02d#-%02d",
        "%02d-#FF0000 %02d#"
    };
    lv_label_set_text_fmt(labelDate, fmt[highLight], RTC_Date.RTC_Month, RTC_Date.RTC_Date);
    LabelWeekUpdate();
}

static void Creat_LabelTimeDate()
{
    /*时间*/
    RTC_GetTime(RTC_Format_BIN, &RTC_Time);
    
    LV_FONT_DECLARE(HandGotn_26);
    
    labelTime = lv_label_create(contTime, NULL);
    static lv_style_t style_label;
    style_label = *lv_label_get_style(labelTime, LV_LABEL_STYLE_MAIN);
    style_label.text.font = &HandGotn_26;
    style_label.text.color = LV_COLOR_WHITE;
    lv_label_set_style(labelTime, LV_LABEL_STYLE_MAIN, &style_label);
    
    lv_label_set_recolor(labelTime, true);
    LabelTimeUpdate();
    lv_obj_align(labelTime, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_auto_realign(labelTime, true);
    
    /*日期*/
    RTC_GetDate(RTC_Format_BIN, &RTC_Date);
    RTCx_GetWeekDay(&RTC_Date);
    
    labelWeek = lv_label_create(contDate, NULL);
    lv_label_set_style(labelWeek, LV_LABEL_STYLE_MAIN, &style_label);
    lv_label_set_recolor(labelWeek, true);
    LabelWeekUpdate();
    lv_obj_align(labelWeek, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -10);
    lv_obj_set_auto_realign(labelWeek, true);
    
    labelYear = lv_label_create(contDate, NULL);
    lv_label_set_style(labelYear, LV_LABEL_STYLE_MAIN, &style_label);
    lv_label_set_recolor(labelYear, true);
    
    lv_obj_align(labelYear, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);
    lv_obj_set_auto_realign(labelYear, true);
    LabelYearUpdate();
    labelDate= lv_label_create(contDate, NULL);
    lv_label_set_style(labelDate, LV_LABEL_STYLE_MAIN, &style_label);
    lv_label_set_recolor(labelDate, true);
    LabelDateUpdate();
    lv_obj_align(labelDate, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_auto_realign(labelDate, true);
}

static void TimeConfigValueUpdate(int8_t dir = 0)
{
    TitleUpdate();
    
    if(CfgSelIndex != 0 && dir != 0)
    {
        CfgChanged = true;
    }
    
    if(CfgSelIndex >= 1 && CfgSelIndex <= 3)
    {
        if(CfgSelIndex == 1)
        {
            __ValuePlus(RTC_Time.RTC_Hours, dir, 0, 23);
        }
        else if(CfgSelIndex == 2)
        {
            __ValuePlus(RTC_Time.RTC_Minutes, dir, 0, 59);
        }
        else if(CfgSelIndex == 3)
        {
            __ValuePlus(RTC_Time.RTC_Seconds, dir, 0, 59);
        }
        LabelTimeUpdate(CfgSelIndex);
    }
    else
    {
        LabelTimeUpdate(false);
    }
    
    if(CfgSelIndex == 4)
    {
        __ValuePlus(RTC_Date.RTC_Year, dir, 0, 99);
        LabelYearUpdate(true);
    }
    else
    {
        LabelYearUpdate(false);
    }
    
    if(CfgSelIndex >= 5 && CfgSelIndex <= 6)
    {
        if(CfgSelIndex == 5)
        {
            __ValuePlus(RTC_Date.RTC_Month, dir, 1, 12);
        }
        else if(CfgSelIndex == 6)
        {
            __ValuePlus(RTC_Date.RTC_Date, dir, 1, 31);
        }
        LabelDateUpdate(CfgSelIndex - 4);
    }
    else
    {
        LabelDateUpdate(false);
    }
}

static void ContAnimMove(bool down)
{
    int step = down ? 0 : 1;

    int cnt = 2;
    while(cnt--)
    {
        switch(step)
        {
            case 0:
                LV_OBJ_ADD_ANIM(contTimeAnim, y, down ? lv_obj_get_height(contTime) : 0, LV_ANIM_TIME_DEFAULT);
                step = 1;
                break;
            case 1:
                LV_OBJ_ADD_ANIM(contDateAnim, y, down ? lv_obj_get_height(contDate) : 0, LV_ANIM_TIME_DEFAULT);
                step = 0;
                break;
        }
        PageDelay(LV_ANIM_TIME_DEFAULT);
    }
}

static void Creat_ContAnim()
{
    contTimeAnim = lv_cont_create(contTime, contTime);
    lv_obj_set_size(contTimeAnim, lv_obj_get_width(contTime), lv_obj_get_height(contTime));
    lv_obj_align(contTimeAnim, NULL, LV_ALIGN_CENTER, 0, 0);
    
    contDateAnim = lv_cont_create(contDate, contDate);
    lv_obj_set_size(contDateAnim, lv_obj_get_width(contDate), lv_obj_get_height(contDate));
    lv_obj_align(contDateAnim, NULL, LV_ALIGN_CENTER, 0, 0);
    
    static lv_style_t style_cont;
    style_cont = *lv_cont_get_style(contTimeAnim, LV_CONT_STYLE_MAIN);
    style_cont.body.main_color = LV_COLOR_RED;
    style_cont.body.grad_color = LV_COLOR_RED;
    style_cont.body.opa = LV_OPA_100;
    lv_cont_set_style(contTimeAnim, LV_CONT_STYLE_MAIN, &style_cont);
    lv_cont_set_style(contDateAnim, LV_CONT_STYLE_MAIN, &style_cont);
}

/**
  * @brief  页面初始化事件
  * @param  无
  * @retval 无
  */
static void Setup()
{
    /*将此页面移到前台*/
    lv_obj_move_foreground(appWindow);
    
    CfgSelIndex = 0;
    CfgChanged = false;
    
    Creat_Title();
    Creat_ContTimeDate();
    Creat_LabelTimeDate();
    Creat_ContAnim();
    ContAnimMove(true);
}

/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit()
{
    ContAnimMove(false);
    lv_obj_clean(appWindow);
}

/**
  * @brief  页面事件
  * @param  event:事件编号
  * @param  param:事件参数
  * @retval 无
  */
static void Event(int event, void* btn)
{
    if(btn == &btOK)
    {
        if(event == ButtonEvent_Type::EVENT_ButtonLongPressed)
        {
            if(CfgSelIndex == 0 && CfgChanged)
            {
                RTCx_SetTime(
                    RTC_Time.RTC_Hours, 
                    RTC_Time.RTC_Minutes, 
                    RTC_Time.RTC_Seconds, 
                    RTC_Time.RTC_H12
                );
                RTCx_SetDate(
                    RTC_Date.RTC_Year, 
                    RTC_Date.RTC_Month, 
                    RTC_Date.RTC_Date, 
                    RTC_Date.RTC_WeekDay
                );
            }
            page.PagePop();
        }
        if(event == ButtonEvent_Type::EVENT_ButtonClick)
        {
            __ValuePlus(CfgSelIndex, 1, 0, 6);
            TimeConfigValueUpdate();
        }
    }
    
    if(event == ButtonEvent_Type::EVENT_ButtonPress || event == ButtonEvent_Type::EVENT_ButtonLongPressRepeat)
    {
        if(btn == &btUP)
        {
            TimeConfigValueUpdate(+1);
        }
        if(btn == &btDOWN)
        {
            TimeConfigValueUpdate(-1);
        }
    }
}

/**
  * @brief  页面注册
  * @param  pageID:为此页面分配的ID号
  * @retval 无
  */
void PageRegister_TimeCfg(uint8_t pageID)
{
    appWindow = AppWindow_GetCont(pageID);
    page.PageRegister(pageID, Setup, NULL, Exit, Event);
}
