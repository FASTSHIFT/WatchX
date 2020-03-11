/* -----------------------------------------------------------------------------
 * Copyright (c) 2014 - 2017 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software. Permission is granted to anyone to use this
 * software for any purpose, including commercial applications, and to alter
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *
 * $Date:        02. November 2017
 * $Revision:    V1.1.0
 *
 * Project:      Flash Device Description for STMicroelectronics STM32F4xx Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.1.0
 *    Added STM32F4xx_1024dual Algorithm
 *  Version 1.0.2
 *    Added more Option Byte Algorithm
 *  Version 1.0.1
 *    Added STM32F411xx Option Byte Algorithm
 *  Version 1.0.0
 *    Initial release
 */

#include "FlashOS.h"        // FlashOS Structures

#ifdef FLASH_MEM

#ifdef STM32F4xx_128
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,             // Driver Version, do not modify!
    "STM32F4xx 128kB Flash",    // Device Name (128kB)
    ONCHIP,                     // Device Type
    0x08000000,                 // Device Start Address
    0x00020000,                 // Device Size in Bytes (128kB)
    1024,                       // Programming Page Size
    0,                          // Reserved, must be 0
    0xFF,                       // Initial Content of Erased Memory
    100,                        // Program Page Timeout 100 mSec
    6000,                       // Erase Sector Timeout 6000 mSec
    // Specify Size and Address of Sectors
    0x04000, 0x000000,          // Sector Size  16kB (4 Sectors)
    0x10000, 0x010000,          // Sector Size  64kB (1 Sectors)
    SECTOR_END
  };
#endif // STM32F2xx_128

#ifdef STM32F4xx_256
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,             // Driver Version, do not modify!
    "STM32F4xx 256kB Flash",    // Device Name (256kB)
    ONCHIP,                     // Device Type
    0x08000000,                 // Device Start Address
    0x00040000,                 // Device Size in Bytes (256kB)
    1024,                       // Programming Page Size
    0,                          // Reserved, must be 0
    0xFF,                       // Initial Content of Erased Memory
    100,                        // Program Page Timeout 100 mSec
    6000,                       // Erase Sector Timeout 6000 mSec
    // Specify Size and Address of Sectors
    0x04000, 0x000000,          // Sector Size  16kB (4 Sectors)
    0x10000, 0x010000,          // Sector Size  64kB (1 Sectors)
    0x20000, 0x020000,          // Sector Size 128kB (1 Sectors)
    SECTOR_END
  };
#endif // STM32F2xx_256

#ifdef STM32F4xx_384
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,             // Driver Version, do not modify!
    "STM32F4xx 384kB Flash",    // Device Name (384kB)
    ONCHIP,                     // Device Type
    0x08000000,                 // Device Start Address
    0x00060000,                 // Device Size in Bytes (384kB)
    1024,                       // Programming Page Size
    0,                          // Reserved, must be 0
    0xFF,                       // Initial Content of Erased Memory
    100,                        // Program Page Timeout 100 mSec
    6000,                       // Erase Sector Timeout 6000 mSec
    // Specify Size and Address of Sectors
    0x04000, 0x000000,          // Sector Size  16kB (4 Sectors)
    0x10000, 0x010000,          // Sector Size  64kB (1 Sectors)
    0x20000, 0x020000,          // Sector Size 128kB (2 Sectors)
    SECTOR_END
  };
#endif // STM32F2xx_384

#ifdef STM32F4xx_512
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,             // Driver Version, do not modify!
    "STM32F4xx 512kB Flash",    // Device Name (512kB)
    ONCHIP,                     // Device Type
    0x08000000,                 // Device Start Address
    0x00080000,                 // Device Size in Bytes (512kB)
    1024,                       // Programming Page Size
    0,                          // Reserved, must be 0
    0xFF,                       // Initial Content of Erased Memory
    100,                        // Program Page Timeout 100 mSec
    6000,                       // Erase Sector Timeout 6000 mSec
    // Specify Size and Address of Sectors
    0x04000, 0x000000,          // Sector Size  16kB (4 Sectors)
    0x10000, 0x010000,          // Sector Size  64kB (1 Sectors)
    0x20000, 0x020000,          // Sector Size 128kB (3 Sectors)
    SECTOR_END
  };
#endif // STM32F2xx_512

#ifdef STM32F4xx_1024
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,             // Driver Version, do not modify!
    "STM32F4xx 1MB Flash",      // Device Name (1024kB/512kB)
    ONCHIP,                     // Device Type
    0x08000000,                 // Device Start Address
    0x00100000,                 // Device Size in Bytes (1024kB)
    1024,                       // Programming Page Size
    0,                          // Reserved, must be 0
    0xFF,                       // Initial Content of Erased Memory
    100,                        // Program Page Timeout 100 mSec
    6000,                       // Erase Sector Timeout 6000 mSec
    // Specify Size and Address of Sectors
    0x04000, 0x000000,          // Sector Size  16kB (4 Sectors)
    0x10000, 0x010000,          // Sector Size  64kB (1 Sectors)
    0x20000, 0x020000,          // Sector Size 128kB (7/3 Sectors)
    SECTOR_END
  };
#endif // STM32F2xx_1024

#if defined STM32F4xx_1024dual
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,             // Driver Version, do not modify!
    "STM32F4xx 1MB dual bank Flash",     // Device Name
    ONCHIP,                     // Device Type
    0x08000000,                 // Device Start Address
    0x00100000,                 // Device Size in Bytes (1024kB)
    1024,                       // Programming Page Size
    0,                          // Reserved, must be 0
    0xFF,                       // Initial Content of Erased Memory
    1000,                       // Program Page Timeout 1000 mSec
    6000,                       // Erase Sector Timeout 6000 mSec
    // Specify Size and Address of Sectors
    0x04000, 0x000000,          // Sector Size  16kB (4 Sectors)
    0x10000, 0x010000,          // Sector Size  64kB (1 Sectors)
    0x20000, 0x020000,          // Sector Size 128kB (3 Sectors)
    0x04000, 0x080000,          // Sector Size  16kB (4 Sectors)
    0x10000, 0x090000,          // Sector Size  64kB (1 Sectors)
    0x20000, 0x0A0000,          // Sector Size 128kB (3 Sectors)
    SECTOR_END
  };
#endif // STM32F7x_1024dual

#ifdef STM32F4xx_2048
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,             // Driver Version, do not modify!
    "STM32F4xx 2MB Flash",      // Device Name (2048kB)
    ONCHIP,                     // Device Type
    0x08000000,                 // Device Start Address
    0x00200000,                 // Device Size in Bytes (2048kB)
    1024,                       // Programming Page Size
    0,                          // Reserved, must be 0
    0xFF,                       // Initial Content of Erased Memory
    100,                        // Program Page Timeout 100 mSec
    6000,                       // Erase Sector Timeout 6000 mSec
    // Specify Size and Address of Sectors
    0x04000, 0x000000,          // Sector Size  16kB (4 Sectors)
    0x10000, 0x010000,          // Sector Size  64kB (1 Sectors)
    0x20000, 0x020000,          // Sector Size 128kB (7 Sectors)
    0x04000, 0x100000,          // Sector Size  16kB (4 Sectors)
    0x10000, 0x110000,          // Sector Size  64kB (1 Sectors)
    0x20000, 0x120000,          // Sector Size 128kB (7 Sectors)
    SECTOR_END
  };
#endif // STM32F2xx_2048

#endif // FLASH_MEM


#ifdef FLASH_OPT

#if   defined STM32F401xx
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,             // Driver Version, do not modify!
    "STM32F401xx Flash Options",  // Device Name
    ONCHIP,                     // Device Type
    0x1FFFC000,                 // Device Start Address
    0x00000004,                 // Device Size in Bytes (4)
    4,                          // Programming Page Size
    0,                          // Reserved, must be 0
    0xFF,                       // Initial Content of Erased Memory
    3000,                       // Program Page Timeout 3 Sec
    3000,                       // Erase Sector Timeout 3 Sec
    // Specify Size and Address of Sectors
    0x0004, 0x000000,           // Sector Size 4B
    SECTOR_END
  };

#elif defined STM32F40xxx_41xxx
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,             // Driver Version, do not modify!
    "STM32F40xxx/41xxx Flash Options",  // Device Name
    ONCHIP,                     // Device Type
    0x1FFFC000,                 // Device Start Address
    0x00000004,                 // Device Size in Bytes (4)
    4,                          // Programming Page Size
    0,                          // Reserved, must be 0
    0xFF,                       // Initial Content of Erased Memory
    3000,                       // Program Page Timeout 3 Sec
    3000,                       // Erase Sector Timeout 3 Sec
    // Specify Size and Address of Sectors
    0x0004, 0x000000,           // Sector Size 4B
    SECTOR_END
  };

#elif defined STM32F411xx
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,             // Driver Version, do not modify!
    "STM32F411xx Flash Options",  // Device Name
    ONCHIP,                     // Device Type
    0x1FFFC000,                 // Device Start Address
    0x00000004,                 // Device Size in Bytes (4)
    4,                          // Programming Page Size
    0,                          // Reserved, must be 0
    0xFF,                       // Initial Content of Erased Memory
    3000,                       // Program Page Timeout 3 Sec
    3000,                       // Erase Sector Timeout 3 Sec
    // Specify Size and Address of Sectors
    0x0004, 0x000000,           // Sector Size 4B
    SECTOR_END
  };

#elif defined STM32F410xx_412xx
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,             // Driver Version, do not modify!
    "STM32F410xx/412xx Flash Options",  // Device Name
    ONCHIP,                     // Device Type
    0x1FFFC000,                 // Device Start Address
    0x00000004,                 // Device Size in Bytes (4)
    4,                          // Programming Page Size
    0,                          // Reserved, must be 0
    0xFF,                       // Initial Content of Erased Memory
    3000,                       // Program Page Timeout 3 Sec
    3000,                       // Erase Sector Timeout 3 Sec
    // Specify Size and Address of Sectors
    0x0004, 0x000000,           // Sector Size 4B
    SECTOR_END
  };

#elif defined STM32F413xx_423xx
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,             // Driver Version, do not modify!
    "STM32F413xx/423xx Flash Options",  // Device Name
    ONCHIP,                     // Device Type
    0x1FFFC000,                 // Device Start Address
    0x00000004,                 // Device Size in Bytes (4)
    4,                          // Programming Page Size
    0,                          // Reserved, must be 0
    0xFF,                       // Initial Content of Erased Memory
    3000,                       // Program Page Timeout 3 Sec
    3000,                       // Erase Sector Timeout 3 Sec
    // Specify Size and Address of Sectors
    0x0004, 0x000000,           // Sector Size 4B
    SECTOR_END
  };

#elif defined STM32F446xx
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,             // Driver Version, do not modify!
    "STM32F446xx Flash Options",  // Device Name
    ONCHIP,                     // Device Type
    0x1FFFC000,                 // Device Start Address
    0x00000004,                 // Device Size in Bytes (4)
    4,                          // Programming Page Size
    0,                          // Reserved, must be 0
    0xFF,                       // Initial Content of Erased Memory
    3000,                       // Program Page Timeout 3 Sec
    3000,                       // Erase Sector Timeout 3 Sec
    // Specify Size and Address of Sectors
    0x0004, 0x000000,           // Sector Size 4B
    SECTOR_END
  };

#elif defined STM32F42xxx_43xxx
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,             // Driver Version, do not modify!
    "STM32F42xxx/43xxx Flash Options",  // Device Name
    ONCHIP,                     // Device Type
    0x1FFFC000,                 // Device Start Address
    0x00000008,                 // Device Size in Bytes (8)
    8,                          // Programming Page Size
    0,                          // Reserved, must be 0
    0xFF,                       // Initial Content of Erased Memory
    3000,                       // Program Page Timeout 3 Sec
    3000,                       // Erase Sector Timeout 3 Sec
    // Specify Size and Address of Sectors
    0x0008, 0x000000,           // Sector Size 8B
    SECTOR_END
  };

#elif defined STM32F469xx_479xx
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,             // Driver Version, do not modify!
    "STM32F469xx/479xx Flash Options",  // Device Name
    ONCHIP,                     // Device Type
    0x1FFFC000,                 // Device Start Address
    0x00000008,                 // Device Size in Bytes (8)
    8,                          // Programming Page Size
    0,                          // Reserved, must be 0
    0xFF,                       // Initial Content of Erased Memory
    3000,                       // Program Page Timeout 3 Sec
    3000,                       // Erase Sector Timeout 3 Sec
    // Specify Size and Address of Sectors
    0x0008, 0x000000,           // Sector Size 8B
    SECTOR_END
  };

#elif defined STM32F4xx        // depricated algorithm
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,             // Driver Version, do not modify!
    "STM32F4xx Flash Options",  // Device Name
    ONCHIP,                     // Device Type
    0x1FFFC000,                 // Device Start Address
    0x00000010,                 // Device Size in Bytes (16)
    16,                         // Programming Page Size
    0,                          // Reserved, must be 0
    0xFF,                       // Initial Content of Erased Memory
    3000,                       // Program Page Timeout 3 Sec
    3000,                       // Erase Sector Timeout 3 Sec
    // Specify Size and Address of Sectors
    0x0010, 0x000000,           // Sector Size 16B
    SECTOR_END
  };

#else
  #error Device not selected!
#endif

#endif

#ifdef FLASH_OTP
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,             // Driver Version, do not modify!
    "STM32F4xx Flash OTP",      // Device Name
    ONCHIP,                     // Device Type
    0x1FFF7800,                 // Device Start Address
    0x00000210,                 // Device Size in Bytes (528)
    528,                        // Programming Page Size
    0,                          // Reserved, must be 0
    0xFF,                       // Initial Content of Erased Memory
    3000,                       // Program Page Timeout 3 Sec
    3000,                       // Erase Sector Timeout 3 Sec
    // Specify Size and Address of Sectors
    0x0210, 0x000000,           // Sector Size 528B
    SECTOR_END
  };
#endif
