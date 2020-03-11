#ifndef __EEPROM_H
#define __EEPROM_H

#include "stm32f4xx.h"

// HACK ALERT. This definition may not match your processor
// To Do. Work out correct value for EEPROM_PAGE_SIZE on the STM32F103CT6 etc
#define MCU_STM32F405RG

#ifndef EEPROM_PAGE_SIZE
#if defined (MCU_STM32F405RG)
#define EEPROM_PAGE_SIZE    (uint16_t)0x800  /* Page size = 2KByte */
#elif defined (MCU_STM32F103ZE) || defined (MCU_STM32F103RE) || defined (MCU_STM32F103RD)
#define EEPROM_PAGE_SIZE    (uint16_t)0x800  /* Page size = 2KByte */
#else
#error  "No MCU type specified. Add something like -DMCU_STM32F103RB to your compiler arguments (probably in a Makefile)."
#endif
#endif

#ifndef EEPROM_START_ADDRESS
#if defined (MCU_STM32F405RG)
#define EEPROM_START_ADDRESS    ((uint32_t)(0x8000000 + 1024 * 1024 - 2 * EEPROM_PAGE_SIZE))
#elif defined (MCU_STM32F103ZE) || defined (MCU_STM32F103RE)
#define EEPROM_START_ADDRESS    ((uint32_t)(0x8000000 + 512 * 1024 - 2 * EEPROM_PAGE_SIZE))
#elif defined (MCU_STM32F103RD)
#define EEPROM_START_ADDRESS    ((uint32_t)(0x8000000 + 384 * 1024 - 2 * EEPROM_PAGE_SIZE))
#else
#error  "No MCU type specified. Add something like -DMCU_STM32F103RB to your compiler arguments (probably in a Makefile)."
#endif
#endif

/* Pages 0 and 1 base and end addresses */
#define EEPROM_PAGE0_BASE       ((uint32_t)(EEPROM_START_ADDRESS + 0x000))
#define EEPROM_PAGE1_BASE       ((uint32_t)(EEPROM_START_ADDRESS + EEPROM_PAGE_SIZE))

/* Page status definitions */
#define EEPROM_ERASED           ((uint16_t)0xFFFF)  /* PAGE is empty */
#define EEPROM_RECEIVE_DATA     ((uint16_t)0xEEEE)  /* PAGE is marked to receive data */
#define EEPROM_VALID_PAGE       ((uint16_t)0x0000)  /* PAGE containing valid data */

/* Page full define */
//enum : uint16_t
//{
//  EEPROM_OK               = ((uint16_t)0x0000),
//  EEPROM_OUT_SIZE         = ((uint16_t)0x0081),
//  EEPROM_BAD_ADDRESS      = ((uint16_t)0x0082),
//  EEPROM_BAD_FLASH        = ((uint16_t)0x0083),
//  EEPROM_NOT_INIT         = ((uint16_t)0x0084),
//  EEPROM_SAME_VALUE       = ((uint16_t)0x0085),
//  EEPROM_NO_VALID_PAGE    = ((uint16_t)0x00AB)
//};
#define EEPROM_OK               ((uint16_t)0x0000)
#define EEPROM_OUT_SIZE         ((uint16_t)0x0081)
#define EEPROM_BAD_ADDRESS      ((uint16_t)0x0082)
#define EEPROM_BAD_FLASH        ((uint16_t)0x0083)
#define EEPROM_NOT_INIT         ((uint16_t)0x0084)
#define EEPROM_SAME_VALUE       ((uint16_t)0x0085)
#define EEPROM_NO_VALID_PAGE    ((uint16_t)0x00AB)

#define EEPROM_DEFAULT_DATA     0xFFFF


class EEPROMClass
{
public:
    EEPROMClass(void);

    uint16_t init(void);
    uint16_t init(uint32_t, uint32_t, uint32_t);

    uint16_t format(void);

    uint16_t erases(uint16_t *);
    uint16_t read (uint16_t address);
    uint16_t read (uint16_t address, uint16_t *data);
    uint16_t write(uint16_t address, uint16_t data);
    uint16_t update(uint16_t address, uint16_t data);
    uint16_t count(uint16_t *);
    uint16_t maxcount(void);

    uint32_t PageBase0;
    uint32_t PageBase1;
    uint32_t PageSize;
    uint16_t Status;
private:
    FLASH_Status EE_ErasePage(uint32_t);

    uint16_t EE_CheckPage(uint32_t, uint16_t);
    uint16_t EE_CheckErasePage(uint32_t, uint16_t);
    uint16_t EE_Format(void);
    uint32_t EE_FindValidPage(void);
    uint16_t EE_GetVariablesCount(uint32_t, uint16_t);
    uint16_t EE_PageTransfer(uint32_t, uint32_t, uint16_t);
    uint16_t EE_VerifyPageFullWriteVariable(uint16_t, uint16_t);
};

extern EEPROMClass EEPROM;

#endif  /* __EEPROM_H */
