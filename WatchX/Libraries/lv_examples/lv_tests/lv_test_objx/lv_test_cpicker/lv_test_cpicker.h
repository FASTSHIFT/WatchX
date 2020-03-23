/**
 * @file lv_test_cpicker.h
 *
 */

#ifndef LV_TEST_CPICKER_H
#define LV_TEST_CPICKER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lvgl.h"
#include "lv_ex_conf.h"
#else
#include "../../../../lvgl/lvgl.h"
#include "../../../../lv_ex_conf.h"
#endif

#if LV_USE_CPICKER && LV_USE_TESTS

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a color picker to test its basic functionalities
 */
void lv_test_cpicker_1(void);


/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_CONT && LV_USE_TESTS*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_TEST_CONT_H*/
