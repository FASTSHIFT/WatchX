#ifndef __MCU_TYPE
#define __MCU_TYPE

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#define __STM32F4__
#define F_CPU SystemCoreClock
#define CYCLES_PER_MICROSECOND (F_CPU / 1000000U)

#define __KEILDUINO__ 290

#define GPIO_HIGH(GPIOX,GPIO_PIN_X)    (GPIOX->BSRRL = GPIO_PIN_X)
#define GPIO_LOW(GPIOX,GPIO_PIN_X)     (GPIOX->BSRRH = GPIO_PIN_X)
#define GPIO_READ(GPIOX,GPIO_PIN_X)   ((GPIOX->IDR   & GPIO_PIN_X)!=0)
#define GPIO_TOGGLE(GPIOX,GPIO_PIN_X)  (GPIOX->ODR  ^= GPIO_PIN_X)

#define digitalWrite_HIGH(Pin) (GPIO_HIGH  (PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))
#define digitalWrite_LOW(Pin)  (GPIO_LOW   (PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))
#define digitalRead_FAST(Pin)  (GPIO_READ  (PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))
#define togglePin(Pin)         (GPIO_TOGGLE(PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))

#endif
