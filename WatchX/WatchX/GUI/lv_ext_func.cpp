#include "DisplayPrivate.h"

/**
  * @brief  安全删除对象，当对象为NULL的时候不执行删除
  * @param  obj:对象二级地址
  * @retval true 成功; false 失败
  */
bool lv_obj_del_safe(lv_obj_t** obj)
{
    bool ret = false;
    if(*obj != NULL)
    {
        lv_obj_del(*obj);
        *obj = NULL;
        ret = true;
    }
    return ret;
}

/**
  * @brief  在label后追加字符串
  * @param  label:被追加的对象
  * @param  text:追加的字符串
  * @retval 无
  */
void lv_label_set_text_add(lv_obj_t * label, const char * text)
{
    if(!label)
        return;
    
    lv_label_ins_text(label, strlen(lv_label_get_text(label)), text);
}

/**
  * @brief  获取对象的X中心坐标
  * @param  obj:对象地址
  * @retval X中心坐标
  */
lv_coord_t lv_obj_get_x_center(lv_obj_t * obj)
{
    return (obj->coords.x2 + obj->coords.x1) / 2; 
    //return (lv_obj_get_x(obj) + lv_obj_get_width(obj) / 2);
}

/**
  * @brief  获取对象的Y中心坐标
  * @param  obj:对象地址
  * @retval Y中心坐标
  */
lv_coord_t lv_obj_get_y_center(lv_obj_t * obj)
{
    return (obj->coords.y2 + obj->coords.y1) / 2; 
    //return (lv_obj_get_y(obj) + lv_obj_get_height(obj) / 2);
}

/**
  * @brief  设置对象的颜色(尽量别用，可能会影响到默认的style)
  * @param  obj:对象地址
  * @param  color:颜色
  * @retval 无
  */
void lv_obj_set_color(lv_obj_t * obj, lv_color_t color)
{
    lv_style_t * style = (lv_style_t*)lv_obj_get_style(obj);
    style->body.grad_color = style->body.main_color = color;
    lv_obj_set_style(obj, style);
}

/**
  * @brief  设置table对象的所有成员的对齐方式
  * @param  table:对象地址
  * @param  align:对其方式
  * @retval 无
  */
void lv_table_set_align(lv_obj_t * table, lv_label_align_t align)
{
    uint16_t col = lv_table_get_col_cnt(table);
    uint16_t row = lv_table_get_row_cnt(table);
    for(uint16_t i = 0; i < col; i++)
    {
        for(uint16_t j = 0; j < row; j++)
        {
            lv_table_set_cell_align(table, j, i, align);
        }
    }
}

/**
  * @brief  获取win对象的标题label
  * @param  win:对象地址
  * @retval label地址
  */
lv_obj_t * lv_win_get_label(lv_obj_t * win)
{
    lv_win_ext_t * ext = (lv_win_ext_t*)lv_obj_get_ext_attr(win);
    return ext->title;
}

/**
  * @brief  为对象添加动画
  * @param  obj:对象地址
  * @param  a:动画控制器地址
  * @param  exec_cb:控制对象属性的函数地址
  * @param  start:动画的开始值
  * @param  end:动画的结束值
  * @param  time:动画的执行时间
  * @param  ready_cb:动画结束事件回调
  * @param  path_cb:动画曲线
  * @retval 无
  */
void lv_obj_add_anim(
    lv_obj_t * obj, lv_anim_t * a,
    lv_anim_exec_xcb_t exec_cb, 
    int32_t start, int32_t end,    
    uint16_t time,
    lv_anim_ready_cb_t ready_cb,
    lv_anim_path_cb_t path_cb
)
{
    lv_anim_t a_tmp;
    if(a == NULL)
    {
        lv_anim_init(&a_tmp);
        a = &a_tmp;
    }

    a->var = obj;
    a->start = start;
    a->end = end;
    a->exec_cb = exec_cb;
    a->path_cb = path_cb;
    a->ready_cb = ready_cb;
    a->time = time;
    lv_anim_create(a);
}
