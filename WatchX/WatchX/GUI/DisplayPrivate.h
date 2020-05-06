#ifndef __DISPLAYPRIVATE_H
#define __DISPLAYPRIVATE_H

/*Basic*/
#include "Adafruit_ST7789/Adafruit_ST7789.h"

typedef Adafruit_ST7789 SCREEN_CLASS;

extern SCREEN_CLASS screen;

#define TEXT_HEIGHT_1   8
#define TEXT_WIDTH_1    6
#define TextSetDefault() screen.setTextColor(screen.White,screen.Black),screen.setTextSize(1)
#define TextMid(x,textnum) (screen.width()*(x)-(TEXT_WIDTH_1*(textnum))/2.0f)
#define TextMidPrint(x,y,text) screen.setCursor(TextMid(x,strlen(text)),screen.height()*(y)-TEXT_HEIGHT_1/2.0f),screen.print(text)
#define ScreenMid_W (screen.width()/2)
#define ScreenMid_H (screen.height()/2)

void DisplayError_Init();

/*Page*/
#include "PageManager/PageManager.h"
typedef enum
{
    /*保留*/
    PAGE_NONE,
    /*用户页面*/
    PAGE_Home,
    PAGE_Settings,
    PAGE_TimeCfg,
    PAGE_Backlight,
    PAGE_StopWatch,
    PAGE_Altitude,
    PAGE_About,
    PAGE_Game,
    /*保留*/
    PAGE_MAX
} Page_Type;

extern PageManager page;
void PageDelay(uint32_t ms);
#define PageWaitUntil(condition)\
while(!(condition)){\
    lv_task_handler();\
}

/*LittleVGL*/
#include "lvgl/lvgl.h"

#define LV_ANIM_TIME_DEFAULT 200
#define LV_SYMBOL_DEGREE_SIGN   "\xC2\xB0"

void lv_port_disp_init();
bool lv_obj_del_safe(lv_obj_t** obj);
void lv_label_set_text_add(lv_obj_t * label, const char * text);
lv_coord_t lv_obj_get_x_center(lv_obj_t * obj);
lv_coord_t lv_obj_get_y_center(lv_obj_t * obj);
void lv_obj_set_color(lv_obj_t * obj, lv_color_t color);
void lv_table_set_align(lv_obj_t * table, lv_label_align_t align);
lv_obj_t * lv_win_get_label(lv_obj_t * win);
void lv_obj_add_anim(
    lv_obj_t * obj, lv_anim_t * a,
    lv_anim_exec_xcb_t exec_cb, 
    int32_t start, int32_t end,
    uint16_t time = LV_ANIM_TIME_DEFAULT,
    lv_anim_ready_cb_t ready_cb = NULL,
    lv_anim_path_cb_t path_cb = lv_anim_path_ease_in_out
);
#define LV_OBJ_ADD_ANIM(obj,attr,target,time)\
do{\
    static lv_anim_t a;\
    lv_obj_add_anim(\
        (obj), &a,\
        (lv_anim_exec_xcb_t)lv_obj_set_##attr,\
        lv_obj_get_##attr(obj),\
        (target),\
        (time)\
    );\
}while(0)

/*AppWindow*/
void Creat_AppWindow();
lv_obj_t * AppWindow_GetCont(uint8_t pageID);
lv_coord_t AppWindow_GetHeight();
lv_coord_t AppWindow_GetWidth();
#define APP_WIN_HEIGHT AppWindow_GetHeight()
#define APP_WIN_WIDTH  AppWindow_GetWidth()

#endif
