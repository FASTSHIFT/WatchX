#include "GUI/DisplayPrivate.h"
#include "Basic/CommonMacro.h"
#include "BSP.h"
#include "rtc.h"

/**
  * @brief  背光亮度渐变，受lv_anim控制
  * @param  obj:无用
  * @param  brightness:亮度值
  * @retval None
  */
static void Backlight_AnimCallback(void * obj, int16_t brightness)
{
    Backlight_SetValue(brightness);
}

/**
  * @brief  背光初始化
  * @param  无
  * @retval 无
  */
void Backlight_Init()
{
    /*PWM初始化，1000级，20K频率*/
    PWM_Init(TFT_BLK_Pin, 1000, 20000);
}

/**
  * @brief  背光设置，渐变效果
  * @param  target:目标亮度(0~1000 -> 0~100%)
  * @retval 无
  */
void Backlight_SetGradual(uint16_t target, uint16_t time)
{
    static lv_anim_t a;
    lv_obj_add_anim(
        NULL, &a,
        (lv_anim_exec_xcb_t)Backlight_AnimCallback,
        Backlight_GetValue(), target,
        time
    );
}

/**
  * @brief  获取背光亮度
  * @param  无
  * @retval 当前亮度(0~1000 -> 0~100%)
  */
uint16_t Backlight_GetValue()
{
    return timer_get_compare(PIN_MAP[TFT_BLK_Pin].TIMx, PIN_MAP[TFT_BLK_Pin].TimerChannel);
}

/**
  * @brief  设置背光亮度
  * @param  val: 亮度(0~1000 -> 0~100%)
  * @retval 无
  */
void Backlight_SetValue(int16_t val)
{
    __LimitValue(val, 0, 1000);
    analogWrite(TFT_BLK_Pin, val);
}

/**
  * @brief  从后备寄存器获取掉电保存的背光亮度
  * @param  无
  * @retval 亮度(0~1000 -> 0~100%)
  */
uint16_t Backlight_GetBKP()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);   //使能PWR和BKP外设时钟
    PWR_BackupAccessCmd(ENABLE);                          //使能后备寄存器访问
    
    uint16_t value = RTC_ReadBackupRegister(RTC_BKP_DR2);
    
    /*亮度值是否非法*/
    if(value == 0 || value > 1000)
    {
        /*设置默认亮度*/
        value = 1000;
    }
    
    return value;
}

/**
  * @brief  将当前亮度更新至后备寄存器
  * @param  无
  * @retval 无
  */
void Backlight_UpdateBKP()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);         //使能PWR和BKP外设时钟
    PWR_BackupAccessCmd(ENABLE);                                //使能后备寄存器访问
    RTC_WaitForSynchro();                                       //等待最近一次对RTC寄存器的写操作完成
    RTC_WriteBackupRegister(RTC_BKP_DR2, Backlight_GetValue()); //将读取的亮度写入后备寄存器
    RTC_WaitForSynchro();                                       //等待最近一次对RTC寄存器的写操作完成
}

