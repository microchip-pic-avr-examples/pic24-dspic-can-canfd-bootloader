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

#ifndef _CAN1_H
#define _CAN1_H

/**
  Section: Included Files
*/

#include <stdbool.h>
#include "can_types.h"

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

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
        CAN1_TransmitEnable();
        CAN1_ReceiveEnable();
        
        while(1)
        {      
            
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
    CAN_OP_MODE_REQUEST_SUCCESS     - Requested Operation mode set successfully
    CAN_OP_MODE_REQUEST_FAIL        - Requested Operation mode set failure. Set configuration 
                                      mode before setting CAN normal or debug operation mode.
    CAN_OP_MODE_SYS_ERROR_OCCURED   - System error occurred while setting Operation mode.

  @Example
    <code>
    int main()
    {
        // initialize the device
        SYSTEM_Initialize();
        CAN1_TransmitEnable();
        CAN1_ReceiveEnable();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                //User Application code
            }
        }

        while(1);      
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
        CAN1_TransmitEnable();
        CAN1_ReceiveEnable();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
            
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                mode = CAN1_OperationModeGet();
                //User Application code
            }
        }
    
        while(1);      
        return 0;
    }
    </code> 
*/
CAN_OP_MODES CAN1_OperationModeGet(void);

/**
  @Summary
    Transmits the message object from user buffer to CAN1 Transmit buffer.

  @Description
    This routine Writes a message object to the CAN1 Transmit buffer.

  @Preconditions
    CAN1_Initialize function should be called before calling this function. 
    The transfer status should be checked to see if transmitter is not full 
    before calling this function.

  @Param
    priority     - priority of the message to be transmitted
    sendCanMsg   - pointer to the message object

  @Returns
    CAN1 Transmit message status.
    CAN_TX_MSG_REQUEST_SUCCESS          - Transmit message object successfully placed into Transmit Buffer
    CAN_TX_MSG_REQUEST_DLC_ERROR        - Transmit message object DLC size is more than Transmit Buffer configured DLC size
    CAN_TX_MSG_REQUEST_BUFFER_FULL      - Transmit Buffer is Full

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
        CAN1_TransmitEnable();
        CAN1_ReceiveEnable();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = CAN_DLC_8;
                msg.data = data;
            
                CAN1_Transmit(CAN_PRIORITY_HIGH, &msg);
            }
        }

        while(1);
        return 0;
    }
    </code>
*/
CAN_TX_MSG_REQUEST_STATUS CAN1_Transmit(CAN_TX_PRIOIRTY priority, CAN_MSG_OBJ *sendCanMsg);

/**
  @Summary
    Reads the message object from CAN1 receive buffer.

  @Description
    This routine reads a message object from the CAN1 receive buffer.

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
    //Note: Example code here is not based on MCC UI configuration, 
    //      this is a sample code to demonstrate CAN1 Receive APIs usage. 
    //      The user must have to allocate 8 bytes of space for CAN1 Receive message object data. 
    
    int main(void) 
    {
        uint8_t data[8] = {0};
        CAN_MSG_OBJ msg;
        msg.data = data;
     
        // initialize the device
        SYSTEM_Initialize();
        CAN1_TransmitEnable();
        CAN1_ReceiveEnable();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
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
                }
            }
        }

        while(1);        
        return 0;
    }
    </code>
*/
bool CAN1_Receive(CAN_MSG_OBJ *recCanMsg);

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
    //      this is a sample code to demonstrate CAN1 transmit APIs usage.

    int main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};
        
        // initialize the device
        SYSTEM_Initialize();
        CAN1_TransmitEnable();
        CAN1_ReceiveEnable();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = CAN_DLC_8;
                msg.data = data;
            
                if(CAN1_IsBusOff() == false)
                {
                    CAN1_Transmit(CAN_PRIORITY_HIGH, &msg);
                }
            }
        }
        
        while(1);
        return 0;
    }
    </code>
*/
bool CAN1_IsBusOff();

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
        uint8_t data[8] = {0};
        CAN_MSG_OBJ msg;
        msg.data = data;
     
        // initialize the device
        SYSTEM_Initialize();
        CAN1_TransmitEnable();
        CAN1_ReceiveEnable();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
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
                }
            }
        }
        
        while(1);
        return 0;
    }
    </code>
*/
bool CAN1_IsRXErrorPassive();

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
        CAN1_TransmitEnable();
        CAN1_ReceiveEnable();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
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
                }
            }
        }
        
        while(1);
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
        CAN1_TransmitEnable();
        CAN1_ReceiveEnable();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
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
                }
            }
        }

        while (1);        
        return 0;
    }
    </code>
*/
bool CAN1_IsRxErrorActive(void);

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
    // sample code to demonstrate CAN1 transmit error passive APIs usage.

    int main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};
        
        // initialize the device
        SYSTEM_Initialize();
        CAN1_TransmitEnable();
        CAN1_ReceiveEnable();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                msg.msgId = 0x1FFFF;
                
                
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = CAN_DLC_8;
                msg.data = data;
            
                if(CAN1_IsTxErrorPassive() == false)
                {
                    CAN1_Transmit(CAN_PRIORITY_HIGH, &msg);
                }
            }
        }
        
        while(1);
        return 0;
    }
    </code>
*/
bool CAN1_IsTXErrorPassive();

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
        CAN1_TransmitEnable();
        CAN1_ReceiveEnable();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = DLC_8;
                msg.data = data;
            
                if(CAN1_IsTxErrorWarning() == false)
                {
                    CAN1_Transmit(CAN_PRIORITY_HIGH, &msg);
                }
            }
        }
        
        while (1);
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
        CAN1_TransmitEnable();
        CAN1_ReceiveEnable();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);

        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                msg.msgId = 0x1FFFF;
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
        
        while (1);
        return 0;
    }
    </code>
*/
bool CAN1_IsTxErrorActive(void);

/**
  @Summary
    CAN1 Return the number of messages object that are received in RX buffer.

  @Description
    This returns the number of messages object that are received RX buffer.

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
        uint8_t data[8] = {0};
        CAN_MSG_OBJ msg;
        msg.data = data;
     
        // initialize the device
        SYSTEM_Initialize();
        CAN1_TransmitEnable();
        CAN1_ReceiveEnable();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);

        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                while(1) 
                {
                    if(CAN1_ReceivedMessageCountGet() > 0) 
                    {
                        CAN1_Receive(&msg);
                    }
                }
            }
        }

        while(1);
        return 0;
    }
    </code>
*/
uint8_t CAN1_ReceivedMessageCountGet();

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
        CAN1_TransmitEnable();
        CAN1_ReceiveEnable();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
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
                        if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
                        {
                            //User Application code                            
                        }
                    }
                }
            }
        }
        
        while(1);
        return 0;
    }
    </code>
*/
void CAN1_Sleep();

/**
  @Summary
    Configure the CAN1 Transmit DMA buffer.

  @Description
    This routine configures the CAN1 Transmit DMA buffer and DMA buffer configuration 
    is mandatory forCAN1 2.0 module. Before calling CAN1_Transmit function, 
    CAN1_TransmitEnable function has to be called.

  @Preconditions
    CAN1_Initialize() function should be called before calling this function. 
 
  @Param
    None

  @Returns
    None 

  @Example
    <code>
    int main()
    {
        CAN_OP_MODES mode;
        
        // initialize the device
        SYSTEM_Initialize();
        CAN1_TransmitEnable();
        CAN1_ReceiveEnable();

        while(1);
        return 0;
    }
    </code> 
*/
void CAN1_TransmitEnable();

/**
  @Summary
    Configure the CAN1 Receive DMA buffer.

  @Description
    This routine configures the CAN1 Receive DMA buffer and DMA buffer configuration 
    is mandatory forCAN1 2.0 module. Before calling CAN1_Receive function, 
    CAN1_ReceiveEnable function has to be called.

  @Preconditions
    CAN1_Initialize() function should be called before calling this function. 
 
  @Param
    None

  @Returns
    None 

  @Example
    <code>
    int main()
    {
        CAN_OP_MODES mode;
        
        // initialize the device
        SYSTEM_Initialize();
        CAN1_TransmitEnable();
        CAN1_ReceiveEnable();

        while(1);      
        return 0;
    }
    </code> 
*/
void CAN1_ReceiveEnable();

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
    }
 
    int main(void) 
    {
        // initialize the device
        SYSTEM_Initialize();
        CAN1_TransmitEnable();
        CAN1_ReceiveEnable();
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
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
                    }
                }
            }
        }
        
        while (1);        
        return 0;
    }
    </code>
*/
void CAN1_DefaultBusWakeUpActivityHandler(void);

/**
  @Summary
    Assigns a function pointer with a CAN1 Bus Wake-Up activity callback address.

  @Description
    This routine assigns a function pointer with a CAN1 Bus Wake-Up activity callback address.

  @Param
    Address of the callback routine.

  @Returns
    None
 
  @Example 
    <code>
    bool gBusWakeUpOccurred = false;
    
    void CAN1_BusWakeUpActivity(void)
    {
        gBusWakeUpOccurred = true;
        //CAN Bus WakeUp activity application code
    }
 
    int main(void) 
    {
        // initialize the device
        SYSTEM_Initialize();
        CAN1_TransmitEnable();
        CAN1_ReceiveEnable();
        CAN1_SetBusWakeUpActivityInterruptHandler(&CAN1_BusWakeUpActivity);
        CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
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
                    }
                }
            }
        }
        
        while(1);
        return 0;
    }
    </code>
*/
void CAN1_SetBusWakeUpActivityInterruptHandler(void *handler);

/*******************************************************************************

  !!! Deprecated Definitions and APIs !!!
  !!! These functions will not be supported in future releases !!!

*******************************************************************************/

/******************************************************************************
*                                                                             
*    Function:		CAN1_receive
*    Description:       Receives the message from CAN1 buffer to user buffer                                                                         
*    Arguments:		recCanMsg: pointer to the message object
*    Return Value:      true - Receive successful
*                       false - Receive failure                                                                              
******************************************************************************/
bool __attribute__((deprecated("\nThis will be removed in future MCC releases. "
"\nUse CAN1_Receive instead."))) CAN1_receive(uCAN_MSG *recCanMsg);

/******************************************************************************
*                                                                             
*    Function:		CAN1_transmit
*    Description:       Transmits the message from user buffer to CAN1 buffer                                                                        
*    Arguments:		priority: priority of the message to be transmitted
*                       sendCanMsg: pointer to the message object                                        
*    Return Value:      true - Transmit successful
*                       false - Transmit failure                                                                              
******************************************************************************/
bool __attribute__((deprecated("\nThis will be removed in future MCC releases. "
"\nUse CAN1_Transmit instead."))) CAN1_transmit(CAN_TX_PRIOIRTY priority, 
                                    uCAN_MSG *sendCanMsg);

/******************************************************************************                                                                       
*    Function:          CAN1_isBusOff
*    Description:       Checks whether the transmitter in Bus off state                                                                                                   
*    Return Value:      true - Transmitter in Bus Off state
*                       false - Transmitter not in Bus Off state                                                                              
******************************************************************************/
bool __attribute__((deprecated("\nThis will be removed in future MCC releases. "
                              "\nUse CAN1_IsBusOff instead."))) CAN1_isBusOff();

/******************************************************************************                                                                 
*    Function:		CAN1_isRXErrorPassive
*    Description:       Checks whether the receive in error passive state                                    
*    Return Value:      true - Receiver in Error Passive state
*                       false - Receiver not in Error Passive state                                                                              
******************************************************************************/
bool __attribute__((deprecated("\nThis will be removed in future MCC releases. "
                              "\nUse CAN1_IsRXErrorPassive instead."))) CAN1_isRXErrorPassive();

/******************************************************************************                                                                      
*    Function:		CAN1_isTXErrorPassive
*    Description:       Checks whether the transmitter in error passive state                                                                                                             
*    Return Value:      true - Transmitter in Error Passive state
*                       false - Transmitter not in Error Passive state                                                                              
******************************************************************************/
bool __attribute__((deprecated("\nThis will be removed in future MCC releases. "
                              "\nUse CAN1_IsTXErrorPassive instead."))) CAN1_isTXErrorPassive();

/******************************************************************************                                                                    
*    Function:		CAN1_messagesInBuffer
*    Description:       returns the number of messages that are received                                                                                                               
*    Return Value:      Number of message received
******************************************************************************/
uint8_t __attribute__((deprecated("\nThis will be removed in future MCC releases. "
       "\nUse CAN1_ReceivedMessageCountGet instead."))) CAN1_messagesInBuffer();

/******************************************************************************
*                                                                             
*    Function:		CAN1_sleep
*    Description:       Puts CAN1 module in disable mode.
*    Return Value:      None
*                                                                       
******************************************************************************/
void __attribute__((deprecated("\nThis will be removed in future MCC releases. "
                                    "\nUse CAN1_Sleep instead."))) CAN1_sleep();
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif  //_CAN1_H
/**
 End of File
*/

