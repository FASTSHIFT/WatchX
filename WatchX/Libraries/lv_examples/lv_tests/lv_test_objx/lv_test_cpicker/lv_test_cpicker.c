/**
 * @file lv_test_cpicker.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_test_cpicker.h"

#if LV_USE_CPICKER && LV_USE_TESTS

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Create a color picker to test its basic functionalities
 */
void lv_test_cpicker_1(void)
{
    const lv_coord_t pickerSize = 200;
    /* Set the style of the color ring */
    static lv_style_t styleMain;
    lv_style_copy(&styleMain, &lv_style_plain);
    styleMain.line.width = 30;
    /* Make the background white */
    styleMain.body.main_color = styleMain.body.grad_color = LV_COLOR_WHITE;
    /* Set the style of the knob */
    static lv_style_t styleIndicator;
    lv_style_copy(&styleIndicator, &lv_style_pretty);
    styleIndicator.body.border.color = LV_COLOR_WHITE;
    /* Ensure that the knob is fully opaque */
    styleIndicator.body.opa = LV_OPA_COVER;
    styleIndicator.body.border.opa = LV_OPA_COVER;
    lv_obj_t * scr = lv_scr_act();
    lv_obj_t * colorPicker = lv_cpicker_create(scr, NULL);
    lv_obj_set_size(colorPicker, pickerSize, pickerSize);
    /* Choose the 'DISC' type */
    lv_cpicker_set_type(colorPicker, LV_CPICKER_TYPE_DISC);
    lv_obj_align(colorPicker, NULL, LV_ALIGN_CENTER, 0, 0);
    /* Set the styles */
    lv_cpicker_set_style(colorPicker, LV_CPICKER_STYLE_MAIN, &styleMain);
    lv_cpicker_set_style(colorPicker, LV_CPICKER_STYLE_INDICATOR, &styleIndicator);
    /* Change the knob's color to that of the selected color */
    lv_cpicker_set_indic_colored(colorPicker, true);

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif /*LV_USE_CPICKER && LV_USE_TESTS*/
