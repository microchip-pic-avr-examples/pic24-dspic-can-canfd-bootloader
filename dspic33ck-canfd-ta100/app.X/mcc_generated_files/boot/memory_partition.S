/**
  @Generated 16-bit Bootloader Source File

  @Company:
    Microchip Technology Inc.

  @File Name: 
    memory_partition.S

  @Summary:
    This is the memory_partition.S file generated using 16-bit Bootloader

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  16-bit Bootloader - 1.19.0
        Device            :  dsPIC33CK256MP508
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36B
        MPLAB             :  MPLAB X v5.15
*/
/*
Copyright (c) [2012-2021] Microchip Technology Inc.  

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

#include "boot_config.h"
    .equ    ERASE_PAGE_MASK,(~((2048) - 1)) 
    .equ    LAST_PAGE_START_ADDRESS, (0x2BEFE & ERASE_PAGE_MASK)
    .equ    RESERVED_MEMORY_START, (0x2B7FE+2)
    .equ    PROGRAM_MEMORY_ORIGIN, (0x000200)
    .equ    LAST_ADDRESS_OF_MEMORY, (0x2BEFE)
   
    /* The exectuable application image lives between:
     *   Start Address = 0x8000
     *   End Address = 0x2B7FE
     * 
     * All other memory on the device must be reserved so that executable
     * application code can't link to those addresses.  This includes the space
     * required by the bootloader.
     *
     * The value in the address() field defines the start address.
     * The number on the .space defines the size of this block in PC units (2 per
     * instruction).
     */

    /* Reserve the memory used by the bootloader */
    .section *, code, address(PROGRAM_MEMORY_ORIGIN), noload, keep
boot_loader_memory:
    .space (BOOT_CONFIG_PROGRAMMABLE_ADDRESS_LOW  - PROGRAM_MEMORY_ORIGIN), 0x00

    /* Reserve all of the memory after the last instruction of the executable
     * image, through the end of device memory.
     */
    .section *, code, address(RESERVED_MEMORY_START), noload, keep
config_page_memory:
    .space (LAST_ADDRESS_OF_MEMORY-RESERVED_MEMORY_START), 0x00
    
