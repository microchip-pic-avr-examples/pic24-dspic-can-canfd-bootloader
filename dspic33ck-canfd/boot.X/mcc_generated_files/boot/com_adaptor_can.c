/**
  @Generated 16-bit Bootloader Source File

  @Company:
    Microchip Technology Inc.

  @File Name: 
    com_adaptor_uart.c

  @Summary:
    This is the com_adaptor_uart.c file generated using 16-bit Bootloader

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
#include <string.h>
#include <stdbool.h>
#include "com_adaptor.h"
#include "boot_config.h"
#include "boot_private.h"
#include "../can_tp.h"

static bool initialized=false;

struct COM_DATA_STRUCT {
    uint8_t pendingCommand[BOOT_CONFIG_MAX_PACKET_SIZE];
    uint16_t pendingCommandLength;
    uint8_t responseBuffer[BOOT_CONFIG_MAX_PACKET_SIZE];
    uint16_t responseBufferLength;
};

struct COM_DATA_STRUCT canComData;

void BOOT_COM_Initialize()
{
    (void)memset(&canComData,0, sizeof(struct COM_DATA_STRUCT )/sizeof(uint8_t));
    initialized = true;
}

uint8_t BOOT_COM_Peek(uint16_t location)
{
    uint8_t response;
    
    if (location>= canComData.pendingCommandLength)
    {
        response =  0;
    } 
    else
    {
        response = canComData.pendingCommand[location];
    }
    
    return response;    
}

uint16_t BOOT_COM_Read(uint8_t* data, uint16_t length)
{
    uint16_t size;
    

    size = BOOT_COM_GetBytesReady();

    if (size > length)
    {
        size = length;
    }

    memcpy(data, canComData.pendingCommand, size);
  
    memcpy(canComData.pendingCommand, &canComData.pendingCommand[size], canComData.pendingCommandLength - size);

    canComData.pendingCommandLength   -= size;   
    
    return size;
};

void BOOT_COM_Write(uint8_t* data, uint16_t length)
{
    bool status;
    // The data to send will be copied to the responseBuffer and then the 
    // ISO-TP layer will drain the data from this buffer as it is sent over 
    // the CAN interface.
    
    if ( (data != NULL) && (length <= sizeof(canComData.responseBuffer) ) )
    {
        memcpy(canComData.responseBuffer, data, length);
        canComData.responseBufferLength = length;

        status = CAN_TP_MessageSend(canComData.responseBuffer, canComData.responseBufferLength);
        // failures here could be wrong msgId, freq, mode, or other physcal CAN failures
    } 
    else
    {
        // failures here are most likely pure program bugs.  Pointer and length.
        //"TELL USER, DON'T FAIL SILENTLY"
    }
 
};


uint16_t BOOT_COM_GetBytesReady()
{
    if (!initialized)
    {
        BOOT_COM_Initialize();
    }
      
    // The data received by the ISO-TP CAN layer will be copied to the 
    // pendingCommand buffer which is used by the boot loader.
    if ((CAN_TP_IsMessageReady() == true) && (canComData.pendingCommandLength == 0u))
    {
        canComData.pendingCommandLength = CAN_TP_MessageLengthGet();
        (void)CAN_TP_MessageGet(canComData.pendingCommand);
  
    }

    return canComData.pendingCommandLength;
}