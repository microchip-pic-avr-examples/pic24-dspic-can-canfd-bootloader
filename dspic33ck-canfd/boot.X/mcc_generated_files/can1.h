/**
  CAN1 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    can1.h

  @Summary
    This is the generated header file for the CAN1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides APIs for driver for CAN1.
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

#ifndef CAN1_H
#define CAN1_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "can_types.h"

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

// Transmit FIFO's Custom Name
#define CAN1_TX_FIFO1   CAN1_FIFO_1

typedef enum 
{
    CAN1_FIFO_1 = 1,
} CAN1_TX_FIFO_CHANNELS; 

/**
  Section: CAN1 Module APIs
*/

/**
  @Summary
    Initialization routine that takes inputs from the CAN1 UI

  @Description
    This routine initializes the CAN1 module.
    This routine must be called before any other CAN1 routine is called.
    This routine should only be called once during system initialization.

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Example
    <code>
    int main()
    {
        // initialize the device
        SYSTEM_Initialize();
   
        while (1)
        {      
            CAN1_Tasks();
        }
      
        return 0;
    }
    </code> 
*/
void CAN1_Initialize(void);

/**
  @Summary
    Set the CAN1 operation mode

  @Description
    This function set the CAN1 operation mode.

  @Preconditions
    CAN1_Initialize() function should be called before calling this function.
 
  @Param
    requestMode - CAN1 operation modes

  @Returns
    CAN1 Operation mode request to set.
    CAN_OP_MODE_REQUEST_SUCCESS - Requested Operation mode set successfully
    CAN_OP_MODE_REQUEST_FAIL - Requested Operation mode set failure. Set configuration 
                               mode before setting CAN normal or debug operation mode.
    CAN_OP_MODE_SYS_ERROR_OCCURED - System error occurred while setting Operation mode.

  @Example
    <code>
    int main()
    {
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                //User Application code
            }
        }

        while (1)
        {      
            CAN1_Tasks();
        }
      
        return 0;
    }
    </code> 
*/
CAN_OP_MODE_STATUS CAN1_OperationModeSet(const CAN_OP_MODES requestMode);

/**
  @Summary
    Get the CAN1 operation mode

  @Description
    This function get the CAN1 operation mode.

  @Preconditions
    CAN1_Initialize() function should be called before calling this function. 
 
  @Param
    None

  @Returns
    Return the present CAN1 operation mode. 

  @Example
    <code>
    int main()
    {
        CAN_OP_MODES mode;
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
            
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                mode = CAN1_OperationModeGet();
                //User Application code
            }
        }
    
        while (1)
        {      
            CAN1_Tasks();
        }
      
        return 0;
    }
    </code> 
*/
CAN_OP_MODES CAN1_OperationModeGet(void);

/**
  @Summary
    Reads the message object from CAN1 receive FIFO.

  @Description
    This routine reads a message object from the CAN1 receive FIFO.

  @Preconditions
    CAN1_Initialize() function should be called before calling this function. 
    The CAN1_ReceivedMessageCountGet() function should be checked to see if the receiver
    is not empty before calling this function.

  @Param
    rxCanMsg    - pointer to the message object

  @Returns
    true        - Receive successful
    false       - Receive failure

  @Example
    <code>
    int main(void) 
    {
        CAN_MSG_OBJ msg;
     
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                while(1) 
                {
                    if(CAN1_ReceivedMessageCountGet() > 0) 
                    {
                        if(true == CAN1_Receive(&msg))
                        {
                            break;
                        }
                    }

                    CAN1_Tasks();
                }
            }
        }

        while (1)
        {
        }
        
        return 0;
    }
    </code>
*/
bool CAN1_Receive(CAN_MSG_OBJ *rxCanMsg);

/**
  @Summary
    Transmits the message object from user buffer to CAN1 Transmit FIFO.

  @Description
    This routine Writes a message object to the CAN1 Transmit FIFO.

  @Preconditions
    CAN1_Initialize function should be called before calling this function. 
    The transfer status should be checked to see if transmitter is not full 
    before calling this function.

  @Param
    fifoChannel - CAN1 Transmit priority FIFO selection
    txCanMsg     - pointer to the message object

  @Returns
    CAN1 Transmit message status.
    CAN_TX_MSG_REQUEST_SUCCESS - Transmit message object successfully placed into transmit FIFO
    CAN_TX_MSG_REQUEST_DLC_EXCEED_ERROR - Transmit message object DLC size is more than transmit FIFO configured DLC size
    CAN_TX_MSG_REQUEST_BRS_ERROR - Transmit FIFO configured has Non BRS mode and CAN Transmit Message object has BRS enabled
    CAN_TX_MSG_REQUEST_FIFO_FULL - Transmit FIFO Full

  @Example
    <code>
    //Note: Example code here is not based on MCC UI configuration, 
    //      this is a sample code to demonstrate CAN transmit APIs usage.
    
    int main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.formatType = CAN_FD_FORMAT;
                msg.field.brs = CAN_NON_BRS_MODE;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = DLC_8;
                msg.data = data;
            
                if(CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TX_FIFO1) & CAN_TX_FIFO_AVAILABLE))
                {
                    CAN1_Transmit(CAN1_TX_FIFO1, &msg);
                }
            }
        }

        while (1)
        {
            CAN1_Tasks();
        }
        return 0;
    }
    </code>
*/
CAN_TX_MSG_REQUEST_STATUS CAN1_Transmit(const CAN1_TX_FIFO_CHANNELS fifoChannel, CAN_MSG_OBJ *txCanMsg);

/**
  @Summary
    CAN1 transmitter FIFO status.

  @Description
    This returns the CAN1 transmitter FIFO status. The returned status 
    contains a transmit FIFO status.

  @Preconditions
    CAN1_Initialize function should be called before calling this function.

  @Param
    fifoChannel - CAN1 TX priority FIFO selection

  @Returns
    CAN1 Transmit FIFO status.
    CAN_TX_FIFO_FULL         - CAN Transmit FIFO is full
    CAN_TX_FIFO_AVAILABLE     - CAN Transmit FIFO message space is available

  @Example
    <code>
    //Note: Example code here is not based on MCC UI configuration, 
    //      this is a sample code to demonstrate CAN transmit APIs usage.
    
    int main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);

        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {    
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.formatType = CAN_FD_FORMAT;
                msg.field.brs = CAN_NON_BRS_MODE;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = DLC_8;
                msg.data = data;
            
                if(CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TX_FIFO1) & CAN_TX_FIFO_AVAILABLE))
                {
                    CAN1_Transmit(CAN1_TX_FIFO1, &msg);
                }
            }
        }
        
        while (1)
        {
            CAN1_Tasks();
        }
        
        return 0;
    }
    </code>
*/
CAN_TX_FIFO_STATUS CAN1_TransmitFIFOStatusGet(const CAN1_TX_FIFO_CHANNELS fifoChannel);

/**
  @Summary
    Checks whether the transmitter is in bus off state.

  @Description
    This routine checks whether the transmitter is in bus off state.

  @Preconditions
    CAN1_Initialize function should be called before calling this function.

  @Param
    None

  @Returns
    true    - Transmitter in Bus Off state
    false   - Transmitter not in Bus Off state

  @Example
    <code>
    //Note: Example code here is not based on MCC UI configuration, 
    //      this is a sample code to demonstrate CAN transmit APIs usage.

    int main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.formatType = CAN_FD_FORMAT;
                msg.field.brs = CAN_NON_BRS_MODE;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = DLC_8;
                msg.data = data;
            
                if(CAN1_IsBusOff() == false)
                {
                    if(CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TX_FIFO1) & CAN_TX_FIFO_AVAILABLE))
                    {
                        CAN1_Transmit(CAN1_TX_FIFO1, &msg);
                    }
                }
            }
        }
        
        while (1)
        {
            CAN1_Tasks();
        }
        return 0;
    }
    </code>
*/
bool CAN1_IsBusOff(void);

/**
  @Summary
    Checks whether the transmitter is in the error passive state.

  @Description
    This routine checks whether the transmitter is in the error passive state.
    If Transmitter error counter is above 127, then the transmitter error passive 
    state is set.

  @Preconditions
    CAN1_Initialize function should be called before calling this function.

  @Param
    None

  @Returns
    true    - Transmitter in Error Passive state
    false   - Transmitter not in Error Passive state

  @Example
    <code>
    //Note: Example code here is not based on MCC UI configuration, 
    // sample code to demonstrate CAN transmit error passive APIs usage.

    int main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.formatType = CAN_FD_FORMAT;
                msg.field.brs = CAN_NON_BRS_MODE;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = DLC_8;
                msg.data = data;
            
                if(CAN1_IsTxErrorPassive() == false)
                {
                    if(CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TX_FIFO1) & CAN_TX_FIFO_AVAILABLE))
                    {
                        CAN1_Transmit(CAN1_TX_FIFO1, &msg);
                    }
                }
            }
        }
        
        while (1)
        {
            CAN1_Tasks();
        }
        return 0;
    }
    </code>
*/
bool CAN1_IsTxErrorPassive(void);

/**
  @Summary
    Checks whether the transmitter is in the error warning state.

  @Description
    This routine checks whether the transmitter is in the error warning state.
    If Transmitter error counter is above 95 to below 128, then transmitter error 
    warning state is set.

  @Preconditions
    CAN1_Initialize function should be called 
    before calling this function.

  @Param
    None

  @Returns
    true    - Transmitter in Error warning state
    false   - Transmitter not in Error warning state

  @Example
    <code>
    //Note: Example code here is not based on MCC UI configuration, 
    //      sample code to demonstrate CAN transmit error warning APIs usage.

    int main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.formatType = CAN_FD_FORMAT;
                msg.field.brs = CAN_NON_BRS_MODE;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = DLC_8;
                msg.data = data;
            
                if(CAN1_IsTxErrorWarning() == false)
                {
                    if(CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TX_FIFO1) & CAN_TX_FIFO_AVAILABLE))
                    {
                        CAN1_Transmit(CAN1_TX_FIFO1, &msg);
                    }
                }
            }
        }
        
        while (1)
        {
            CAN1_Tasks();
        }
        return 0;
    }
    </code>
*/
bool CAN1_IsTxErrorWarning(void);

/**
  @Summary
    Checks whether the transmitter is in the error active state.

  @Description
    This routine checks whether the transmitter is in the error active state.
    If Transmitter error counter is above 0 to below 128, then transmitter error 
    active state is set.

  @Preconditions
    CAN1_Initialize function should be called 
    before calling this function.

  @Param
    None

  @Returns
    true    - Transmitter in Error active state
    false   - Transmitter not in Error active state

  @Example
    <code>
    //Note: Example code here is not based on MCC UI configuration, 
    //      sample code to demonstrate CAN transmit error active APIs usage.

    int main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);

        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.formatType = CAN_FD_FORMAT;
                msg.field.brs = CAN_NON_BRS_MODE;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = DLC_8;
                msg.data = data;
            
                if(CAN1_IsTxErrorActive() == false)
                {
                    if(CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TX_FIFO1) & CAN_TX_FIFO_AVAILABLE))
                    {
                        CAN1_Transmit(CAN1_TX_FIFO1, &msg);
                    }
                }
            }
        }
        
        while (1)
        {
            CAN1_Tasks();
        }
        return 0;
    }
    </code>
*/
bool CAN1_IsTxErrorActive(void);

/**
  @Summary
    Checks whether the Receiver is in the error passive state.

  @Description
    This routine checks whether the receive is in the error passive state.
    If Receiver error counter is above 127, then receiver error passive 
    state is set.

  @Preconditions
    CAN1_Initialize function should be called before calling this function.

  @Param
    None

  @Returns
    true    - Receiver in Error passive state
    false   - Receiver not in Error passive state

  @Example
    <code>
    int main(void) 
    {
        CAN_MSG_OBJ msg;
     
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                while(1) 
                {
                    if(CAN1_IsRxErrorPassive() == false)
                    {
                        if(CAN1_ReceivedMessageCountGet() > 0) 
                        {
                            CAN1_Receive(&msg);
                        }
                    }

                    CAN1_Tasks();
                }
            }
        }
        
        while (1)
        {
        }
        
        return 0;
    }
    </code>
*/
bool CAN1_IsRxErrorPassive(void);

/**
  @Summary
    Checks whether the Receiver is in the error warning state.

  @Description
    This routine checks whether the receive is in the error warning state.
    If Receiver error counter is above 95 to below 128, then receiver error warning 
    state is set.

  @Preconditions
    CAN1_Initialize function should be called before calling this function.

  @Param
    None

  @Returns
    true    - Receiver in Error warning state
    false   - Receiver not in Error warning state

  @Example
    <code>
    int main(void) 
    {
        CAN_MSG_OBJ msg;
     
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                while(1) 
                {
                    if(CAN1_IsRxErrorWarning() == false)
                    {
                        if(CAN1_ReceivedMessageCountGet() > 0) 
                        {
                            CAN1_Receive(&msg);
                        }
                    }

                    CAN1_Tasks();
                }
            }
        }

        while (1)
        {
        }
        
        return 0;
    }
    </code>
*/
bool CAN1_IsRxErrorWarning(void);

/**
  @Summary
    Checks whether the Receiver is in the error active state.

  @Description
    This routine checks whether the receive is in the error active state.
    If Receiver error counter is above 0 to below 128, then receiver error active 
    state is set.

  @Preconditions
    CAN1_Initialize function should be called before calling this function.

  @Param
    None

  @Returns
    true    - Receiver in Error active state
    false   - Receiver not in Error active state

  @Example
    <code>
    int main(void) 
    {
        CAN_MSG_OBJ msg;
     
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                while(1) 
                {
                    if(CAN1_IsRxErrorActive() == false)
                    {
                        if(CAN1_ReceivedMessageCountGet() > 0) 
                        {
                            CAN1_Receive(&msg);
                        }
                    }

                    CAN1_Tasks();
                }
            }
        }

        while (1)
        {
        }
        
        return 0;
    }
    </code>
*/
bool CAN1_IsRxErrorActive(void);

/**
  @Summary
    Puts CAN1 module in sleep mode.

  @Description
    This routine puts CAN1 module in sleep mode.

  @Preconditions
    CAN1_Initialize function should be called before calling this function.

  @Param
    None

  @Returns
    None

  @Example
    <code>
    int main(void) 
    {     
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                CAN1_Sleep();
                
                //Check CAN1 module is in CAN_DISABLE_MODE
                if(CAN_DISABLE_MODE == CAN1_OperationModeGet())
                {
                    Sleep(); //Call sleep instruction
                    C1INTLbits.WAKIF = 0; // Clear Wake-Up interrupt flag
                    
                    // Recover to Normal mode
                    if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_CONFIGURATION_MODE))
                    {
                        if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
                        {
                            //User Application code                            
                        }
                    }
                }
            }
        }
        
        while (1)
        {
            CAN1_Tasks();
        }
        
        return 0;
    }
    </code>
*/
void CAN1_Sleep();

/**
  @Summary
    CAN1 RX FIFO number of messages that are received.

  @Description
    This returns the number of messages that are received.

  @Preconditions
    CAN1_Initialize function should be called before calling this function.

  @Param
     None

  @Returns
    Number of message received.

  @Example
    <code>
    int main(void) 
    {
        CAN_MSG_OBJ msg;
     
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);

        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                while(1) 
                {
                    if(CAN1_ReceivedMessageCountGet() > 0) 
                    {
                        CAN1_Receive(&msg);
                    }

                    CAN1_Tasks();
                }
            }
        }

        while (1)
        {
        }
        
        return 0;
    }
    </code>
*/
uint8_t CAN1_ReceivedMessageCountGet(void);

/**
  @Summary
    Callback for CAN1 invalid message.

  @Description
    This routine is callback for CAN1 invalid message

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
    //Note: Example code here is not based on MCC UI configuration, 
    //      this is a sample code to demonstrate CAN transmit APIs usage.
    
    bool gInvalidMsgOccurred = false;
    
    void CAN1_DefaultInvalidMessageHandler(void)
    {
        gInvalidMsgOccurred = true;
        //CAN Invalid Message application code
    }
 
    int main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};
     
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);

        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {    
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.formatType = CAN_FD_FORMAT;
                msg.field.brs = CAN_NON_BRS_MODE;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = DLC_8;
                msg.data = data;

                while(1)
                {            
                    if(CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TX_FIFO1) & CAN_TX_FIFO_AVAILABLE))
                    {
                        CAN1_Transmit(CAN1_TX_FIFO1, &msg);
                    }
                    
                    if(gInvalidMsgOccurred == true)
                    {
                        break;
                    }

                    CAN1_Tasks();
                }
            }
        }
        
        while (1)
        {
        }
        
        return 0;
    }
    </code>
*/
void CAN1_DefaultInvalidMessageHandler(void);

/**
  @Summary
    Callback for CAN1 Bus WakeUp activity.

  @Description
    This routine is callback for CAN1 bus wakeUp activity

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
    bool gBusWakeUpOccurred = false;
    
    void CAN1_DefaultBusWakeUpActivityHandler(void)
    {
        gBusWakeUpOccurred = true;
        //CAN Bus WakeUp activity application code
    }
 
    int main(void) 
    {
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                CAN1_Sleep();
                            
                //Check CAN1 module is in CAN_DISABLE_MODE
                if(CAN_DISABLE_MODE == CAN1_OperationModeGet())
                {
                    Sleep(); //Call sleep instruction
                    
                    while(1) 
                    {
                        if(gBusWakeUpOccurred == true)
                        {
                            break;
                        }                        
                        
                        CAN1_Tasks();
                    }
                }
            }
        }
        
        while (1)
        {
        }
        
        return 0;
    }
    </code>
*/
void CAN1_DefaultBusWakeUpActivityHandler(void);

/**
  @Summary
    Callback for CAN1 Bus Error.

  @Description
    This routine is callback for CAN1 bus error

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
    //Note: Example code here is not based on MCC UI configuration, 
    //      this is a sample code to demonstrate CAN transmit APIs usage.
    
    bool gBusErrorOccurred = false;
    
    void CAN1_DefaultBusErrorHandler(void)
    {
        gBusErrorOccurred = true;
        //CAN Bus Error application code
    }
 
    int main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};
     
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);

        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {    
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.formatType = CAN_FD_FORMAT;
                msg.field.brs = CAN_NON_BRS_MODE;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = DLC_8;
                msg.data = data;

                while(1)
                {            
                    if(CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TX_FIFO1) & CAN_TX_FIFO_AVAILABLE))
                    {
                        CAN1_Transmit(CAN1_TX_FIFO1, &msg);
                    }
                    
                    if(gBusErrorOccurred == true)
                    {
                        break;
                    }
                
                    CAN1_Tasks();
                }
            }
        }
        
        while (1)
        {
        }
        
        return 0;
    }
    </code>
*/
void CAN1_DefaultBusErrorHandler(void);

/**
  @Summary
    Callback for CAN1 Mode Change.

  @Description
    This routine is callback for CAN1 mode change

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
    bool gModeChangeOccurred = false;
    
    void CAN1_DefaultModeChangeHandler(void)
    {
        gModeChangeOccurred = true;
        //CAN Mode Change application code
    }
 
    int main(void) 
    {
        CAN_MSG_OBJ msg;
     
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                while(1) 
                {
                    if(gModeChangeOccurred == true)
                    {
                        break;
                    }                    

                    CAN1_Tasks();
                }
            }
        }

        while (1)
        {
        }
        
        return 0;
    }
    </code>
*/
void CAN1_DefaultModeChangeHandler(void);

/**
  @Summary
    Callback for CAN1 System Error.

  @Description
    This routine is callback for CAN1 system error

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
    //Note: Example code here is not based on MCC UI configuration, 
    //      this is a sample code to demonstrate CAN transmit APIs usage.
    
    bool gSystemOccurred = false;
    
    void CAN1_DefaultSystemErrorHandler(void)
    {
        gSystemOccurred = true;
        //CAN System Error application code
    }
 
    int main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};
         
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);

        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {    
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.formatType = CAN_FD_FORMAT;
                msg.field.brs = CAN_NON_BRS_MODE;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = DLC_8;
                msg.data = data;

                while(1)
                {            
                    if(CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TX_FIFO1) & CAN_TX_FIFO_AVAILABLE))
                    {
                        CAN1_Transmit(CAN1_TX_FIFO1, &msg);
                    }
                    
                    if(gSystemOccurred == true)
                    {
                        break;
                    }

                    CAN1_Tasks();
                }
                
            }
        }
        
        while (1)
        {
        }
        
        return 0;
    }
    </code>
*/
void CAN1_DefaultSystemErrorHandler(void);

/**
  @Summary
    Callback for CAN1 Transmit Attempt.

  @Description
    This routine is callback for CAN1 transmit attempt 

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
    //Note: Example code here is not based on MCC UI configuration, 
    //      this is a sample code to demonstrate CAN transmit APIs usage.
    
    bool gTxAttemptOccurred = false;
    
    void CAN1_DefaultTxAttemptHandler(void)
    {
        gTxAttemptOccurred = true;
        //CAN Transmit Attempt application code
    }
 
    int main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};
        
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);

        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {    
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.formatType = CAN_FD_FORMAT;
                msg.field.brs = CAN_NON_BRS_MODE;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = DLC_8;
                msg.data = data;

                while(1)
                {                               
                    if(CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TX_FIFO1) & CAN_TX_FIFO_AVAILABLE))
                    {
                        CAN1_Transmit(CAN1_TX_FIFO1, &msg);
                    }
                    
                    if(gTxAttemptOccurred == true)
                    {
                        break;
                    }
                    
                    CAN1_Tasks();
                }
            }
        }
        
        while (1)
        {
        }
        
        return 0;
    }
    </code>
*/
void CAN1_DefaultTxAttemptHandler(void);

/**
  @Summary
    Callback for CAN1 Receive Buffer OverFlow .

  @Description
    This routine is callback for CAN1 receive buffer overFlow  

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
    bool gRxOverFlowOccurred = false;
    
    void CAN1_DefaultRxBufferOverFlowHandler(void)
    {
        gRxOverFlowOccurred = true;
        //CAN Receive Buffer OverFlow application code
    }
 
    int main(void) 
    {    
        // initialize the device
        SYSTEM_Initialize();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);

        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {    
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_FD_MODE))
            {
                while(1) 
                {
                    if(gRxOverFlowOccurred == true)
                    {
                        gRxOverFlowOccurred = false;
                        // User Application code
                        break;
                        
                    }
                }

                CAN1_Tasks();
            }
        }
        
        while (1)
        {
        }
        
        return 0;
    }
    </code>
*/
void CAN1_DefaultRxBufferOverFlowHandler(void);

/**
  @Summary		
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    CAN1_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to CAN1_Initialize() for an example
    
*/
void CAN1_Tasks(void);

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif  //_CAN1_H
/**
 End of File
*/

