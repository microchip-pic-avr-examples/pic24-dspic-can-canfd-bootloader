/**
  @Generated 16-bit Bootloader Source File

  @Company:
    Microchip Technology Inc.

  @File Name: 
    sha256.c

  @Summary:
    This is the sha256.c file generated using 16-bit Bootloader

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

#include <stdint.h>
#include <string.h>

#include "../memory/flash.h"

#define SHA256_BLOCK_LENGTH 32

extern void SHA_Initialize(void);
extern void SHA_HashBlock(void);
extern void SHA_HashGet(uint8_t *output);
extern uint32_t workingBuffer[64];

static uint32_t signatureStartAddress;
static uint32_t signatureEndAddress;

static bool IsAddressInSignature(uint32_t address)
{
    return (address >= signatureStartAddress) && ( address <= signatureEndAddress );
}

static bool IsSignatureInBlock(uint32_t blockAddress)
{
    if((signatureStartAddress >= blockAddress) && ( signatureStartAddress < (blockAddress + SHA256_BLOCK_LENGTH) ))
    {
        return true;
    }
    
    return (signatureEndAddress >= blockAddress) && ( signatureEndAddress < (blockAddress + SHA256_BLOCK_LENGTH) );
}

static void BlankSignature(uint32_t blockAddress)
{
    if( IsSignatureInBlock(blockAddress) == true )
    {
        unsigned int i;

        for(i = 0; i<16; i++)
        {
            if(IsAddressInSignature(blockAddress + (i*2)) == true)
            {
                //Zero out signature data
                workingBuffer[i] = 0;
            }
        }
    }
}

/* startAddress and endAddress are PC instruction addresses only (must be even).
 * startAddress and endAddress are inclusive.
 */
void SHA256(const uint32_t startAddress, const uint32_t endAddress, uint8_t* resultBuffer, uint32_t signatureStartAddressParam, uint32_t signatureEndAddressParam)
{    
    uint32_t byteCount = ((endAddress + 2) - startAddress) * 2;
    uint32_t currentAddress = startAddress;
    uint32_t i;
    uint8_t * blockPtr;
    uint8_t * endPtr;
    uint32_t blockAddress;
    const unsigned int blockSize = 64;
    unsigned int blockCount = byteCount / blockSize;
    
    SHA_Initialize();
    
    signatureStartAddress = signatureStartAddressParam;
    signatureEndAddress = signatureEndAddressParam;

    /* process all block size chunks first */
    while(blockCount--)
    {
        workingBuffer[0] = FLASH_ReadWord24(currentAddress);
        workingBuffer[1] = FLASH_ReadWord24(currentAddress+2);
        workingBuffer[2] = FLASH_ReadWord24(currentAddress+4);
        workingBuffer[3] = FLASH_ReadWord24(currentAddress+6);
        workingBuffer[4] = FLASH_ReadWord24(currentAddress+8);
        workingBuffer[5] = FLASH_ReadWord24(currentAddress+10);
        workingBuffer[6] = FLASH_ReadWord24(currentAddress+12);
        workingBuffer[7] = FLASH_ReadWord24(currentAddress+14);
        workingBuffer[8] = FLASH_ReadWord24(currentAddress+16);
        workingBuffer[9] = FLASH_ReadWord24(currentAddress+18);
        workingBuffer[10] = FLASH_ReadWord24(currentAddress+20);
        workingBuffer[11] = FLASH_ReadWord24(currentAddress+22);
        workingBuffer[12] = FLASH_ReadWord24(currentAddress+24);
        workingBuffer[13] = FLASH_ReadWord24(currentAddress+26);
        workingBuffer[14] = FLASH_ReadWord24(currentAddress+28);
        workingBuffer[15] = FLASH_ReadWord24(currentAddress+30);
        
        BlankSignature(currentAddress);

        currentAddress += SHA256_BLOCK_LENGTH;

        SHA_HashBlock();
    }

    /* calculate on the remainder */
    i=0;
    blockAddress = currentAddress;
    while(currentAddress <= endAddress)
    {
        workingBuffer[i++] = FLASH_ReadWord24(currentAddress);
        currentAddress += 2;
    }
    
    BlankSignature(blockAddress);

    blockPtr = ((uint8_t*)workingBuffer) + (byteCount & 0x3F);
    
    *blockPtr++ = 0x80;

    endPtr = ((uint8_t*)workingBuffer) + 56;

    if(blockPtr > endPtr)
    {
        endPtr += 8;
        while (blockPtr < endPtr)
        {
            *blockPtr++ = 0x00;
        }
        SHA_HashBlock();
        blockPtr = ((uint8_t*)workingBuffer);
        endPtr -= 8;
    }

    // Set the offset to 5 bytes before the end of the block
    endPtr += 3;
    while (blockPtr < endPtr)
    {
        *blockPtr++ = 0x00;
    }

    *blockPtr++ = byteCount >> 29;
    *blockPtr++ = byteCount >> 21;
    *blockPtr++ = byteCount >> 13;
    *blockPtr++ = byteCount >> 5;
    *blockPtr++ = byteCount << 3;

    // Calculate a hash on this final block and add it to the sum
    SHA_HashBlock();

    SHA_HashGet(resultBuffer);
}