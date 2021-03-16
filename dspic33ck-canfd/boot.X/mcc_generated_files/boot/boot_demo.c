/**
  @Generated 16-bit Bootloader Source File

  @Company:
    Microchip Technology Inc.

  @File Name: 
    boot_demo.c

  @Summary:
    This is the boot_demo.c file generated using 16-bit Bootloader

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  16-bit Bootloader - 1.18.4-SNAPSHOT
        Device            :  dsPIC33CK256MP508
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36B
        MPLAB             :  MPLAB X v5.15
*/
/*
Copyright (c) [2012-2019] Microchip Technology Inc.  

    All rights reserved.

    You are permitted to use the accompanying software and its derivatives 
    with Microchip products. See the Microchip license agreement accompanying 
    this software, if any, for additional info regarding your rights and 
    obligations.
    
    MICROCHIP SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT 
    WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
    LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
    AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP OR ITS
    LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT 
    LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE 
    THEORY FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT 
    LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, 
    OR OTHER SIMILAR COSTS. 
    
    To the fullest extend allowed by law, Microchip and its licensors 
    liability will not exceed the amount of fees, if any, that you paid 
    directly to Microchip to use this software. 
    
    THIRD PARTY SOFTWARE:  Notwithstanding anything to the contrary, any 
    third party software accompanying this software is subject to the terms 
    and conditions of the third party's license agreement.  To the extent 
    required by third party licenses covering such third party software, 
    the terms of such license will apply in lieu of the terms provided in 
    this notice or applicable license.  To the extent the terms of such 
    third party licenses prohibit any of the restrictions described here, 
    such restrictions will not apply to such third party software.
*/
#include "../memory/flash.h"
#include "boot_process.h"
#include "../pin_manager.h"
#include <stdbool.h>
#include <stdint.h>
#include "boot_config.h"
#include "../tmr1.h"

static bool inBootloadMode = false;

static bool EnterBootloadMode(void);

void BOOT_DEMO_Initialize(void)
{    

}

/*
 * EXP16 Board, LED0 = IO_RE0
 * dsPIC33CK Curiosity Board LED = IO_RE13 = BLUE LED
 * Automotive Networking Board LED1 = IO_RE0
 * 
 */
void BOOT_DEMO_Tasks(void)
{
    // Turn on LED to indicate bootloader mode.
    IO_RE0_SetHigh();
    
    if(inBootloadMode == false)
    {
        if( (EnterBootloadMode() == true) || (BOOT_Verify() == false) )
        {
            inBootloadMode = true;
        }
        else
        {
            TMR1_Stop();
            BOOT_StartApplication();
        }
    }
    
    BOOT_ProcessCommand();
}


 
/*
 * EXP16 Board, SW S3 = IO_RB14
 * dsPIC33CK Curiosity Board SW S1 = IO_RE7 
 * Automotive Networking Board SW S1 = IO_RE6
 * 
 */
static bool EnterBootloadMode(void)
{
 //   #warning "Update this function to return 'true' when you want to stay in the boot loader, and 'false' when you want to allow a release to the application code"
 
    /* NOTE: This might be a a push button status on power up, a command from a peripheral, 
     * or whatever is specific to your boot loader implementation */    
    return (IO_RB14_GetValue() == 0);
    
}
