/**
  @Generated 16-bit Bootloader Source File

  @Company:
    Microchip Technology Inc.

  @File Name: 
    application_header_checksum16.S

  @Summary:
    This is the application_header_checksum16.S file generated using 16-bit Bootloader

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  16-bit Bootloader - 1.18.4
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

/* This file puts an application header at the start of the application image that the
 * bootloader will use to verify the integrity of the application image.
 */

#include "boot_config.h"

//#error "A script to append verification checksum has been added to your project. You must add a call to it as a post-build step in your project properties. Delete this warning after you have done so."  
/*
* Steps to append verification checksum to the project hex file:  
* 1. Right click on your project, and click 'Properties'.
* 2. Select the 'Building' left navigation node.
* 3. Check the box next to 'Execute this line after the build'.
* 4. In the text field below, 
*    add "cd mcc_generated_files/boot && postBuild.bat $(MP_CC_DIR)" (without quotes) if you are on a Windows machine, 
*    or "cd mcc_generated_files/boot && ./postBuild.sh $(MP_CC_DIR)" (without quotes) if you are on a Linux/Unix/Mac machine.  
* 5. Delete or comment out the #error message above
*/

    .section .application_header_checksum16, code, address(BOOT_CONFIG_APPLICATION_IMAGE_APPLICATION_HEADER_ADDRESS), keep
    
    /* Skip the instruction that holds the Checksum - this will have to be injected
     * by a tool later, but we need to reserve the space for it. 
     */
    .skip(2) 	
	
    /* Address where to start the checksum */
    .long(0x3000)
	
    /* Address where to end the checksum */
    .long(0x2B7FE)

