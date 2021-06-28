/**
  @Generated 16-bit Bootloader Source File

  @Company:
    Microchip Technology Inc.

  @File Name: 
    boot_verify_checksum16.c

  @Summary:
    This is the boot_verify_checksum16.c file generated using 16-bit Bootloader

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

#include <stdbool.h>
#include <stdint.h>

#include "../memory/flash.h"
#include "boot_config.h"

struct BOOT_VERIFY_APPLICATION_HEADER
{
    uint16_t checksum;
    uint32_t startAddress;
    uint32_t endAddress;
};

static const uint32_t applicationHeaderAddress = BOOT_CONFIG_APPLICATION_IMAGE_APPLICATION_HEADER_ADDRESS;

static inline bool isOdd(uint32_t number)
{
    return ((number & 0x00000001) == 0x00000001);
}

/* startAddress and endAddress are PC instruction addresses only (must be even).
 * startAddress and endAddress are inclusive.
 */
static uint16_t Checksum(const uint32_t startAddress, const uint32_t endAddress, const uint16_t initialValue, const uint32_t excludeAddress)
{
    uint16_t checksum = initialValue;
    uint32_t address;
    uint32_t instruction;
    
    for(address = startAddress; address <= endAddress; address+=2)
    {
        if(address != excludeAddress)
        {
            instruction = FLASH_ReadWord24(address);

            checksum += (uint16_t)(instruction & 0x0000FFFF);
            checksum += (uint16_t)(instruction >> 16);
        }
    }
    
    return checksum;
}

static void ApplicationHeaderRead(uint32_t sourceAddress, struct BOOT_VERIFY_APPLICATION_HEADER *applicationHeader)
{
    applicationHeader->checksum = FLASH_ReadWord16(sourceAddress);

    applicationHeader->startAddress = FLASH_ReadWord16(sourceAddress + 2);
    applicationHeader->startAddress += ((uint32_t)FLASH_ReadWord16(sourceAddress + 4)) << 16;

    applicationHeader->endAddress = FLASH_ReadWord16(sourceAddress + 6);
    applicationHeader->endAddress += ((uint32_t)FLASH_ReadWord16(sourceAddress + 8)) << 16;
}

bool BOOT_Verify(void)
{   
    struct BOOT_VERIFY_APPLICATION_HEADER applicationHeader;
    uint16_t calculatedChecksum;

    ApplicationHeaderRead(applicationHeaderAddress, &applicationHeader);

    if( isOdd(applicationHeader.startAddress) )
    {
        return false;
    }
    
    if( isOdd(applicationHeader.endAddress) )
    {
        return false;
    }

    if( applicationHeader.startAddress < BOOT_CONFIG_PROGRAMMABLE_ADDRESS_LOW )
    {
        return false;
    }

    if( applicationHeader.endAddress < BOOT_CONFIG_PROGRAMMABLE_ADDRESS_LOW )
    {
        return false;
    }

    if( applicationHeader.startAddress > BOOT_CONFIG_PROGRAMMABLE_ADDRESS_HIGH )
    {
        return false;
    }

    if( applicationHeader.endAddress > BOOT_CONFIG_PROGRAMMABLE_ADDRESS_HIGH )
    {
        return false;
    }

    calculatedChecksum = Checksum(applicationHeader.startAddress, applicationHeader.endAddress, 0, applicationHeaderAddress);

    return ( applicationHeader.checksum == calculatedChecksum );
}
