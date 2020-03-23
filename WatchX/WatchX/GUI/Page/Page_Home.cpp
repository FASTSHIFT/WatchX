#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

static RTC_TimeTypeDef RTC_Time;
static RTC_TimeTypeDef RTC_TimeLast;
static RTC_DateTypeDef RTC_Date;

static lv_obj_t * appWindow;
static lv_obj_t * imgBg;

static lv_obj_t * labelBMP;
static lv_obj_t * labelBatt;

static lv_obj_t * labelDate;

static lv_obj_t * contTime;
static lv_obj_t * ledSec[2];
static lv_obj_t * labelTime_Grp[4];
static lv_obj_t * labelTime_Grp2[4];

static lv_obj_t * imgRun;
static lv_obj_t * labelStepCnt;

static lv_task_t * taskTimeUpdate;
static lv_task_t * taskTopBarUpdate;

static void Creat_ImgBg(void)
{
    LV_IMG_DECLARE(ImgBg);
    imgBg = lv_img_create(appWindow, NULL);
    lv_img_set_src(imgBg, &ImgBg);
    lv_obj_align(imgBg, NULL, LV_ALIGN_CENTER, 0, 0);
}

static void Task_TopBarUpdate(lv_task_t * task)
{
    Task_BMP_Update();
    lv_label_set_text_fmt(labelBMP, "% 2dC"LV_SYMBOL_DEGREE_SIGN" %dm", (int)BMP180.temperature, (int)BMP180.altitude);
    
    float battVoltage = 3.7;//(float)analogRead_DMA(BAT_DET_Pin) / 4095.0f * 3.3f * 2;
    bool Is_BattCharging = digitalRead(BAT_CHG_Pin);

    const char * battSymbol[] =
    {
        LV_SYMBOL_BATTERY_EMPTY,
        LV_SYMBOL_BATTERY_1,
        LV_SYMBOL_BATTERY_2,
        LV_SYMBOL_BATTERY_3,
        LV_SYMBOL_BATTERY_FULL
    };
    
    int symIndex = fmap(
        battVoltage, 
        2.8f, 4.2f, 
        0, __Sizeof(battSymbol)
    );
    __LimitValue(symIndex, 0, __Sizeof(battSymbol) - 1);
    
    if(Is_BattCharging)
    {
        static uint8_t usage = 0;
        usage++;
        usage %= (symIndex + 1);
        symIndex = usage;
    }

    lv_label_set_text_fmt(labelBatt, "#FFFFFF %s#", battSymbol[symIndex]);
}

static void Creat_LabelTopBar()
{
    LV_FONT_DECLARE(HandGotn_14);
    labelBMP = lv_label_create(appWindow, NULL);
    static lv_style_t bmp_style;
    bmp_style = *lv_label_get_style(labelBMP, LV_LABEL_STYLE_MAIN);
    bmp_style.text.font = &HandGotn_14;
    bmp_style.text.color = LV_COLOR_WHITE;
    lv_label_set_style(labelBMP, LV_LABEL_STYLE_MAIN, &bmp_style);
    lv_label_set_text(labelBMP, "-- C -- kPa");
    lv_obj_align(labelBMP, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_obj_set_auto_realign(labelBMP, true);
    
    labelBatt = lv_label_create(appWindow, NULL);
    lv_label_set_recolor(labelBatt, true);
    lv_label_set_text(labelBatt, "#FFFFFF "LV_SYMBOL_BATTERY_EMPTY"#");
    lv_obj_align(labelBatt, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
    lv_obj_set_auto_realign(labelBatt, true);
    
    taskTopBarUpdate = lv_task_create(Task_TopBarUpdate, 2 * 1000, LV_TASK_PRIO_LOW, NULL);
    Task_TopBarUpdate(taskTopBarUpdate);
}

#define LABEL_TIME_CHECK_DEF(val_now,val_last,index)\
do{\
    if((val_now) != (val_last))\
    {\
        lv_obj_t * next_label;\
        lv_obj_t * now_label;\
        if(lv_obj_get_y(labelTime_Grp2[index]) > lv_obj_get_y(labelTime_Grp[index]))\
        {\
            now_label = labelTime_Grp[index];\
            next_label = labelTime_Grp2[index];\
        }\
        else\
        {\
            now_label = labelTime_Grp2[index];\
            next_label = labelTime_Grp[index];\
        }\
        \
        lv_label_set_text_fmt(now_label, "%d", (val_last));\
        lv_label_set_text_fmt(next_label, "%d", (val_now));\
        \
        lv_obj_align(next_label, now_label, LV_ALIGN_OUT_TOP_MID, 0, -10);\
        lv_coord_t y_offset = abs(lv_obj_get_y(now_label) - lv_obj_get_y(next_label));\
        LV_OBJ_ADD_ANIM(now_label, y, lv_obj_get_y(now_label) + y_offset, LV_ANIM_TIME_DEFAULT);\
        LV_OBJ_ADD_ANIM(next_label, y, lv_obj_get_y(next_label) + y_offset, LV_ANIM_TIME_DEFAULT);\
    }\
}while(0)

static void LabelTimeGrp_Update()
{
    RTC_GetTime(RTC_Format_BIN, &RTC_Time);
//    RTC_Time.RTC_Hours = (millis() / (3600 * 1000)) % 100;
//    RTC_Time.RTC_Minutes = (millis() / (60 * 1000)) % 60;
//    RTC_Time.RTC_Seconds = (millis() / 1000) % 60;
    
//    /*秒-个位*/
//    LABEL_TIME_CHECK_DEF(RTC_Time.RTC_Seconds % 10,RTC_TimeLast.RTC_Seconds % 10, 3);
//    /*秒-十位*/
//    LABEL_TIME_CHECK_DEF(RTC_Time.RTC_Seconds / 10,RTC_TimeLast.RTC_Seconds / 10, 2);
    
    /*分-个位*/
    LABEL_TIME_CHECK_DEF(RTC_Time.RTC_Minutes % 10,RTC_TimeLast.RTC_Minutes % 10, 3);
    /*分-十位*/
    LABEL_TIME_CHECK_DEF(RTC_Time.RTC_Minutes / 10,RTC_TimeLast.RTC_Minutes / 10, 2);
    
    /*时-个位*/
    LABEL_TIME_CHECK_DEF(RTC_Time.RTC_Hours % 10,RTC_TimeLast.RTC_Hours % 10, 1);
    /*时-十位*/
    LABEL_TIME_CHECK_DEF(RTC_Time.RTC_Hours / 10,RTC_TimeLast.RTC_Hours / 10, 0);
    
    RTC_TimeLast = RTC_Time;
}

static void Task_TimeUpdate(lv_task_t * task)
{
    LabelTimeGrp_Update();
    
    lv_led_toggle(ledSec[0]);
    lv_led_toggle(ledSec[1]);
    
    /*日期*/
    RTC_GetDate(RTC_Format_BIN, &RTC_Date);
    const char* week_str[7] = { "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
    int8_t index = RTC_Date.RTC_WeekDay - 1;
    __LimitValue(index, 0, 6);
    lv_label_set_text_fmt(labelDate, "%02d#FF0000 /#%02d %s", RTC_Date.RTC_Month, RTC_Date.RTC_Date, week_str[index]);
}

static void Creat_LabelDate()
{
    LV_FONT_DECLARE(Morganite_36);
    labelDate = lv_label_create(appWindow, NULL);
    
    static lv_style_t bmp_style;
    bmp_style = *lv_label_get_style(labelDate, LV_LABEL_STYLE_MAIN);
    bmp_style.text.font = &Morganite_36;
    bmp_style.text.color = LV_COLOR_WHITE;
    lv_label_set_style(labelDate, LV_LABEL_STYLE_MAIN, &bmp_style);
    
    lv_label_set_recolor(labelDate, true);
    lv_label_set_text(labelDate, "01#FF0000 /#01 MON");
    lv_obj_align(labelDate, NULL, LV_ALIGN_IN_TOP_MID, 0, 30);
    lv_obj_set_auto_realign(labelDate, true);
}

static void Creat_LabelTime()
{
    Creat_LabelDate();
    
    /*contTime*/
    contTime = lv_cont_create(appWindow, NULL);
    lv_cont_set_style(contTime, LV_CONT_STYLE_MAIN, &lv_style_transp);
    lv_obj_set_size(contTime, 130, 80);
    lv_obj_align(contTime, NULL, LV_ALIGN_CENTER, 0, 0);
    
    /*ledSec*/
    static lv_style_t led_style;
    led_style = lv_style_pretty_color;
    led_style.body.main_color = LV_COLOR_RED;
    led_style.body.grad_color = LV_COLOR_RED;
    for(int i = 0; i < __Sizeof(ledSec); i++)
    {
        lv_obj_t * led = lv_led_create(contTime, NULL);
        lv_led_set_style(led, LV_LED_STYLE_MAIN, &led_style);
        lv_obj_set_size(led, 8, 10);
        lv_obj_align(led, NULL, LV_ALIGN_CENTER, 0, i == 0 ? -10 : 10);
        ledSec[i] = led;
    }
    
    /*labelTime*/
    LV_FONT_DECLARE(Morganite_100);
    static lv_style_t time_style;
    time_style = lv_style_plain;
    time_style.text.font = &Morganite_100;
    time_style.text.color = LV_COLOR_WHITE;
    const lv_coord_t x_mod[4] = {-45, -20, 20, 45};
    for(int i = 0; i < __Sizeof(labelTime_Grp); i++)
    {
        lv_obj_t * label = lv_label_create(contTime, NULL);
        lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &time_style);
        lv_label_set_text(label, "0");
        lv_obj_align(label, NULL, LV_ALIGN_CENTER, x_mod[i], 0);
        labelTime_Grp[i] = label;
    }
    for(int i = 0; i < __Sizeof(labelTime_Grp2); i++)
    {
        lv_obj_t * label = lv_label_create(contTime, NULL);
        lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &time_style);
        lv_label_set_text(label, "0");
        lv_obj_align(label, labelTime_Grp[i], LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
        labelTime_Grp2[i] = label;
    }

    memset(&RTC_TimeLast, 0, sizeof(RTC_TimeLast));
    taskTimeUpdate = lv_task_create(Task_TimeUpdate, 500, LV_TASK_PRIO_MID, NULL);
    Task_TimeUpdate(taskTimeUpdate);
}

static void Creat_LabelStep()
{
    LV_IMG_DECLARE(ImgRun);
    imgRun = lv_img_create(appWindow, NULL);
    lv_img_set_src(imgRun, &ImgRun);
    lv_obj_align(imgRun, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 20, -20);
    
    LV_FONT_DECLARE(HandGotn_26);
    
    labelStepCnt = lv_label_create(appWindow, NULL);
    
    static lv_style_t step_style;
    step_style = *lv_label_get_style(labelStepCnt, LV_LABEL_STYLE_MAIN);
    step_style.text.font = &HandGotn_26;
    step_style.text.color = LV_COLOR_WHITE;
    lv_label_set_style(labelStepCnt, LV_LABEL_STYLE_MAIN, &step_style);
    
    lv_label_set_recolor(labelStepCnt, true);
    lv_label_set_text(labelStepCnt, "#FF0000 /# 1255");
    lv_obj_align(labelStepCnt, imgRun, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_set_auto_realign(labelStepCnt, true);
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
    
    Creat_ImgBg();
    Creat_LabelTopBar();
    Creat_LabelTime();
    Creat_LabelStep();
    
    Power_SetAutoLowPowerEnable(true);
}

static void Loop()
{
}

/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit()
{
    lv_task_del(taskTimeUpdate);
    lv_task_del(taskTopBarUpdate);
    lv_obj_clean(appWindow);
    
    Power_SetAutoLowPowerEnable(false);
}

/**
  * @brief  页面事件
  * @param  event:事件编号
  * @param  param:事件参数
  * @retval 无
  */
static void Event(int event, void* btn)
{
    if(event == ButtonEvent_Type::EVENT_ButtonClick || event == ButtonEvent_Type::EVENT_ButtonLongPressed)
    {
        page.PagePush(PAGE_Settings);
    }
}

/**
  * @brief  页面注册
  * @param  pageID:为此页面分配的ID号
  * @retval 无
  */
void PageRegister_Home(uint8_t pageID)
{
    appWindow = AppWindow_GetCont(pageID);
    page.PageRegister(pageID, Setup, Loop, Exit, Event);
}
