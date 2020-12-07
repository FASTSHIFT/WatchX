# WatchX 更新日志
> https://github.com/FASTSHIFT/WatchX

## [v0.1] - 2020-03-07
* 1.框架搭建完成，UI基于littlevGL v6.1.2
* 2.优化屏幕驱动，刷新率提升至104FPS

## [v0.2] - 2020-03-11
* 1.主表盘界面完成
* 2.设置界面完成
* 3.时间设置界面完成

## [v0.3] - 2020-03-12
* 1.秒表完成
* 2.海拔表完成
* 3.About完成
* 4.所有功能基本完工

## [v0.4] - 2020-03-14
* 1.添加LV_OBJ_ADD_ANIM，更简洁的动画绑定

## [v0.5] - 2020-03-18
* 1.添加低功耗模式
* 2.添加Arduboy Game System
* 3.添加游戏 "CHRI-BOCCHI CAT"

## [v0.6] - 2020-03-23
* 1.海拔显示添加自动量程
* 2.添加MillisTaskManager，可统计CPU占用率
* 3.添加时间上下滑动特效

## [v0.7] - 2020-05-03
* 1.屏幕使用DMA发送，配合lvgl乒乓缓冲，画面无撕裂
* 2.Brightness更改为Backlight

## [v0.8] - 2020-06-27
* 1.修复电量显示的问题
* 2.添加 电源使能保持脚POWER_ON 和 电源使能按键状态读取脚POWER_EN
* 3.添加屏幕亮度掉电保存支持
* 4.添加自动待机时间设置以及掉电保存支持
* 5.感谢"日天电子工作室"提出的改进建议
* 6.修改了一些函数的命名

## [v0.9] - 2020-07-30
* 1.更新MillisTaskManager库，无需手动指定ID
* 2.TFT_LED_Pin 更名为 TFT_BLK_Pin
* 3.添加部分注释
* 4.Home更名为DialPlate(表盘)页面，Settings更名为MainMenu页面
* 5.更新PageManager v1.4
* 6.添加Arduboy2

## [v1.0] - 2020-12-07
* 1.POWER初始化整合
* 2.修改默认的动画曲线，更强的非线性效果，lv_anim_path_ease_in_out -> lv_anim_path_ease_out
* 3.修正错误的单词拼写，xxx_Creat -> xxx_Create， Fouce -> Focus
* 4.加大lv_disp_buf，使用两个半屏buffer，提升刷新率
* 5.去除FileGroup.h
* 6.更新海拔显示的tick自适应方法
