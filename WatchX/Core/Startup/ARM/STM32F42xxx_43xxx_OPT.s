;/*****************************************************************************/
;/* STM32F42xxx_43xxx_OPT.s: STM32F42xxx and STM32F43xxx Flash Option Bytes   */
;/*****************************************************************************/
;/* <<< Use Configuration Wizard in Context Menu >>>                          */
;/*****************************************************************************/
;/*  This file is part of the uVision/ARM development tools                   */
;/*  Copyright (c) 2014 Keil - An ARM Company.                                */
;/*  All rights reserved.                                                     */
;/*****************************************************************************/

;// <e> Flash Option Bytes
FLASH_OPT       EQU     1

;// <h> Flash Read Protection
;//     <i> Read protection is used to protect the software code stored in Flash memory
;//   <o0.8..15> Read Protection Level
;//     <i> Level 0: No Protection 
;//     <i> Level 1: Read Protection of Memories (debug features limited)
;//     <i> Level 2: Chip Protection (debug and boot in RAM features disabled)
;//          <0xAA=> Level 0 (No Protection) 
;//          <0x00=> Level 1 (Read Protection of Memories)
;//          <0xCC=> Level 2 (Chip Protection)
;// </h>

;// <h> Flash Write Protection
;//   <o0.31> SPRMOD
;//     <i> Selection of protection mode for nWPRi bits
;//          <0=> PCROP disabled
;//          <1=> PCROP enabled
;//   <o0.30> DB1M
;//     <i> Dual-bank on 1 Mbyte Flash memory devices
;//          <0=> 1 Mbyte single bank Flash memory
;//          <1=> 1 Mbyte dual bank Flash memory
;//   <h> nWRP Sectors 0 to 11
;//       <i> Not write protect Sectors 0 to 11
;//     <o0.16> Sector 0
;//     <o0.17> Sector 1
;//     <o0.18> Sector 2
;//     <o0.19> Sector 3
;//     <o0.20> Sector 4
;//     <o0.21> Sector 5
;//     <o0.22> Sector 6
;//     <o0.23> Sector 7
;//     <o0.24> Sector 8
;//     <o0.25> Sector 9
;//     <o0.26> Sector 10
;//     <o0.27> Sector 11
;//   </h>
;//   <h> nWRP Sectors 12 to 23
;//       <i> Not write protect Sectors 12 to 23
;//     <o1.16> Sector 12
;//     <o1.17> Sector 13
;//     <o1.18> Sector 14
;//     <o1.19> Sector 15
;//     <o1.20> Sector 16
;//     <o1.21> Sector 17
;//     <o1.22> Sector 18
;//     <o1.23> Sector 19
;//     <o1.24> Sector 20
;//     <o1.25> Sector 21
;//     <o1.26> Sector 22
;//     <o1.27> Sector 23
;//   </h>
;// </h>

;// <h> User Configuration
;//   <o0.2..3> BOR_LEV     
;//          <0=> BOR Level 3 (VBOR3). Reset threshold level from 2.70 to 3.60 V
;//          <1=> BOR Level 2 (VBOR2). Reset threshold level from 2.40 to 2.70 V
;//          <2=> BOR Level 1 (VBOR1). Reset threshold level from 2.10 to 2.40 V
;//          <3=> BOR off     (VBOR0). Reset threshold level from 1.80 to 2.10 V
;//   <o0.4> Dual-bank Boot Option     
;//          <0=> Dual-bank Boot Disabled
;//          <1=> Dual-bank Boot Enabled
;//   <o0.5> WDG_SW     
;//          <0=> HW Watchdog
;//          <1=> SW Watchdog
;//   <o0.6> nRST_STOP
;//     <i> Generate Reset when entering STOP Mode
;//          <0=> Enabled
;//          <1=> Disabled
;//   <o0.7> nRST_STDBY
;//     <i> Generate Reset when entering Standby Mode
;//          <0=> Enabled
;//          <1=> Disabled
;// </h>

FLASH_OPTCR    EQU     0x0FFFAAEC
FLASH_OPTCR1   EQU     0x0FFF0000
;// </e>


                IF      FLASH_OPT <> 0
                AREA    |.ARM.__AT_0x1FFFC000|, CODE, READONLY
                DCD     FLASH_OPTCR
                DCD     FLASH_OPTCR1
                ENDIF

                END
