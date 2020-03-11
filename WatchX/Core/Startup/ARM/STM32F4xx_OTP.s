;/*****************************************************************************/
;/* STM32F4xx_OTP.s: STM32F4xx Flash One-Time Programmable Bytes              */
;/*****************************************************************************/
;/* <<< Use Configuration Wizard in Context Menu >>>                          */
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2012 Keil Software. All rights reserved.                    */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/


;// <e> Flash One-Time Programmable Bytes
FLASH_OTP       EQU     1

;// <h> OTP Block 0
;//   <o8> Block Lock
;//       <0x00=> Enabled 
;//       <0xFF=> Disabled 
;//   <o0> Byte  3.. 0
;//   <o1> Byte  7.. 4
;//   <o2> Byte 11.. 8
;//   <o3> Byte 15..12
;//   <o4> Byte 19..16
;//   <o5> Byte 23..20
;//   <o6> Byte 27..24
;//   <o7> Byte 31..28
;// </h>
OTP00_0          EQU     0xFFFFFFFF
OTP00_1          EQU     0xFFFFFFFF
OTP00_2          EQU     0xFFFFFFFF
OTP00_3          EQU     0xFFFFFFFF
OTP00_4          EQU     0xFFFFFFFF
OTP00_5          EQU     0xFFFFFFFF
OTP00_6          EQU     0xFFFFFFFF
OTP00_7          EQU     0xFFFFFFFF
LOCKB00          EQU     0xFF

;// <h> OTP Block 1
;//   <o8> Block Lock
;//       <0x00=> Enabled 
;//       <0xFF=> Disabled 
;//   <o0> Byte  3.. 0
;//   <o1> Byte  7.. 4
;//   <o2> Byte 11.. 8
;//   <o3> Byte 15..12
;//   <o4> Byte 19..16
;//   <o5> Byte 23..20
;//   <o6> Byte 27..24
;//   <o7> Byte 31..28
;// </h>
OTP01_0          EQU     0xFFFFFFFF
OTP01_1          EQU     0xFFFFFFFF
OTP01_2          EQU     0xFFFFFFFF
OTP01_3          EQU     0xFFFFFFFF
OTP01_4          EQU     0xFFFFFFFF
OTP01_5          EQU     0xFFFFFFFF
OTP01_6          EQU     0xFFFFFFFF
OTP01_7          EQU     0xFFFFFFFF
LOCKB01          EQU     0xFF

;// <h> OTP Block 2
;//   <o8> Block Lock
;//       <0x00=> Enabled 
;//       <0xFF=> Disabled 
;//   <o0> Byte  3.. 0
;//   <o1> Byte  7.. 4
;//   <o2> Byte 11.. 8
;//   <o3> Byte 15..12
;//   <o4> Byte 19..16
;//   <o5> Byte 23..20
;//   <o6> Byte 27..24
;//   <o7> Byte 31..28
;// </h>
OTP02_0          EQU     0xFFFFFFFF
OTP02_1          EQU     0xFFFFFFFF
OTP02_2          EQU     0xFFFFFFFF
OTP02_3          EQU     0xFFFFFFFF
OTP02_4          EQU     0xFFFFFFFF
OTP02_5          EQU     0xFFFFFFFF
OTP02_6          EQU     0xFFFFFFFF
OTP02_7          EQU     0xFFFFFFFF
LOCKB02          EQU     0xFF

;// <h> OTP Block 3
;//   <o8> Block Lock
;//       <0x00=> Enabled 
;//       <0xFF=> Disabled 
;//   <o0> Byte  3.. 0
;//   <o1> Byte  7.. 4
;//   <o2> Byte 11.. 8
;//   <o3> Byte 15..12
;//   <o4> Byte 19..16
;//   <o5> Byte 23..20
;//   <o6> Byte 27..24
;//   <o7> Byte 31..28
;// </h>
OTP03_0          EQU     0xFFFFFFFF
OTP03_1          EQU     0xFFFFFFFF
OTP03_2          EQU     0xFFFFFFFF
OTP03_3          EQU     0xFFFFFFFF
OTP03_4          EQU     0xFFFFFFFF
OTP03_5          EQU     0xFFFFFFFF
OTP03_6          EQU     0xFFFFFFFF
OTP03_7          EQU     0xFFFFFFFF
LOCKB03          EQU     0xFF

;// <h> OTP Block 4
;//   <o8> Block Lock
;//       <0x00=> Enabled 
;//       <0xFF=> Disabled 
;//   <o0> Byte  3.. 0
;//   <o1> Byte  7.. 4
;//   <o2> Byte 11.. 8
;//   <o3> Byte 15..12
;//   <o4> Byte 19..16
;//   <o5> Byte 23..20
;//   <o6> Byte 27..24
;//   <o7> Byte 31..28
;// </h>
OTP04_0          EQU     0xFFFFFFFF
OTP04_1          EQU     0xFFFFFFFF
OTP04_2          EQU     0xFFFFFFFF
OTP04_3          EQU     0xFFFFFFFF
OTP04_4          EQU     0xFFFFFFFF
OTP04_5          EQU     0xFFFFFFFF
OTP04_6          EQU     0xFFFFFFFF
OTP04_7          EQU     0xFFFFFFFF
LOCKB04          EQU     0xFF

;// <h> OTP Block 5
;//   <o8> Block Lock
;//       <0x00=> Enabled 
;//       <0xFF=> Disabled 
;//   <o0> Byte  3.. 0
;//   <o1> Byte  7.. 4
;//   <o2> Byte 11.. 8
;//   <o3> Byte 15..12
;//   <o4> Byte 19..16
;//   <o5> Byte 23..20
;//   <o6> Byte 27..24
;//   <o7> Byte 31..28
;// </h>
OTP05_0          EQU     0xFFFFFFFF
OTP05_1          EQU     0xFFFFFFFF
OTP05_2          EQU     0xFFFFFFFF
OTP05_3          EQU     0xFFFFFFFF
OTP05_4          EQU     0xFFFFFFFF
OTP05_5          EQU     0xFFFFFFFF
OTP05_6          EQU     0xFFFFFFFF
OTP05_7          EQU     0xFFFFFFFF
LOCKB05          EQU     0xFF

;// <h> OTP Block 6
;//   <o8> Block Lock
;//       <0x00=> Enabled 
;//       <0xFF=> Disabled 
;//   <o0> Byte  3.. 0
;//   <o1> Byte  7.. 4
;//   <o2> Byte 11.. 8
;//   <o3> Byte 15..12
;//   <o4> Byte 19..16
;//   <o5> Byte 23..20
;//   <o6> Byte 27..24
;//   <o7> Byte 31..28
;// </h>
OTP06_0          EQU     0xFFFFFFFF
OTP06_1          EQU     0xFFFFFFFF
OTP06_2          EQU     0xFFFFFFFF
OTP06_3          EQU     0xFFFFFFFF
OTP06_4          EQU     0xFFFFFFFF
OTP06_5          EQU     0xFFFFFFFF
OTP06_6          EQU     0xFFFFFFFF
OTP06_7          EQU     0xFFFFFFFF
LOCKB06          EQU     0xFF

;// <h> OTP Block 7
;//   <o8> Block Lock
;//       <0x00=> Enabled 
;//       <0xFF=> Disabled 
;//   <o0> Byte  3.. 0
;//   <o1> Byte  7.. 4
;//   <o2> Byte 11.. 8
;//   <o3> Byte 15..12
;//   <o4> Byte 19..16
;//   <o5> Byte 23..20
;//   <o6> Byte 27..24
;//   <o7> Byte 31..28
;// </h>
OTP07_0          EQU     0xFFFFFFFF
OTP07_1          EQU     0xFFFFFFFF
OTP07_2          EQU     0xFFFFFFFF
OTP07_3          EQU     0xFFFFFFFF
OTP07_4          EQU     0xFFFFFFFF
OTP07_5          EQU     0xFFFFFFFF
OTP07_6          EQU     0xFFFFFFFF
OTP07_7          EQU     0xFFFFFFFF
LOCKB07          EQU     0xFF

;// <h> OTP Block 8
;//   <o8> Block Lock
;//       <0x00=> Enabled 
;//       <0xFF=> Disabled 
;//   <o0> Byte  3.. 0
;//   <o1> Byte  7.. 4
;//   <o2> Byte 11.. 8
;//   <o3> Byte 15..12
;//   <o4> Byte 19..16
;//   <o5> Byte 23..20
;//   <o6> Byte 27..24
;//   <o7> Byte 31..28
;// </h>
OTP08_0          EQU     0xFFFFFFFF
OTP08_1          EQU     0xFFFFFFFF
OTP08_2          EQU     0xFFFFFFFF
OTP08_3          EQU     0xFFFFFFFF
OTP08_4          EQU     0xFFFFFFFF
OTP08_5          EQU     0xFFFFFFFF
OTP08_6          EQU     0xFFFFFFFF
OTP08_7          EQU     0xFFFFFFFF
LOCKB08          EQU     0xFF

;// <h> OTP Block 9
;//   <o8> Block Lock
;//       <0x00=> Enabled 
;//       <0xFF=> Disabled 
;//   <o0> Byte  3.. 0
;//   <o1> Byte  7.. 4
;//   <o2> Byte 11.. 8
;//   <o3> Byte 15..12
;//   <o4> Byte 19..16
;//   <o5> Byte 23..20
;//   <o6> Byte 27..24
;//   <o7> Byte 31..28
;// </h>
OTP09_0          EQU     0xFFFFFFFF
OTP09_1          EQU     0xFFFFFFFF
OTP09_2          EQU     0xFFFFFFFF
OTP09_3          EQU     0xFFFFFFFF
OTP09_4          EQU     0xFFFFFFFF
OTP09_5          EQU     0xFFFFFFFF
OTP09_6          EQU     0xFFFFFFFF
OTP09_7          EQU     0xFFFFFFFF
LOCKB09          EQU     0xFF

;// <h> OTP Block 10
;//   <o8> Block Lock
;//       <0x00=> Enabled 
;//       <0xFF=> Disabled 
;//   <o0> Byte  3.. 0
;//   <o1> Byte  7.. 4
;//   <o2> Byte 11.. 8
;//   <o3> Byte 15..12
;//   <o4> Byte 19..16
;//   <o5> Byte 23..20
;//   <o6> Byte 27..24
;//   <o7> Byte 31..28
;// </h>
OTP10_0          EQU     0xFFFFFFFF
OTP10_1          EQU     0xFFFFFFFF
OTP10_2          EQU     0xFFFFFFFF
OTP10_3          EQU     0xFFFFFFFF
OTP10_4          EQU     0xFFFFFFFF
OTP10_5          EQU     0xFFFFFFFF
OTP10_6          EQU     0xFFFFFFFF
OTP10_7          EQU     0xFFFFFFFF
LOCKB10          EQU     0xFF

;// <h> OTP Block 11
;//   <o8> Block Lock
;//       <0x00=> Enabled 
;//       <0xFF=> Disabled 
;//   <o0> Byte  3.. 0
;//   <o1> Byte  7.. 4
;//   <o2> Byte 11.. 8
;//   <o3> Byte 15..12
;//   <o4> Byte 19..16
;//   <o5> Byte 23..20
;//   <o6> Byte 27..24
;//   <o7> Byte 31..28
;// </h>
OTP11_0          EQU     0xFFFFFFFF
OTP11_1          EQU     0xFFFFFFFF
OTP11_2          EQU     0xFFFFFFFF
OTP11_3          EQU     0xFFFFFFFF
OTP11_4          EQU     0xFFFFFFFF
OTP11_5          EQU     0xFFFFFFFF
OTP11_6          EQU     0xFFFFFFFF
OTP11_7          EQU     0xFFFFFFFF
LOCKB11          EQU     0xFF

;// <h> OTP Block 12
;//   <o8> Block Lock
;//       <0x00=> Enabled 
;//       <0xFF=> Disabled 
;//   <o0> Byte  3.. 0
;//   <o1> Byte  7.. 4
;//   <o2> Byte 11.. 8
;//   <o3> Byte 15..12
;//   <o4> Byte 19..16
;//   <o5> Byte 23..20
;//   <o6> Byte 27..24
;//   <o7> Byte 31..28
;// </h>
OTP12_0          EQU     0xFFFFFFFF
OTP12_1          EQU     0xFFFFFFFF
OTP12_2          EQU     0xFFFFFFFF
OTP12_3          EQU     0xFFFFFFFF
OTP12_4          EQU     0xFFFFFFFF
OTP12_5          EQU     0xFFFFFFFF
OTP12_6          EQU     0xFFFFFFFF
OTP12_7          EQU     0xFFFFFFFF
LOCKB12          EQU     0xFF

;// <h> OTP Block 13
;//   <o8> Block Lock
;//       <0x00=> Enabled 
;//       <0xFF=> Disabled 
;//   <o0> Byte  3.. 0
;//   <o1> Byte  7.. 4
;//   <o2> Byte 11.. 8
;//   <o3> Byte 15..12
;//   <o4> Byte 19..16
;//   <o5> Byte 23..20
;//   <o6> Byte 27..24
;//   <o7> Byte 31..28
;// </h>
OTP13_0          EQU     0xFFFFFFFF
OTP13_1          EQU     0xFFFFFFFF
OTP13_2          EQU     0xFFFFFFFF
OTP13_3          EQU     0xFFFFFFFF
OTP13_4          EQU     0xFFFFFFFF
OTP13_5          EQU     0xFFFFFFFF
OTP13_6          EQU     0xFFFFFFFF
OTP13_7          EQU     0xFFFFFFFF
LOCKB13          EQU     0xFF

;// <h> OTP Block 14
;//   <o8> Block Lock
;//       <0x00=> Enabled 
;//       <0xFF=> Disabled 
;//   <o0> Byte  3.. 0
;//   <o1> Byte  7.. 4
;//   <o2> Byte 11.. 8
;//   <o3> Byte 15..12
;//   <o4> Byte 19..16
;//   <o5> Byte 23..20
;//   <o6> Byte 27..24
;//   <o7> Byte 31..28
;// </h>
OTP14_0          EQU     0xFFFFFFFF
OTP14_1          EQU     0xFFFFFFFF
OTP14_2          EQU     0xFFFFFFFF
OTP14_3          EQU     0xFFFFFFFF
OTP14_4          EQU     0xFFFFFFFF
OTP14_5          EQU     0xFFFFFFFF
OTP14_6          EQU     0xFFFFFFFF
OTP14_7          EQU     0xFFFFFFFF
LOCKB14          EQU     0xFF

;// <h> OTP Block 15
;//   <o8> Block Lock
;//       <0x00=> Enabled 
;//       <0xFF=> Disabled 
;//   <o0> Byte  3.. 0
;//   <o1> Byte  7.. 4
;//   <o2> Byte 11.. 8
;//   <o3> Byte 15..12
;//   <o4> Byte 19..16
;//   <o5> Byte 23..20
;//   <o6> Byte 27..24
;//   <o7> Byte 31..28
;// </h>
OTP15_0          EQU     0xFFFFFFFF
OTP15_1          EQU     0xFFFFFFFF
OTP15_2          EQU     0xFFFFFFFF
OTP15_3          EQU     0xFFFFFFFF
OTP15_4          EQU     0xFFFFFFFF
OTP15_5          EQU     0xFFFFFFFF
OTP15_6          EQU     0xFFFFFFFF
OTP15_7          EQU     0xFFFFFFFF
LOCKB15          EQU     0xFF


;// </e>


                IF      FLASH_OTP <> 0
                AREA    |.ARM.__AT_0x1FFF7800|, CODE, READONLY
                DCD     OTP00_0,  OTP00_1,  OTP00_2, OTP00_3, OTP00_4, OTP00_5, OTP00_6, OTP00_7
                DCD     OTP01_0,  OTP01_1,  OTP01_2, OTP01_3, OTP01_4, OTP01_5, OTP01_6, OTP01_7
                DCD     OTP02_0,  OTP02_1,  OTP02_2, OTP02_3, OTP02_4, OTP02_5, OTP02_6, OTP02_7
                DCD     OTP03_0,  OTP03_1,  OTP03_2, OTP03_3, OTP03_4, OTP03_5, OTP03_6, OTP03_7
                DCD     OTP04_0,  OTP04_1,  OTP04_2, OTP04_3, OTP04_4, OTP04_5, OTP04_6, OTP04_7
                DCD     OTP05_0,  OTP05_1,  OTP05_2, OTP05_3, OTP05_4, OTP05_5, OTP05_6, OTP05_7
                DCD     OTP06_0,  OTP06_1,  OTP06_2, OTP06_3, OTP06_4, OTP06_5, OTP06_6, OTP06_7
                DCD     OTP07_0,  OTP07_1,  OTP07_2, OTP07_3, OTP07_4, OTP07_5, OTP07_6, OTP07_7
                DCD     OTP08_0,  OTP08_1,  OTP08_2, OTP08_3, OTP08_4, OTP08_5, OTP08_6, OTP08_7
                DCD     OTP09_0,  OTP09_1,  OTP09_2, OTP09_3, OTP09_4, OTP09_5, OTP09_6, OTP09_7
                DCD     OTP10_0,  OTP10_1,  OTP10_2, OTP10_3, OTP10_4, OTP10_5, OTP10_6, OTP10_7
                DCD     OTP11_0,  OTP11_1,  OTP11_2, OTP11_3, OTP11_4, OTP11_5, OTP11_6, OTP11_7
                DCD     OTP12_0,  OTP12_1,  OTP12_2, OTP12_3, OTP12_4, OTP12_5, OTP12_6, OTP12_7
                DCD     OTP13_0,  OTP13_1,  OTP13_2, OTP13_3, OTP13_4, OTP13_5, OTP13_6, OTP13_7
                DCD     OTP14_0,  OTP14_1,  OTP14_2, OTP14_3, OTP14_4, OTP14_5, OTP14_6, OTP14_7
                DCD     OTP15_0,  OTP15_1,  OTP15_2, OTP15_3, OTP15_4, OTP15_5, OTP15_6, OTP15_7
                DCB     LOCKB00,  LOCKB01,  LOCKB02, LOCKB03, LOCKB04, LOCKB05, LOCKB06, LOCKB07
                DCB     LOCKB08,  LOCKB09,  LOCKB10, LOCKB11, LOCKB12, LOCKB13, LOCKB14, LOCKB15
                ENDIF

                END
