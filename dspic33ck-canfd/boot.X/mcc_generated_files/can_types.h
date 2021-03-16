/**
  CAN Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    can_types.h

  @Summary
    This is the generated header file for the CAN driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides APIs for driver for CAN.
    Generation Information :
        Product Revision   :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.169.0
        Device             :  dsPIC33CK256MP508
    The generated drivers are tested against the following:
        Compiler           :  XC16 v1.50
        MPLAB              :  MPLAB X v5.40
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#ifndef CAN_TYPES_H
#define CAN_TYPES_H

/**
  Section: Included Files
*/

#include <stdint.h>

/**
  CAN Message Object data structure

  @Summary
    Defines the CAN Message Object data structure.

  @Description
    This Data structure is to implement a CAN FIFO message object.
*/
typedef union {
    uint8_t msgfields;
    struct{
        uint8_t idType:1;       // 1 bit (Standard Frame or Extended Frame)
        uint8_t frameType:1;    // 1 bit (Data Frame or RTR Frame)
        uint8_t dlc:4;          // 4 bit (No of data bytes a message frame contains)
        uint8_t formatType:1;   // 1 bit (CAN 2.0 Format or CAN_FD Format)
        uint8_t brs:1;          // 1 bit (Bit Rate Switch)
    };
} CAN_MSG_FIELD;

typedef struct 
{
    uint32_t msgId;          // 29 bit (SID: 11bit, EID:18bit)
    CAN_MSG_FIELD field;     // CAN TX/RX Message Object Control
    uint8_t *data;           // Pointer to message data
} CAN_MSG_OBJ;   

/**
  CAN Message Object Bit Rate Switch Selection Enumeration

  @Summary
    Defines the CAN message object bit rate switch selection enumeration.

  @Description
    This enumeration defines the CAN message object bit rate switch selection option.
*/
typedef enum 
{   
    CAN_NON_BRS_MODE    = 0,
    CAN_BRS_MODE        = 1     //Supported only in CAN FD mode
} CAN_MSG_OBJ_BRS_MODE;

/**
  CAN Message Object Identifier Selection Enumeration

  @Summary
    Defines the CAN message object identifier selection enumeration.

  @Description
    This enumeration defines the CAN message object identifier selection option.
*/
typedef enum 
{   
    CAN_FRAME_STD       = 0,
    CAN_FRAME_EXT       = 1,
} CAN_MSG_OBJ_ID_TYPE;

/**
  CAN Message Object Frame Type Selection Enumeration

  @Summary
    Defines the CAN message object frame type selection enumeration.

  @Description
    This enumeration defines the CAN message object frame type selection option.
*/
typedef enum 
{   
    CAN_FRAME_DATA      = 0,
    CAN_FRAME_RTR       = 1,
} CAN_MSG_OBJ_FRAME_TYPE;

/**
  CAN Message Object Format Type Selection Enumeration

  @Summary
    Defines the CAN message object format type selection enumeration.

  @Description
    This enumeration defines the CAN message object format type selection option.
*/
typedef enum 
{   
    CAN_2_0_FORMAT      = 0,
    CAN_FD_FORMAT       = 1     //Supported only in CAN FD mode
} CAN_MSG_OBJ_TYPE;

/**
  CAN Transmit Message Object Request Status Enumeration

  @Summary
    Defines the CAN transmit message object request status enumeration.

  @Description
    This enumeration defines the CAN transmit message object request status.
*/
typedef enum 
{ 
    CAN_TX_MSG_REQUEST_SUCCESS = 0,             // Transmit message object successfully placed into Transmit FIFO
    CAN_TX_MSG_REQUEST_DLC_EXCEED_ERROR = 1,    // Transmit message object DLC size is more than Transmit FIFO configured DLC size
    CAN_TX_MSG_REQUEST_BRS_ERROR = 2,           // Transmit FIFO is configured has Non BRS mode and CAN TX Message object has BRS enabled
    CAN_TX_MSG_REQUEST_FIFO_FULL = 3,           // Transmit FIFO is Full
} CAN_TX_MSG_REQUEST_STATUS;

/**
  CAN Operation Modes Enumeration

  @Summary
    Defines the CAN operation modes enumeration.

  @Description
    This enumeration defines the CAN different operating modes.
*/
typedef enum 
{
    CAN_NORMAL_FD_MODE = 0x0,    //Supported only in CAN FD mode
    CAN_DISABLE_MODE = 0x1,               
    CAN_INTERNAL_LOOPBACK_MODE = 0x2,
    CAN_LISTEN_ONLY_MODE = 0x3,
    CAN_CONFIGURATION_MODE = 0x4,
    CAN_EXTERNAL_LOOPBACK_MODE = 0x5,
    CAN_NORMAL_2_0_MODE = 0x6,
    CAN_RESTRICTED_OPERATION_MODE =0x7,
} CAN_OP_MODES;   

/**
  CAN Operation Modes Request Status Enumeration

  @Summary
    Defines the CAN operation modes request status enumeration.

  @Description
    This enumeration defines the CAN operation mode request status.
*/
typedef enum 
{
    CAN_OP_MODE_REQUEST_SUCCESS,     // Requested Operation mode set successfully
    CAN_OP_MODE_REQUEST_FAIL,        // Requested Operation mode set failure. Set configuration mode before setting CAN normal or debug operation mode.
    CAN_OP_MODE_SYS_ERROR_OCCURED    // System error occurred while setting Operation mode.
}  CAN_OP_MODE_STATUS;

/**
  CAN Transmit FIFO Status Enumeration

  @Summary
    Defines the CAN transmit FIFO status Enumeration.

  @Description
    This enumeration defines the CAN transmit FIFO status.
*/
typedef enum 
{
    CAN_TX_FIFO_FULL,
    CAN_TX_FIFO_AVAILABLE,
} CAN_TX_FIFO_STATUS;

/**
  CAN Message Object Data Length code Enumeration

  @Summary
    Defines the CAN message object data length code enumeration.

  @Description
    The Data Length Code (DLC) specifies the number of data bytes a message frame contains.
*/
typedef enum 
{
    /*DLC_0 to DLC_8 for CAN 2.0 and CAN FD*/
    DLC_0,
    DLC_1,
    DLC_2,
    DLC_3,
    DLC_4,
    DLC_5,
    DLC_6,
    DLC_7,
    DLC_8,

    //Supported only in CAN FD mode
    /*DLC_12 to DLC_64 for CAN FD*/ 
    DLC_12,
    DLC_16,
    DLC_20,
    DLC_24,
    DLC_32,
    DLC_48,
    DLC_64,
} CAN_DLC;

#endif  //_CAN_TYPES_H
/**
 End of File
*/

