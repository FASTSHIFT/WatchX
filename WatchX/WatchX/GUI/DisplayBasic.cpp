#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "BSP/BSP.h"

SCREEN_CLASS screen(TFT_CS_Pin, TFT_DC_Pin, TFT_RST_Pin);

PageManager page(PAGE_MAX);

#define PAGE_REG(name)\
do{\
    extern void PageRegister_##name(uint8_t pageID);\
    PageRegister_##name(PAGE_##name);\
}while(0)

static void Pages_Init()
{
    PAGE_REG(Home);
    PAGE_REG(Settings);
    PAGE_REG(TimeCfg);
    PAGE_REG(Backlight);
    PAGE_REG(StopWatch);
    PAGE_REG(Altitude);
    PAGE_REG(About);
    PAGE_REG(Game);
    
    page.PagePush(PAGE_Home);
}

void Display_Init()
{
    Backlight_SetValue(0);
    
    screen.begin();
    screen.setRotation(0);
    screen.fillScreen(screen.Black);
    
    DisplayError_Init();
    
    lv_port_disp_init();
    lv_theme_set_current(lv_theme_night_init(200, NULL));
    
    Creat_AppWindow();
    Pages_Init();
    
    Backlight_SetGradual(1000, 1000);
}

void Task_Display()
{
    lv_task_handler();
    page.Running();
}

void PageDelay(uint32_t ms)
{
    uint32_t lastTime = millis();
    while(millis() - lastTime <= ms)
    {
        lv_task_handler();
    }
}

//static void DisplayFPSTest()
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
