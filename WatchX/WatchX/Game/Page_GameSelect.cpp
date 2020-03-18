#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "GamePrivate.h"

static lv_obj_t * appWindow;
static lv_obj_t * rollerGame;
static lv_obj_t * btnStart;

static void Obj_EventHandler(lv_obj_t * obj, lv_event_t event)
{
    static char name[32];
    if(obj == rollerGame)
    {
        if(event == LV_EVENT_VALUE_CHANGED)
        {
            lv_roller_get_selected_str(obj, name, sizeof(name));
        }
    }
    if(obj == btnStart)
    {
        if(event == LV_EVENT_CLICKED)
        {
            int index = Game_GetNameID(name);
            if(index > 0)
            {
                GamePage.PageChangeTo(index);
                page.PagePush(PAGE_Game);
            }
        }
    }
}

static void Creat_GameStartButton()
{
    btnStart = lv_btn_create(appWindow, NULL);

    lv_obj_set_size(btnStart, lv_obj_get_width(rollerGame), 50);
    lv_obj_align(btnStart, rollerGame, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_event_cb(btnStart, Obj_EventHandler);
    
    lv_obj_t * label = lv_label_create(btnStart, NULL);
    lv_label_set_text(label, "GO!");
    
    lv_btn_set_ink_in_time(btnStart, 200);
    lv_btn_set_ink_out_time(btnStart, 200);
    lv_obj_set_protect(btnStart, LV_PROTECT_PRESS_LOST);
}

static void Creat_RollerGame(void)
{
    rollerGame = lv_roller_create(appWindow, NULL);
    
    static lv_style_t style = *lv_roller_get_style(rollerGame, LV_ROLLER_STYLE_BG);
    style.text.font = &lv_font_roboto_28;
    lv_roller_set_options(rollerGame, Game_GetNameList().c_str(), LV_ROLLER_MODE_INIFINITE);
    lv_roller_set_visible_row_count(rollerGame, 3);
    lv_obj_align(rollerGame, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_event_cb(rollerGame, Obj_EventHandler);
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
    
    Game_Begin();
    
    Creat_RollerGame();
    Creat_GameStartButton();
}

/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit()
{
    lv_obj_clean(appWindow);
}

/**
  * @brief  页面事件
  * @param  event:事件编号
  * @param  param:事件参数
  * @retval 无
  */
static void Event(int event, void* param)
{
    lv_obj_t * btn = (lv_obj_t*)param;
    if(event == LV_EVENT_CLICKED)
    {
        if(btn == btnBack)
        {
            page.PagePop();
        }
    }
}

/**
  * @brief  页面注册
  * @param  pageID:为此页面分配的ID号
  * @retval 无
  */
void PageRegister_GameSelect(uint8_t pageID)
{
    appWindow = AppWindow_GetCont(pageID);
    lv_style_t * style = (lv_style_t *)lv_cont_get_style(appWindow, LV_CONT_STYLE_MAIN);
    *style = lv_style_pretty;
    page.PageRegister(pageID, Setup, NULL, Exit, Event);
}
