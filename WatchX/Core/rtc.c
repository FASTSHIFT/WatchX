#include "rtc.h"
#include "delay.h"

/**
  * @brief  RTC时间设置
  * @param  hour:小时
  * @param  min:分钟
  * @param  sec:秒钟
  * @param  ampm:@RTC_AM_PM_Definitions  :RTC_H12_AM/RTC_H12_PM
  * @retval SUCCESS(1),成功 ERROR(0),进入初始化模式失败
  */
ErrorStatus RTCx_SetTime(uint8_t hour, uint8_t min, uint8_t sec, uint8_t ampm)
{
    RTC_TimeTypeDef RTC_TimeTypeInitStructure;

    RTC_TimeTypeInitStructure.RTC_Hours = hour;
    RTC_TimeTypeInitStructure.RTC_Minutes = min;
    RTC_TimeTypeInitStructure.RTC_Seconds = sec;
    RTC_TimeTypeInitStructure.RTC_H12 = ampm;

    return RTC_SetTime(RTC_Format_BIN, &RTC_TimeTypeInitStructure);
}

/**
  * @brief  RTC日期设置
  * @param  year:年(0~99)
  * @param  month:月(1~12)
  * @param  date:日(0~31)
  * @param  week:星期(1~7,0:非法!)
  * @retval SUCCESS(1),成功 ERROR(0),进入初始化模式失败
  */
ErrorStatus RTCx_SetDate(uint8_t year, uint8_t month, uint8_t date, uint8_t week)
{
    RTC_DateTypeDef RTC_DateTypeInitStructure;
    RTC_DateTypeInitStructure.RTC_Date = date;
    RTC_DateTypeInitStructure.RTC_Month = month;
    RTC_DateTypeInitStructure.RTC_WeekDay = week;
    RTC_DateTypeInitStructure.RTC_Year = year;
    return RTC_SetDate(RTC_Format_BIN, &RTC_DateTypeInitStructure);
}

/**
  * @brief  日期换算星期(1~7)
  * @param  RTC_DateStruct:日期结构体地址
  * @retval 无
  */
void RTCx_GetWeekDay(RTC_DateTypeDef *RTC_DateStruct)
{
    int year = RTC_DateStruct->RTC_Year + 2000;
    int month = RTC_DateStruct->RTC_Month;
    int date = RTC_DateStruct->RTC_Date;
    if(month == 1) month = 13, year--;
    if(month == 2) month = 14, year--;
    int weekdey = (date + 2 * month + 3 * (month + 1) / 5 + year + year / 4 - year / 100 + year / 400) % 7;
    RTC_DateStruct->RTC_WeekDay = weekdey + 1;
}

/**
  * @brief  RTC日期设置
  * @param  无
  * @retval 0:初始化成功; 1:LSE开启失败; 2:进入初始化模式失败;
  */
void RTCx_Init(void)
{
    RTC_InitTypeDef RTC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟
    PWR_BackupAccessCmd(ENABLE);    //使能后备寄存器访问

    if(RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5050)   //是否第一次配置?
    {
        RCC_LSEConfig(RCC_LSE_ON);//LSE 开启
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET); //检查指定的RCC标志位设置与否,等待低速晶振就绪

        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);     //设置RTC时钟(RTCCLK),选择LSE作为RTC时钟
        RCC_RTCCLKCmd(ENABLE);  //使能RTC时钟

        RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC异步分频系数(1~0X7F)
        RTC_InitStructure.RTC_SynchPrediv  = 0xFF;//RTC同步分频系数(0~7FFF)
        RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//RTC设置为,24小时格式
        RTC_Init(&RTC_InitStructure);

        RTCx_SetTime(00, 00, 00, RTC_H12_AM); //设置时间
        RTCx_SetDate(00, 1, 1, 1);  //设置日期

        RTC_WriteBackupRegister(RTC_BKP_DR0, 0x5050);   //标记已经初始化过了
    }
}

/**
  * @brief  设置闹钟时间(按星期闹铃,24小时制)
  * @param  week:星期几(1~7) @ref  RTC_Alarm_Definitions
  * @param  hour:小时
  * @param  min:分钟
  * @param  sec:秒钟
  * @retval 0:初始化成功; 1:LSE开启失败; 2:进入初始化模式失败;
  */
void RTCx_SetAlarmA(uint8_t week, uint8_t hour, uint8_t min, uint8_t sec)
{
    RTC_AlarmTypeDef RTC_AlarmTypeInitStructure;
    RTC_TimeTypeDef RTC_TimeTypeInitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    RTC_AlarmCmd(RTC_Alarm_A, DISABLE); //关闭闹钟A

    RTC_TimeTypeInitStructure.RTC_Hours = hour; //小时
    RTC_TimeTypeInitStructure.RTC_Minutes = min; //分钟
    RTC_TimeTypeInitStructure.RTC_Seconds = sec; //秒
    RTC_TimeTypeInitStructure.RTC_H12 = RTC_H12_AM;

    RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDay = week; //星期
    RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay; //按星期闹
    RTC_AlarmTypeInitStructure.RTC_AlarmMask = RTC_AlarmMask_None; //精确匹配星期，时分秒
    RTC_AlarmTypeInitStructure.RTC_AlarmTime = RTC_TimeTypeInitStructure;
    RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmTypeInitStructure);

    RTC_ClearITPendingBit(RTC_IT_ALRA);//清除RTC闹钟A的标志
    RTC_ITConfig(RTC_IT_ALRA, ENABLE); //开启闹钟A中断
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE); //开启闹钟A

    NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);//配置
}

/**
  * @brief  周期性唤醒定时器设置
  * @param  wksel:  @ref RTC_Wakeup_Timer_Definitions
  * @param  cnt:自动重装载值.减到0,产生中断.
  * @retval 无
  */
void RTCx_SetWakeUp(uint32_t wksel, uint16_t cnt)
{
    NVIC_InitTypeDef  NVIC_InitStructure;

    RTC_WakeUpCmd(DISABLE);//关闭WAKE UP
    RTC_WakeUpClockConfig(wksel);//唤醒时钟选择
    RTC_SetWakeUpCounter(cnt);//设置WAKE UP自动重装载寄存器
    RTC_ClearITPendingBit(RTC_IT_WUT); //清除RTC WAKE UP的标志

    RTC_ITConfig(RTC_IT_WUT, ENABLE); //开启WAKE UP 定时器中断
    RTC_WakeUpCmd( ENABLE);//开启WAKE UP 定时器　

    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);//配置
}

/**
  * @brief  RTC闹钟中断服务函数
  * @param  无
  * @retval 无
  */
void RTC_Alarm_IRQHandler(void)
{
    if(RTC_GetFlagStatus(RTC_FLAG_ALRAF) == SET) //ALARM A中断
    {
        RTC_ClearFlag(RTC_FLAG_ALRAF);//清除中断标志
    }
}

/**
  * @brief  RTC WAKE UP中断服务函数
  * @param  无
  * @retval 无
  */
void RTC_WKUP_IRQHandler(void)
{
    if(RTC_GetFlagStatus(RTC_FLAG_WUTF) == SET) //WK_UP中断
    {
        RTC_ClearFlag(RTC_FLAG_WUTF);   //清除中断标志
    }
}
