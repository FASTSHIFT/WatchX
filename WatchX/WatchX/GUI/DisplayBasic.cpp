#include "GUI/DisplayPrivate.h"
#include "BSP/BSP.h"

/*实例化屏幕对象*/
SCREEN_CLASS screen(TFT_CS_Pin, TFT_DC_Pin, TFT_RST_Pin);

/*实例化页面调度器*/
PageManager page(PAGE_MAX);

/*页面注册器*/
#define PAGE_REG(name)\
do{\
    extern void PageRegister_##name(uint8_t pageID);\
    PageRegister_##name(PAGE_##name);\
}while(0)

/**
  * @brief  页面初始化
  * @param  无
  * @retval 无
  */
static void Pages_Init()
{
    PAGE_REG(DialPlate);    //表盘
    PAGE_REG(MainMenu);     //主菜单
    PAGE_REG(TimeCfg);      //时间设置
    PAGE_REG(Backlight);    //背光设置
    PAGE_REG(StopWatch);    //秒表
    PAGE_REG(Altitude);     //海拔表
    PAGE_REG(About);        //关于
    PAGE_REG(Game);         //游戏
    
    page.PagePush(PAGE_DialPlate);//打开表盘
}

/**
  * @brief  显示初始化
  * @param  无
  * @retval 无
  */
void Display_Init()
{
    /*背光关闭*/
    Backlight_SetValue(0);
    
    /*屏幕初始化*/
    screen.begin();
    screen.setRotation(0);
    screen.fillScreen(screen.Black);
    
    /*自动报错初始化*/
    DisplayError_Init();
    
    /*lvgl初始化*/
    lv_init();
    lv_port_disp_init();
    lv_theme_set_current(lv_theme_night_init(200, NULL));
    
    /*APP窗口初始化*/
    AppWindow_Create();
    
    /*页面初始化*/
    Pages_Init();
    
    /*背光渐亮*/
    Backlight_SetGradual(Backlight_GetBKP(), 1000);
}

/**
  * @brief  显示更新
  * @param  无
  * @retval 无
  */
void Display_Update()
{
    lv_task_handler();
    page.Running();
}

/**
  * @brief  页面阻塞延时，保持lvgl更新
  * @param  无
  * @retval 无
  */
void PageDelay(uint32_t ms)
{
    uint32_t lastTime = millis();
    while(millis() - lastTime <= ms)
    {
        lv_task_handler();
    }
}

//static void Display_FPSTest()
//{
//    float Ftime, Ltime;
//    float FPS;
//    Ftime = millis();
//    int i;
//    for(i = 100; i > 0; i--)
//    {
//        screen.fillScreen(screen.Black);
//        screen.fillScreen(screen.White);
//    }
//    Ltime = millis() - Ftime;
//    FPS = 100.0f / (Ltime / 1000.0f) * 2.0f;
//    screen.fillScreen(screen.Blue);
//    screen.setTextSize(1);
//    screen.setTextColor(screen.White);
//    screen.setCursor(0, 0);
//    screen.print(Ltime);
//    screen.setCursor(45, 0);
//    screen.print("ms");
//    screen.setCursor(0, 9);
//    screen.print("FPS:");
//    screen.setCursor(25, 9);
//    screen.print(FPS);
//    while(1);
//}
