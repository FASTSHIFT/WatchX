#include "GUI/DisplayPrivate.h"
#include "cm_backtrace/cm_backtrace.h"
#include "Basic/FileGroup.h"
#include "BSP/BSP.h"
#include "SoftSerial/SoftSerial.h"

static void SoftDelay(uint32_t ms)
{
    volatile uint32_t i = F_CPU / 1000 * ms / 5;
    while(i--);
}

#include "Adafruit_GFX_Library/Fonts\FreeMono12pt7b.h"
static void System_CrashReports(const char* report)
{
    screen.fillScreen(screen.Blue);
    screen.setTextColor(screen.White);
    screen.setFont(&FreeMono12pt7b);
    screen.setTextSize(2);
    screen.setCursor(0, 34);
    screen.print(":(");

    screen.setFont();

    screen.setTextSize(1);
    screen.setCursor(0, ScreenMid_H - TEXT_HEIGHT_1 - 5);
    screen.println(report);
    screen.print("Press 'OK' to reboot..");

    screen.setCursor(0, screen.height() - TEXT_HEIGHT_1 * 6);
    screen.println("Error code:");
    screen.printf("MMFAR = 0x%08X\r\n", SCB->MMFAR);
    screen.printf("BFAR  = 0x%08X\r\n", SCB->BFAR);
    screen.printf("CFSR  = 0x%08X\r\n", SCB->CFSR);
    screen.printf("HFSR  = 0x%08X\r\n", SCB->HFSR);
    screen.printf("DFSR  = 0x%08X\r\n", SCB->DFSR);
}

void DisplayError_Init()
{
    cm_backtrace_init(WX_NAME, WX_HARDWARE_VERSION, WX_SOFTWARE_VERSION" "__DATE__);
    
    screen.setTextWrap(true);
    screen.setTextSize(1);
    screen.setCursor(0, 0);
    screen.setFont();
    screen.setTextColor(screen.White, screen.Blue);
}

static void SoftSerial_Print(const char* str)
{
    while (*str)
    {
        SoftSerial_Write(LED_Pin, 768, *str++);
    }
}

void cmb_printf_hook(const char *__restrict __format, ...)
{
    char printf_buff[256];

    va_list args;
    va_start(args, __format);
    int ret_status = vsnprintf(printf_buff, sizeof(printf_buff), __format, args);
    va_end(args);
    
    SoftSerial_Print(printf_buff);
}

/***************************** HardFault_Handler *******************************/
extern "C"
{
    void vApplicationHardFaultHook()
    {
        System_CrashReports("FXXK! hardfault!");
        while(digitalRead(KEY_OK_Pin) == HIGH)
        {
            SoftDelay(1000);
            togglePin(LED_Pin);
        }
        NVIC_SystemReset();
    }
    __asm void HardFault_Handler()
    {
        extern vApplicationHardFaultHook
        extern cm_backtrace_fault
            
        mov r0, lr
        mov r1, sp
        bl cm_backtrace_fault
        bl vApplicationHardFaultHook
Fault_Loop
        b Fault_Loop
    }
}
