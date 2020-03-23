#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

static lv_obj_t * appWindow;

static lv_obj_t * labelTitle;
static lv_obj_t * lineTitle;

static lv_obj_t * contKPaTemp;
static lv_obj_t * labelKPa;
static lv_obj_t * labelTemp;

static lv_obj_t * chartAlt;
static lv_chart_series_t * serAlt;
static lv_obj_t * labelAlt;

static lv_task_t * taskChartUpdate;

static lv_style_t style_label_public;

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

static void Creat_ContKPaTemp()
{
    contKPaTemp = lv_cont_create(appWindow, NULL);
    lv_obj_set_size(contKPaTemp, 130, 58);
    lv_obj_align(contKPaTemp, lineTitle, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_opa_scale_enable(contKPaTemp, true);
    lv_obj_set_opa_scale(contKPaTemp, LV_OPA_TRANSP);
    
    static lv_style_t style_cont;
    style_cont = lv_style_plain;
    style_cont.body.opa = LV_OPA_0;
    style_cont.body.border.opa = LV_OPA_COVER;
    style_cont.body.border.width = 1;
    style_cont.body.border.color = LV_COLOR_RED;
    style_cont.body.radius = 10;
    lv_cont_set_style(contKPaTemp, LV_CONT_STYLE_MAIN, &style_cont);
}

static void Creat_LabelKPaTemp()
{
    LV_IMG_DECLARE(ImgPressure);
    lv_obj_t * imgP = lv_img_create(contKPaTemp, NULL);
    lv_img_set_src(imgP, &ImgPressure);
    lv_obj_align(imgP, NULL, LV_ALIGN_IN_LEFT_MID, 8, -12);
    
    LV_IMG_DECLARE(ImgTemperature);
    lv_obj_t * imgT = lv_img_create(contKPaTemp, NULL);
    lv_img_set_src(imgT, &ImgTemperature);
    lv_obj_align(imgT, NULL, LV_ALIGN_IN_LEFT_MID, 8, 12);
    
    LV_FONT_DECLARE(HandGotn_14);
    LV_FONT_DECLARE(HandGotn_20);
    style_label_public = lv_style_plain_color;
    style_label_public.body.main_color = LV_COLOR_BLACK;
    style_label_public.body.grad_color = LV_COLOR_BLACK;
    style_label_public.text.font = &HandGotn_20;
    style_label_public.text.color = LV_COLOR_WHITE;
    
    static lv_style_t style2_label;
    style2_label = style_label_public;
    style2_label.text.font = &HandGotn_14;
    
    labelKPa = lv_label_create(contKPaTemp, NULL);
    lv_label_set_style(labelKPa, LV_LABEL_STYLE_MAIN, &style_label_public);
    lv_label_set_static_text(labelKPa, "0.00");
    lv_obj_align(labelKPa, imgP, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_auto_realign(labelKPa, true);

    lv_obj_t * label = lv_label_create(contKPaTemp, NULL);
    lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &style2_label);
    lv_label_set_static_text(label, "KPa");
    lv_obj_align(label, NULL, LV_ALIGN_IN_RIGHT_MID, -4, -12);
    
    labelTemp = lv_label_create(contKPaTemp, NULL);
    lv_label_set_style(labelTemp, LV_LABEL_STYLE_MAIN, &style_label_public);
    lv_label_set_static_text(labelTemp, "00.0C"LV_SYMBOL_DEGREE_SIGN);
    lv_obj_align(labelTemp, imgT, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_auto_realign(labelTemp, true);
}

static void lv_chart_get_series_point_min_max(const lv_obj_t * chart, const lv_chart_series_t * series, lv_coord_t * ymin, lv_coord_t * ymax)
{
    uint16_t cnt = lv_chart_get_point_cnt(chart);
    lv_coord_t min = series->points[0];
    lv_coord_t max = series->points[0];
    for(uint16_t i = 0; i < cnt; i++)
    {
        lv_coord_t val = series->points[i];
        if(val < min)
            min = val;
        if(val > max)
            max = val;
    }
    if(ymin != NULL)*ymin = min;
    if(ymax != NULL)*ymax = max;
}

static void ChartAlt_AutoTickProcess(lv_coord_t ymin, lv_coord_t ymax)
{
    static char tick_texts[50];
    lv_coord_t diff = ymax - ymin;
    snprintf(
        tick_texts, sizeof(tick_texts), 
        "%d\n%d\n%d\n%d\n%d\n%d",
        ymax,
        int(ymin + diff * 0.8),
        int(ymin + diff * 0.6),
        int(ymin + diff * 0.4),
        int(ymin + diff * 0.2),
        ymin
    );
        
    lv_chart_set_y_tick_texts(chartAlt, tick_texts, 2, LV_CHART_AXIS_DRAW_LAST_TICK);
}

static void ChartAlt_AutoRangeProcess()
{
    lv_coord_t min, max;
    lv_chart_get_series_point_min_max(chartAlt, serAlt, &min, &max);
    min /= 10;
    max /= 10;
    lv_chart_set_range(chartAlt, min * 10 - 10, max * 10 + 10);
    ChartAlt_AutoTickProcess(min * 10 - 20, max * 10 + 30);
}

static void Task_ChartUpdate(lv_task_t * task)
{
    Task_BMP_Update();
    lv_label_set_text_fmt(labelKPa, "%0.2f", (float)BMP180.pressure / 1000.0f);
    lv_label_set_text_fmt(labelTemp, "%0.1fC"LV_SYMBOL_DEGREE_SIGN, BMP180.temperature);
    lv_label_set_text_fmt(labelAlt, BMP180.altitude > 0.0f ? "%+0.1fm" : "%0.1fm", BMP180.altitude);
    lv_chart_set_next(chartAlt, serAlt, BMP180.altitude);
    ChartAlt_AutoRangeProcess();
}

static void Creat_ChartAlt()
{
    chartAlt = lv_chart_create(appWindow , NULL);
    
    static lv_style_t sytle_chart;
    sytle_chart = *lv_chart_get_style(chartAlt, LV_CHART_STYLE_MAIN);
    sytle_chart.body.main_color = LV_COLOR_BLACK;
    sytle_chart.body.grad_color = LV_COLOR_BLACK;
    sytle_chart.body.opa = LV_OPA_COVER;
    sytle_chart.line.width = 1;
    sytle_chart.line.color = LV_COLOR_GRAY;
    sytle_chart.text.font = &lv_font_roboto_12;
    sytle_chart.text.color = LV_COLOR_GRAY;
    lv_chart_set_style(chartAlt, LV_CHART_STYLE_MAIN, &sytle_chart);
    
    lv_obj_set_opa_scale_enable(chartAlt, true);
    lv_obj_set_opa_scale(chartAlt, LV_OPA_TRANSP);
    
    lv_obj_set_size(chartAlt, 90, 80);
    lv_obj_align(chartAlt, contKPaTemp, LV_ALIGN_OUT_BOTTOM_MID, 15, 20);
    lv_chart_set_update_mode(chartAlt, LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_type(chartAlt, LV_CHART_TYPE_COLUMN);
    lv_chart_set_series_opa(chartAlt, LV_OPA_70);
    lv_chart_set_series_width(chartAlt, 1);
    lv_chart_set_series_darking(chartAlt, LV_OPA_50);
    lv_chart_set_div_line_count(chartAlt, 4, 3);
    lv_chart_set_range(chartAlt, 0, 100);
    lv_chart_set_point_count(chartAlt, lv_obj_get_width(chartAlt));
    lv_chart_set_margin(chartAlt, 50);
    
    lv_chart_set_y_tick_length(chartAlt, LV_CHART_TICK_LENGTH_AUTO, LV_CHART_TICK_LENGTH_AUTO);
    lv_chart_set_y_tick_texts(chartAlt, "m\n" "80\n" "60\n" "40\n" "20\n" "0", 2, LV_CHART_AXIS_DRAW_LAST_TICK);

    serAlt = lv_chart_add_series(chartAlt, LV_COLOR_WHITE);
    lv_chart_init_points(chartAlt, serAlt, 0);
    
    labelAlt = lv_label_create(appWindow, NULL);
    lv_label_set_style(labelAlt, LV_LABEL_STYLE_MAIN, &style_label_public);
    lv_label_set_static_text(labelAlt, "-m");
    lv_obj_align(labelAlt, chartAlt, LV_ALIGN_OUT_BOTTOM_MID, -10, 10);
    lv_obj_set_auto_realign(labelAlt, true);
    lv_obj_set_opa_scale_enable(labelAlt, true);
    lv_obj_set_opa_scale(labelAlt, LV_OPA_TRANSP);
}

static void PageAnimOpen(bool open)
{
    int step = open ? 0 : 1;
    
    int cnt = 2;

    while(cnt--)
    {
        switch(step)
        {
            case 0:
                LV_OBJ_ADD_ANIM(
                    contKPaTemp, opa_scale,
                    open ? LV_OPA_COVER : LV_OPA_TRANSP,
                    LV_ANIM_TIME_DEFAULT
                );
                step = 1;
                break;
            case 1:
                LV_OBJ_ADD_ANIM(
                    chartAlt, opa_scale,
                    open ? LV_OPA_COVER : LV_OPA_TRANSP,
                    LV_ANIM_TIME_DEFAULT
                );
                LV_OBJ_ADD_ANIM(
                    labelAlt, opa_scale,
                    open ? LV_OPA_COVER : LV_OPA_TRANSP,
                    LV_ANIM_TIME_DEFAULT
                );
                step = 0;
                break;
        }
        PageDelay(LV_ANIM_TIME_DEFAULT + 100);
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
    
    Creat_Title("Altitude");
    Creat_ContKPaTemp();
    Creat_LabelKPaTemp();
    Creat_ChartAlt();
    
    taskChartUpdate = lv_task_create(Task_ChartUpdate, 1000, LV_TASK_PRIO_MID, NULL);
    Task_ChartUpdate(taskChartUpdate);
    PageAnimOpen(true);
}

/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit()
{
    PageAnimOpen(false);
    lv_task_del(taskChartUpdate);
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
        if(btn == &btUP)
        {
        }
        if(btn == &btDOWN)
        {
        }
    }
}

/**
  * @brief  页面注册
  * @param  pageID:为此页面分配的ID号
  * @retval 无
  */
void PageRegister_Altitude(uint8_t pageID)
{
    appWindow = AppWindow_GetCont(pageID);
    page.PageRegister(pageID, Setup, NULL, Exit, Event);
}
