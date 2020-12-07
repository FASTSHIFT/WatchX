#include "GUI/DisplayPrivate.h"
#include "Basic/CommonMacro.h"
#include "BSP/BSP.h"

/*此页面窗口*/
static lv_obj_t * appWindow;

/*标题栏*/
static lv_obj_t * labelTitle;

/*标题栏分隔线*/
static lv_obj_t * lineTitle;

/*图标显示容器，用于裁剪显示*/
static lv_obj_t * contDisp;

/*图标组容器*/
static lv_obj_t * contICON;

/*图片资源*/
LV_IMG_DECLARE(ImgStopWatch);
LV_IMG_DECLARE(ImgMountain);
LV_IMG_DECLARE(ImgLight);
LV_IMG_DECLARE(ImgTimeCfg);
LV_IMG_DECLARE(ImgInfo);
LV_IMG_DECLARE(ImgButterfly);

/*图标信息定义*/
typedef struct{
    const void * src_img;   //图片数据地址
    const char * text;      //文字描述
    lv_obj_t * img;         //图片控件
    uint8_t pageID;         //对应的页面ID
}ICON_TypeDef;

/*图标组定义*/
static ICON_TypeDef ICON_Grp[] = 
{
    {.src_img = &ImgStopWatch, .text = "StopWatch",  .pageID = PAGE_StopWatch},  //秒表
    {.src_img = &ImgMountain,  .text = "Altitude",   .pageID = PAGE_Altitude},   //海拔表
    {.src_img = &ImgLight,     .text = "Backlight",  .pageID = PAGE_Backlight},  //背光设置
    {.src_img = &ImgTimeCfg,   .text = "TimeCfg",    .pageID = PAGE_TimeCfg},    //时间设置
    {.src_img = &ImgButterfly, .text = "Game",       .pageID = PAGE_Game},       //游戏
    {.src_img = &ImgInfo,      .text = "About",      .pageID = PAGE_About}       //关于
};

/*图标间隔*/
static const uint8_t ICON_IntervalPixel = 20;
/*图标大小*/
static const uint8_t ICON_Size = 50;
/*当前选中的图标索引*/
static int8_t ICON_NowSelIndex = 0;
/*图标索引最大值*/
#define ICON_MAX_INDEX (__Sizeof(ICON_Grp) - 1)

/**
  * @brief  创建图标组
  * @param  无
  * @retval 无
  */
static void ICON_Grp_Create()
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
    
    /*遍历图标组*/
    for(int i = 0; i < __Sizeof(ICON_Grp); i++)
    {
        /*创建图片控件*/
        lv_obj_t * img = lv_img_create(contICON, NULL);
        ICON_Grp[i].img = img;
        lv_img_set_src(img, ICON_Grp[i].src_img);
        lv_obj_align(img, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
        
        /*计算偏移量*/
        int y_offset = (ICON_Size - lv_obj_get_height(img)) / 2;
        
        lv_obj_set_y(img, (ICON_Size + ICON_IntervalPixel) * i + y_offset);
    }
}

/**
  * @brief  创建标题栏
  * @param  无
  * @retval 无
  */
static void Title_Create()
{
    LV_FONT_DECLARE(HandGotn_20);
    labelTitle = lv_label_create(appWindow, NULL);
    
    static lv_style_t style_label;
    style_label = *lv_label_get_style(labelTitle, LV_LABEL_STYLE_MAIN);
    style_label.text.font = &HandGotn_20;
    style_label.text.color = LV_COLOR_WHITE;
    lv_label_set_style(labelTitle, LV_LABEL_STYLE_MAIN, &style_label);
    
    /*默认选中的是第二个图标*/
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

/**
  * @brief  移动到选中图标
  * @param  iconIndex:目标图标的索引
  * @retval 无
  */
static void ICON_Grp_MoveFocus(uint8_t iconIndex)
{
    if(iconIndex > ICON_MAX_INDEX)
        return;
    
    /*改变标题栏的文字说明*/
    lv_label_set_static_text(labelTitle, ICON_Grp[iconIndex].text);
    
    /*计算目标Y坐标*/
    int16_t target_y = -(ICON_Size + ICON_IntervalPixel) * (iconIndex - 1);

    /*执行滑动动画*/
    LV_OBJ_ADD_ANIM(contICON, y, target_y, LV_ANIM_TIME_DEFAULT);
}

/**
  * @brief  上下移动选中的图标
  * @param  dir:方向
  * @retval 无
  */
static void ICON_Grp_Move(int8_t dir)
{
    /*在限定范围内移动*/
    __ValuePlus(ICON_NowSelIndex, dir, 0, ICON_MAX_INDEX);
    
    /*移动到新图标*/
    ICON_Grp_MoveFocus(ICON_NowSelIndex);
}

/**
  * @brief  创建滚轮阴影
  * @param  无
  * @retval 无
  */
static void ImgShadow_Create()
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
    Title_Create();
    ICON_Grp_Create();
    ImgShadow_Create();
    
    /*图标滑到上一次选中的图标*/
    ICON_Grp_MoveFocus(ICON_NowSelIndex);
}

/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit()
{
    /*图标全部滑出*/
    LV_OBJ_ADD_ANIM(contICON, y, lv_obj_get_height(contDisp) + ICON_Size, LV_ANIM_TIME_DEFAULT);
    PageDelay(LV_ANIM_TIME_DEFAULT);
    lv_obj_clean(appWindow);
}

/**
  * @brief  页面事件
  * @param  btn:发出事件的按键
  * @param  event:事件编号
  * @retval 无
  */
static void Event(void* btn, int event)
{
    if(btn == &btOK)
    {
        if(event == ButtonEvent::EVENT_ButtonLongPressed)
        {
            /*长按OK，退出上一个页面*/
            page.PagePop();
        }
        else if(event == ButtonEvent::EVENT_ButtonClick)
        {
            /*单击OK，进入对应页面*/
            uint8_t pageID = ICON_Grp[ICON_NowSelIndex].pageID;
            if(pageID != PAGE_NONE)
            {
                page.PagePush(pageID);
            }
        }
    }
    
    if(event == ButtonEvent::EVENT_ButtonPress || event == ButtonEvent::EVENT_ButtonLongPressRepeat)
    {
        /*按下或长按上下键，图标上下选择*/
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
void PageRegister_MainMenu(uint8_t pageID)
{
    /*获取分配给此页面的窗口*/
    appWindow = AppWindow_GetCont(pageID);
    
    /*注册至页面调度器*/
    page.PageRegister(pageID, Setup, NULL, Exit, Event);
}
