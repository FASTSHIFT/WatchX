#include "DisplayPrivate.h"

typedef struct{
    lv_obj_t * cont;
    lv_style_t style;
}AppWindow_TypeDef;

static AppWindow_TypeDef appWindow_Grp[PAGE_MAX];

lv_obj_t * AppWindow_GetCont(uint8_t pageID)
{
    return (pageID < PAGE_MAX) ? appWindow_Grp[pageID].cont : NULL;
}

lv_coord_t AppWindow_GetHeight()
{
    return (lv_obj_get_height(lv_scr_act()));// - BarStatus_GetHeight() - BarNavigation_GetHeight());
}

lv_coord_t AppWindow_GetWidth()
{
    return (lv_obj_get_width(lv_scr_act()));
}

void Creat_AppWindow()
{
    for(int i = 0; i < PAGE_MAX; i++)
    {
        appWindow_Grp[i].cont = lv_cont_create(lv_scr_act(), NULL);

        lv_obj_set_size(appWindow_Grp[i].cont, AppWindow_GetWidth(), AppWindow_GetHeight());
        lv_obj_align(appWindow_Grp[i].cont, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_cont_set_fit(appWindow_Grp[i].cont, LV_FIT_NONE);
        
        appWindow_Grp[i].style = lv_style_plain;
        appWindow_Grp[i].style.body.main_color = LV_COLOR_BLACK;
        appWindow_Grp[i].style.body.grad_color = LV_COLOR_BLACK;
        lv_cont_set_style(appWindow_Grp[i].cont, LV_CONT_STYLE_MAIN, &appWindow_Grp[i].style);
    }
}
