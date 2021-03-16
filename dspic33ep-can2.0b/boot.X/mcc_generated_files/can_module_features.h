/**
  CAN Module Feature Header File

  @Company
    Microchip Technology Inc.

  @File Name
    can_module_features.h

  @Summary
    This is the generated module feature header file for CAN driver 
    using PIC24 / dsPIC33 / PIC32MM MCUs.
    This file provides module feature list available on the selected device. 
    The macros defined in this file provides the flexibility to easily migrate the user
    application to other device which might have varied feature list.
         
    The content in this file is strictly "read only" and should not be altered.

  @Description
    This header file provides module feature list for CAN driver.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  dsPIC33EP512MU810      
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB 	          :  MPLAB X v5.45
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

#ifndef CAN_MODULE_FEATURES
#define CAN_MODULE_FEATURES

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
        
/*******************************************************************************
********************************************************************************
******************************************************************************** 
                            ********IMPORTANT********

This file defines the CAN module feature C-macros available 
on the selected device. The macros defined in this file provides the 
flexibility to easily migrate the user application to other device which might 
have varied feature list.

The file has to be manually included in main.c, if the user intends to migrate 
the application to another device which might have varied feature list.
  
  
 ***************The content in this file is strictly "read only" and should 
 not be altered. Failing to do so, the migration is not guaranteed.*************

********************************************************************************
********************************************************************************         
********************************************************************************/

/*******************************************************************************
            Macros defined for features supported in the device
*******************************************************************************/

/** CAN 2.0 WITH DMA SUPPORT FEATURE
 
 @Summary 
   Defines the macro associated with CAN 2.0 module with DMA support.

 @APIs Supported:
   void CANx_TransmitEnable();
   void CANx_ReceiveEnable();
   
   x denotes instance of CAN in CANx. 
   Refer to device specific datasheet to check number of CAN module instance.
   Refer driver header file for detailed description of the APIs.
  
 @Remarks:
   None
 */ 
#define CAN_DMA_INTERFACE_FEATURE_AVAILABLE  1


/*******************************************************************************
            Macros defined for features not supported in the device
*******************************************************************************/

/** CAN FD MESSAGE FORMAT FEATURE
 
 @Summary 
   Defines the macro associated with CAN message object structure "CAN_MSG_OBJ".
   "brs" and "formatType" bit field variables are applicable only CAN FD message 
   format.
 
 @APIs Supported:
   None
  
 @Remarks:
   None
 */
#define CAN_FD_MESSAGE_FORMAT_FEATURE_AVAILABLE  0

/** CAN FIFO BASED TRANSMIT PRIORITY FEATURE
 
 @Summary 
   Defines the macro associated with CAN FD FIFO based transmit
   priority feature.
 
 @APIs Supported:
   None
  
 @Remarks:
   None
 */
#define CAN_FIFO_BASED_TRANSMIT_PRIORITY_FEATURE_AVAILABLE  0


#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif //CAN_MODULE_FEATURES