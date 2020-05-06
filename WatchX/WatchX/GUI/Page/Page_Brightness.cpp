#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

static lv_obj_t * appWindow;

static lv_obj_t * labelTitle;
static lv_obj_t * lineTitle;

static lv_obj_t * labelBright;
static lv_obj_t * arcBright;

const uint16_t BrightMinVal = 10;
static int16_t ArcNowAngle = 0;

static void ArcBright_AnimCallback(lv_obj_t * obj, int16_t angle)
{
    ArcNowAngle = angle;
    if(angle < 180)
    {
        lv_arc_set_angles(obj, 180 - angle ,180);
    }
    else 
    {
        lv_arc_set_angles(obj, 540 - angle ,180);
    }
}

static void BrightArcSetVal(int targetAngle, uint16_t anim_time = 500)
{
    static lv_anim_t a;
    lv_obj_add_anim(
        arcBright, &a,
        (lv_anim_exec_xcb_t)ArcBright_AnimCallback,
        ArcNowAngle,
        targetAngle,
        anim_time
    );
}

static void BrightCtrl(int8_t dir)
{
    int16_t brightVal = Backlight_GetValue();
    brightVal += dir;
    __LimitValue(brightVal, BrightMinVal, 1000);
    Backlight_SetValue(brightVal);
    lv_label_set_text_fmt(labelBright, "%d", map(brightVal, BrightMinVal, 1000, 0, 100));
    
    if(dir != 0)
    {
        int targetAngle = map(brightVal, BrightMinVal, 1000, 0, 359);
        BrightArcSetVal(targetAngle);
    }
}

static void Creat_Title(const char * text)
{
    LV_FONT_DECLARE(HandGotn_20);
    labelTitle = lv_label_create(appWindow, NULL);
    
    static lv_style_t style_label;
    style_label = *lv_label_get_style(labelTitle, LV_LABEL_STYLE_MAIN);
    style_label.text.font = &HandGotn_20;
    style_label.text.color = LV_COLOR_WHITE;
    lv_label_set_style(labelTitle, LV_LABEL_STYLE_MAIN, &style_label);
    
    lv_label_set_static_text(labelTitle, text);
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

static void Creat_Bright()
{
    static lv_style_t style_arc;
    style_arc = lv_style_plain;
    style_arc.line.color = LV_COLOR_RED;
    style_arc.line.width = 8;
    style_arc.body.shadow.type = LV_SHADOW_FULL;
    style_arc.body.shadow.color = LV_COLOR_RED;
    style_arc.body.shadow.width = 10;

    arcBright = lv_arc_create(appWindow, NULL);
    lv_obj_set_size(arcBright, APP_WIN_WIDTH - 20, APP_WIN_WIDTH - 20);
    lv_arc_set_angles(arcBright, 180, 180);
    lv_arc_set_style(arcBright, LV_ARC_STYLE_MAIN, &style_arc);
    lv_obj_align(arcBright, NULL, LV_ALIGN_CENTER, 0, lv_obj_get_y(lineTitle));
    
    LV_FONT_DECLARE(Morganite_100);
    
    labelBright = lv_label_create(appWindow, NULL);
    static lv_style_t style_label;
    style_label = *lv_label_get_style(labelBright, LV_LABEL_STYLE_MAIN);
    style_label.text.font = &Morganite_100;
    style_label.text.color = LV_COLOR_WHITE;
    lv_label_set_style(labelBright, LV_LABEL_STYLE_MAIN, &style_label);
    lv_obj_align(labelBright, arcBright, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_auto_realign(labelBright, true);
    lv_obj_set_opa_scale_enable(labelBright, true);
    lv_obj_set_opa_scale(labelBright, LV_OPA_TRANSP);
}

static void BrightAnim(bool open)
{
    int step = open ? 0 : 1;
    
    int cnt = 2;
    int targetAngle;
    while(cnt--)
    {
        switch(step)
        {
            case 0:
                targetAngle = open ? map(Backlight_GetValue(), BrightMinVal, 1000, 0, 359) : 0;
                BrightArcSetVal(targetAngle, 400);
                step = 1;
                break;
            case 1:
                LV_OBJ_ADD_ANIM(labelBright, opa_scale, open ? LV_OPA_COVER : LV_OPA_TRANSP, 400);
                step = 0;
                break;
        }
        PageDelay(400);
    }
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
    
    ArcNowAngle = 0;
    
    Creat_Title("Backlight");
    Creat_Bright();

    BrightCtrl(0);
    BrightAnim(true);
}

/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit()
{
    BrightAnim(false);
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
            page.PagePop();
        }
    }
    
    if(event == ButtonEvent_Type::EVENT_ButtonPress || event == ButtonEvent_Type::EVENT_ButtonLongPressRepeat)
    {
        int valPlus = event == ButtonEvent_Type::EVENT_ButtonPress ? 10 : 50;
        if(btn == &btUP)
        {
            BrightCtrl(+valPlus);
        }
        if(btn == &btDOWN)
        {
            BrightCtrl(-valPlus);
        }
    }
}

/**
  * @brief  页面注册
  * @param  pageID:为此页面分配的ID号
  * @retval 无
  */
void PageRegister_Backlight(uint8_t pageID)
{
    appWindow = AppWindow_GetCont(pageID);
    page.PageRegister(pageID, Setup, NULL, Exit, Event);
}
