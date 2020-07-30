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
* 2.TFT_BLK_Pin 更名为 TFT_BLK_Pin
* 3.添加部分注释
* 4.Home更名为DiolPlate(表盘)页面，Settings更名为MainMenu页面
* 5.更新PageManager v1.4
* 6.添加Arduboy2
