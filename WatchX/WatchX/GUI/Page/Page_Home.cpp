#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

static RTC_TimeTypeDef RTC_Time;
static RTC_DateTypeDef RTC_Date;

static lv_obj_t * appWindow;
static lv_obj_t * imgBg;

static lv_obj_t * labelBMP;
static lv_obj_t * labelBatt;

static lv_obj_t * labelDate;

static lv_obj_t * labelTime;

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
    
    taskTopBarUpdate = lv_task_create(Task_TopBarUpdate, 2 * 1000, LV_TASK_PRIO_MID, NULL);
    Task_TopBarUpdate(taskTopBarUpdate);
}

static void Task_TimeUpdate(lv_task_t * task)
{
    /*时间显示*/
    RTC_GetTime(RTC_Format_BIN, &RTC_Time);
//    uint16_t hh = (millis() / (3600 * 1000)) % 100;
//    uint16_t mm = (millis() / (60 * 1000)) % 60;
//    uint16_t ss = (millis() / 1000) % 60;
    
    static bool showRed;
    lv_label_set_text_fmt(labelTime, showRed ? "%02d#FF0000 :#%02d" : "%02d:%02d", RTC_Time.RTC_Hours, RTC_Time.RTC_Minutes);
    showRed = !showRed;
    
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
    
    LV_FONT_DECLARE(Morganite_100);
    /*Time*/
    labelTime = lv_label_create(appWindow, NULL);
    static lv_style_t hour_style;
    hour_style = *lv_label_get_style(labelTime, LV_LABEL_STYLE_MAIN);
    hour_style.text.font = &Morganite_100;
    hour_style.text.color = LV_COLOR_WHITE;
    lv_label_set_style(labelTime, LV_LABEL_STYLE_MAIN, &hour_style);
    lv_label_set_recolor(labelTime, true);
    lv_label_set_text(labelTime, "00:00");
    lv_obj_align(labelTime, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_auto_realign(labelTime, true);
    
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
