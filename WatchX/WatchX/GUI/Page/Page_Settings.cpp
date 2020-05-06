#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

static lv_obj_t * appWindow;

static lv_obj_t * labelTitle;
static lv_obj_t * lineTitle;

static lv_obj_t * contDisp;
static lv_obj_t * contICON;

LV_IMG_DECLARE(ImgStopWatch);
LV_IMG_DECLARE(ImgMountain);
LV_IMG_DECLARE(ImgLight);
LV_IMG_DECLARE(ImgTimeCfg);
LV_IMG_DECLARE(ImgInfo);
LV_IMG_DECLARE(ImgButterfly);

typedef struct{
    const void * src_img;
    const char * text;
    lv_obj_t * img;
    uint8_t pageID;
}ICON_TypeDef;

ICON_TypeDef ICON_Grp[] = 
{
    {.src_img = &ImgStopWatch, .text = "StopWatch",  .pageID = PAGE_StopWatch},
    {.src_img = &ImgMountain,  .text = "Altitude",   .pageID = PAGE_Altitude},
    {.src_img = &ImgLight,     .text = "Backlight", .pageID = PAGE_Backlight},
    {.src_img = &ImgTimeCfg,   .text = "TimeCfg",    .pageID = PAGE_TimeCfg},
    {.src_img = &ImgButterfly, .text = "Game",       .pageID = PAGE_Game},
    {.src_img = &ImgInfo,      .text = "About",      .pageID = PAGE_About}
};

static const uint8_t ICON_IntervalPixel = 20;
static const uint8_t ICON_Size = 50;
static int8_t ICON_NowSelIndex = 0;

#define ICON_MAX_INDEX (__Sizeof(ICON_Grp) - 1)

static void Creat_ICON_Grp()
{
    contDisp = lv_cont_create(appWindow, NULL);
    lv_cont_set_style(contDisp, LV_CONT_STYLE_MAIN, &lv_style_transp);
    lv_obj_set_size(contDisp, ICON_Size + 20, APP_WIN_HEIGHT - lv_obj_get_y(lineTitle) - 20);
    lv_obj_align(contDisp, lineTitle, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    
    contICON = lv_cont_create(contDisp, NULL);
    lv_cont_set_style(contICON, LV_CONT_STYLE_MAIN, &lv_style_transp);
    lv_obj_set_size(contICON, lv_obj_get_width(contDisp), (ICON_Size + ICON_IntervalPixel) * __Sizeof(ICON_Grp));
    lv_obj_set_y(contICON, lv_obj_get_height(contDisp));
    
    
//    lv_obj_t * contSel = lv_cont_create(contDisp, NULL);
//    lv_obj_set_size(contSel, ICON_Size + 20, ICON_Size + 20);
//    lv_obj_align(contSel, contDisp, LV_ALIGN_CENTER, 0, 0);
//    static lv_style_t style_cont;
//    style_cont = *lv_cont_get_style(contSel, LV_CONT_STYLE_MAIN);
//    style_cont.body.opa = LV_OPA_0;
//    style_cont.body.border.opa = LV_OPA_COVER;
//    style_cont.body.border.width = 2;
//    style_cont.body.border.color = LV_COLOR_RED;
//    style_cont.body.radius = 5;
//    lv_cont_set_style(contSel, LV_CONT_STYLE_MAIN, &style_cont);
    
    for(int i = 0; i < __Sizeof(ICON_Grp); i++)
    {
        lv_obj_t * img = lv_img_create(contICON, NULL);
        ICON_Grp[i].img = img;
        lv_img_set_src(img, ICON_Grp[i].src_img);
        lv_obj_align(img, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
        
        int y_offset = (ICON_Size - lv_obj_get_height(img)) / 2;
        
        lv_obj_set_y(img, (ICON_Size + ICON_IntervalPixel) * i + y_offset);
    }
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
    
    lv_label_set_static_text(labelTitle, ICON_Grp[1].text);
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

static void ICON_Grp_MoveFouce(uint8_t iconIndex)
{
    if(iconIndex > ICON_MAX_INDEX)
        return;
    
    lv_label_set_static_text(labelTitle, ICON_Grp[iconIndex].text);
    
    int16_t target_y = -(ICON_Size + ICON_IntervalPixel) * (iconIndex - 1);

    LV_OBJ_ADD_ANIM(contICON, y, target_y, LV_ANIM_TIME_DEFAULT);
}

static void ICON_Grp_Move(int8_t dir)
{
    __ValuePlus(ICON_NowSelIndex, dir, 0, ICON_MAX_INDEX);
    ICON_Grp_MoveFouce(ICON_NowSelIndex);
}

static void Creat_ImgShadow(void)
{
    LV_IMG_DECLARE(ImgShadowUp);
    LV_IMG_DECLARE(ImgShadowDown);
    
    lv_obj_t * imgUp = lv_img_create(contDisp, NULL);
    lv_img_set_src(imgUp, &ImgShadowUp);
    lv_obj_align(imgUp, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    
    lv_obj_t * imgDown = lv_img_create(contDisp, NULL);
    lv_img_set_src(imgDown, &ImgShadowDown);
    lv_obj_align(imgDown, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
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
    Creat_Title();
    Creat_ICON_Grp();
    Creat_ImgShadow();
    ICON_Grp_MoveFouce(ICON_NowSelIndex);
}

/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit()
{
    LV_OBJ_ADD_ANIM(contICON, y, lv_obj_get_height(contDisp) + ICON_Size, LV_ANIM_TIME_DEFAULT);
    PageDelay(LV_ANIM_TIME_DEFAULT);
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
        else if(event == ButtonEvent_Type::EVENT_ButtonClick)
        {
            uint8_t pageID = ICON_Grp[ICON_NowSelIndex].pageID;
            if(pageID != PAGE_NONE)
            {
                page.PagePush(pageID);
            }
        }
    }
    
    if(event == ButtonEvent_Type::EVENT_ButtonPress || event == ButtonEvent_Type::EVENT_ButtonLongPressRepeat)
    {
        if(btn == &btUP)
        {
            ICON_Grp_Move(-1);
        }
        if(btn == &btDOWN)
        {
            ICON_Grp_Move(+1);
        }
    }
}

/**
  * @brief  页面注册
  * @param  pageID:为此页面分配的ID号
  * @retval 无
  */
void PageRegister_Settings(uint8_t pageID)
{
    appWindow = AppWindow_GetCont(pageID);
    page.PageRegister(pageID, Setup, NULL, Exit, Event);
}
