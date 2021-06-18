/**
  @Generated 16-bit Bootloader Source File

  @Company:
    Microchip Technology Inc.

  @File Name: 
    boot_verify_atecc608.c

  @Summary:
    This is the boot_verify_atecc608.c file generated using 16-bit Bootloader

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
#include <string.h>

#include "../CryptoAuthenticationLibrary/atca_basic.h"
#include "../CryptoAuthenticationLibrary/atca_iface.h"
#include "../memory/flash.h"
#include "boot_config.h"

#warning "To reduce the size of the memory required by the CryptoAuthLibrary, disable the examples and printf options in the CryptoAuthLibrary.  Double click this warning for more details."
/* To reduce the code used by the CryptoAuthLibrary, the examples and printf
 * options can be disabled.  
 *
 * This is done by going through the following steps:
 * 1) Re-open MCC for this project
 * 2) Click on the CrytpoAuthLibrary module in your project resources pane
 * 3) Uncheck the box next to "Print debug statements in library"
 * 4) Uncheck the box next to "Generate Example"
 * 5) You can now adjust the bootloader size in the "16-bit Bootloader: Bootloader" module in the project resources pane.
 * 6) For these changes to go into effect, click the "Generate" button.
 * 
 * You can comment out this warning once this step is complete.
 */


#define ECDSA_SIGNATURE_LENGTH 64

extern void SHA256(const uint32_t startAddress, const uint32_t endAddress, uint8_t* resultBuffer, uint32_t signatureStartAddress, uint32_t signatureEndAddress);

#define MEMCPY_BUFFERS_MATCH 0

static const uint8_t publicKey[64] =  { 
    0x4A, 0xFD, 0xF1, 0xE6, 0x14, 0x56, 0x38, 0x77, 
    0xCE, 0xC1, 0x05, 0xE9, 0x30, 0x50, 0x00, 0x2E, 
    0x38, 0x34, 0x69, 0x71, 0x2C, 0x72, 0xE2, 0x0B, 
    0x46, 0x98, 0x0C, 0xC4, 0x3E, 0x02, 0x3B, 0xA4, 
    0xCF, 0x52, 0x10, 0x63, 0x15, 0x77, 0x5D, 0x84, 
    0xC5, 0x70, 0x32, 0x5A, 0xC6, 0xC1, 0xF8, 0x4F, 
    0xEF, 0xB7, 0x1F, 0x63, 0x32, 0xAD, 0x5C, 0x1A, 
    0x4F, 0x31, 0xF8, 0xF3, 0xEE, 0xC2, 0xA5, 0xC5, 
};

struct BOOT_VERIFY_APPLICATION_HEADER
{
    uint8_t signature[ECDSA_SIGNATURE_LENGTH];
    uint32_t startAddress;
    uint32_t endAddress;
};

static const uint32_t applicationHeaderAddress = BOOT_CONFIG_APPLICATION_IMAGE_APPLICATION_HEADER_ADDRESS;

static inline bool isOdd(uint32_t number)
{
    return ((number & 0x00000001) == 0x00000001);
}

static void ApplicationHeaderRead(uint32_t sourceAddress, struct BOOT_VERIFY_APPLICATION_HEADER *applicationHeader)
{
    uint16_t data;
    uint8_t offset;

    for(offset=0; offset<ECDSA_SIGNATURE_LENGTH; offset+=2)
    {
        data = FLASH_ReadWord16(sourceAddress + offset);
        applicationHeader->signature[offset] = (uint8_t) (data & 0x00FF);
        applicationHeader->signature[offset+1] = (uint8_t) (data >> 8);
    }

    applicationHeader->startAddress = FLASH_ReadWord16(sourceAddress + offset);
    applicationHeader->startAddress += ((uint32_t)FLASH_ReadWord16(sourceAddress + offset + 2)) << 16;

    applicationHeader->endAddress = FLASH_ReadWord16(sourceAddress + offset + 4);
    applicationHeader->endAddress += ((uint32_t)FLASH_ReadWord16(sourceAddress + offset + 6)) << 16;
}

static uint32_t getSignatureEndAddress(uint32_t applicationHeaderAddress)
{
    return applicationHeaderAddress + ECDSA_SIGNATURE_LENGTH - 2;
}

static uint32_t getSignatureStartAddress(uint32_t applicationHeaderAddress)
{
    return applicationHeaderAddress;
}

bool BOOT_Verify(void)
{   
    ATCA_STATUS status;
    bool isVerified;
    struct BOOT_VERIFY_APPLICATION_HEADER applicationHeader;
    uint8_t calculatedHash[32];

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

    SHA256(applicationHeader.startAddress, applicationHeader.endAddress, calculatedHash, getSignatureStartAddress(applicationHeaderAddress), getSignatureEndAddress(applicationHeaderAddress));
	
    status = atcab_verify_extern(calculatedHash, applicationHeader.signature, publicKey, &isVerified);

    return ((status == ATCA_SUCCESS) && (isVerified == true));
}
