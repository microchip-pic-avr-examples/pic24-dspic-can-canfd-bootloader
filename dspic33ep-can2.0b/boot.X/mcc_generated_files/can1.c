/**
  CAN1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    can1.c

  @Summary
    This is the generated driver implementation file for the CAN1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for CAN1.
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

/**
  Section: Included Files
*/

#include "can1.h"
#include "dma.h"

#define CAN1_TX_DMA_CHANNEL DMA_CHANNEL_0
#define CAN1_RX_DMA_CHANNEL DMA_CHANNEL_1

/* Valid options are 4, 6, 8, 12, 16, 24, or 32. */
#define CAN1_MESSAGE_BUFFERS         8

#define CAN1_TX_BUFFER_COUNT 1

#define CAN1_RX_BUFFER_MSG_DATA_SIZE    8U   // CAN RX Buffer Message object data field size

/******************************************************************************/

/******************************************************************************/
/* Private type definitions                                                   */
/******************************************************************************/
typedef struct __attribute__((packed))
{
    unsigned priority                   :2;
    unsigned remote_transmit_enable     :1;
    unsigned send_request               :1;
    unsigned error                      :1;
    unsigned lost_arbitration           :1;
    unsigned message_aborted            :1;
    unsigned transmit_enabled           :1;
} CAN1_TX_CONTROLS;


/**
 Section: Private Variable Definitions
*/

/* This alignment is required because of the DMA's peripheral indirect 
 * addressing mode. */
 // CAN1 with DMA Errata workaround: configured CAN1 message object buffer to "Dual Port RAM"
 // Possible loss of interrupts when DMA is used with CAN.
 __eds__ static unsigned int can1msgBuf[CAN1_MESSAGE_BUFFERS][8] __attribute__((space(eds), aligned(8 * 8 * 2)));

/******************************************************************************/
/* Private function prototypes                                                */
/******************************************************************************/
static void CAN1_DMACopy(uint8_t buffer_number, CAN_MSG_OBJ *message);
static void CAN1_MessageToBuffer(uint_fast8_t i, CAN_MSG_OBJ *message);

// CAN1 Default Interrupt Handler
static void (*CAN1_BusWakeUpActivityInterruptHandler)(void) = NULL;

/**
 Section: Private Function Definitions
*/

/**
  @Summary
    Read the message object from Receive buffer and update to the user message object 
    pointer.

  @Description
    This routine read the message object from Receive buffer and update to the user  
    message object pointer.

  @Preconditions
    CAN1_Initialize function should be called before calling this function.

  @Param
    bufferNumber - A buffer number is in the Receive buffer where the message would 
                   be stored.
    message      - pointer to the CAN1 Receive message object. 

  @Returns
    None
    
  @Example
    None
*/
static void CAN1_DMACopy(uint8_t buffer_number, CAN_MSG_OBJ *message)
{
    uint16_t ide=0;
    uint16_t rtr=0;
    uint32_t id=0;

    /* read word 0 to see the message type */
    ide=can1msgBuf[buffer_number][0] & 0x0001U;			

    /* check to see what type of message it is */
    /* message is standard identifier */
    if(ide==0U)
    {
        message->msgId =(can1msgBuf[buffer_number][0] & 0x1FFCU) >> 2U;		
        message->field.idType = CAN_FRAME_STD;
        rtr=can1msgBuf[buffer_number][0] & 0x0002U;
    }
    /* message is extended identifier */
    else
    {
        id=can1msgBuf[buffer_number][0] & 0x1FFCU;		
        message->msgId = id << 16U;
        message->msgId += ( ((uint32_t) can1msgBuf[buffer_number][1] & (uint32_t)0x0FFF) << 6U );
        message->msgId += ( ((uint32_t) can1msgBuf[buffer_number][2] & (uint32_t)0xFC00U) >> 10U );		
        message->field.idType = CAN_FRAME_EXT;
        rtr=can1msgBuf[buffer_number][2] & 0x0200;
    }
    /* check to see what type of message it is */
    /* RTR message */
    if(rtr != 0U)
    {
        /* to be defined ?*/
        message->field.frameType = CAN_FRAME_RTR;	
    }
    /* normal message */
    else
    {
        message->field.frameType = CAN_FRAME_DATA;
        message->data[0] =(uint8_t) can1msgBuf[buffer_number][3];
        message->data[1] =(uint8_t) ((can1msgBuf[buffer_number][3] & 0xFF00U) >> 8U);
        message->data[2] =(uint8_t) can1msgBuf[buffer_number][4];
        message->data[3] =(uint8_t) ((can1msgBuf[buffer_number][4] & 0xFF00U) >> 8U);
        message->data[4] =(uint8_t) can1msgBuf[buffer_number][5];
        message->data[5] =(uint8_t) ((can1msgBuf[buffer_number][5] & 0xFF00U) >> 8U);
        message->data[6] =(uint8_t) can1msgBuf[buffer_number][6];
        message->data[7] =(uint8_t) ((can1msgBuf[buffer_number][6] & 0xFF00U) >> 8U);
        message->field.dlc =(uint8_t) (can1msgBuf[buffer_number][2] & 0x000FU);
    }
}

/**
  @Summary
    Read the message object from user input and update to the CAN1 TX buffer.

  @Description
    This routine Read the message object from user input and update to the CAN1 
    TX buffer.

  @Preconditions
    CAN1_Initialize function should be called before calling this function.

  @Param
    buffer      - pointer to the CAN1 Message object. 
    message     - pointer to the CAN1 transmit message object. 

  @Returns
    None
    
  @Example
    None
*/
static void CAN1_MessageToBuffer(uint_fast8_t i, CAN_MSG_OBJ* message)
{   
    if(message->field.idType == CAN_FRAME_STD)
    {
        can1msgBuf[i][0]= (message->msgId & 0x000007FF) << 2;
        can1msgBuf[i][1]= 0;
        can1msgBuf[i][2]= message->field.dlc & 0x0F;
    }
    else
    {
        can1msgBuf[i][0]= ( ( (uint16_t)(message->msgId >> 16 ) & 0x1FFC ) ) | 0x3;
        can1msgBuf[i][1]= (uint16_t)(message->msgId >> 6) & 0x0FFF;
        can1msgBuf[i][2]= (message->field.dlc & 0x0F) + ( (uint16_t)(message->msgId << 10) & 0xFC00);
    }
    
    if(message->data != NULL)
    {
        can1msgBuf[i][3]= ((message->data[1])<<8) + message->data[0];
        can1msgBuf[i][4]= ((message->data[3])<<8) + message->data[2];
        can1msgBuf[i][5]= ((message->data[5])<<8) + message->data[4];
        can1msgBuf[i][6]= ((message->data[7])<<8) + message->data[6];
    }
}

/**
  Section: CAN1 APIs
*/

void CAN1_Initialize(void)
{
    // Disable interrupts before the Initialization
    IEC2bits.C1IE = 0;
    C1INTE = 0;

    // set the CAN1_initialize module to the options selected in the User Interface

    /* put the module in configuration mode */
    C1CTRL1bits.REQOP = CAN_CONFIGURATION_MODE;
    while(C1CTRL1bits.OPMODE != CAN_CONFIGURATION_MODE);

    /* Set up the baud rate*/	
    // CAN1 Clock Errata workaround: CANCKS bit in C1CTRL1 register is reversed
    C1CFG1 = 0x07;	//BRP TQ = (2 x 8)/FCAN; SJW 1 x TQ; 
    C1CFG2 = 0x2A0;	//WAKFIL disabled; SEG2PHTS Freely programmable; SEG2PH 3 x TQ; SEG1PH 5 x TQ; PRSEG 1 x TQ; SAM Once at the sample point; 
    C1FCTRL = 0x4001;	//FSA Transmit/Receive Buffer TRB1; DMABS 8; 
    C1FEN1 = 0x01;	//FLTEN8 disabled; FLTEN7 disabled; FLTEN9 disabled; FLTEN0 enabled; FLTEN2 disabled; FLTEN10 disabled; FLTEN1 disabled; FLTEN11 disabled; FLTEN4 disabled; FLTEN3 disabled; FLTEN6 disabled; FLTEN5 disabled; FLTEN12 disabled; FLTEN13 disabled; FLTEN14 disabled; FLTEN15 disabled; 
    C1CTRL1 = 0x800;	//CANCKS FOSC/2; CSIDL disabled; ABAT disabled; REQOP Sets Normal Operation Mode; WIN Uses buffer window; CANCAP disabled; 

    /* Filter configuration */
    /* enable window to access the filter configuration registers */
    /* use filter window*/
    C1CTRL1bits.WIN=1;	   
    
    /* select acceptance masks for filters */
    C1FMSKSEL1bits.F0MSK = 0x0; //Select Mask 0 for Filter 0
    
    /* Configure the masks */
    C1RXM0SIDbits.SID = 0x7ff; 
    C1RXM1SIDbits.SID = 0x0; 
    C1RXM2SIDbits.SID = 0x0; 
    
    C1RXM0SIDbits.EID = 0x0; 
    C1RXM1SIDbits.EID = 0x0; 
    C1RXM2SIDbits.EID = 0x0; 
    
    C1RXM0EID = 0x00;     	
    C1RXM1EID = 0x00;     	
    C1RXM2EID = 0x00;     	
    
    C1RXM0SIDbits.MIDE = 0x0; 
    C1RXM1SIDbits.MIDE = 0x0; 
    C1RXM2SIDbits.MIDE = 0x0; 
    
    /* Configure the filters */
    C1RXF0SIDbits.SID = 0xa1; 
    
    C1RXF0SIDbits.EID = 0x0; 
    
    C1RXF0EID = 0x00; 
    
    C1RXF0SIDbits.EXIDE = 0x0; 
    
    /* Non FIFO Mode */
    C1BUFPNT1bits.F0BP = 0x1; //Filter 0 uses Buffer1
    
    /* clear window bit to access CAN1 control registers */
    C1CTRL1bits.WIN=0;    

    /*configure CAN1 Transmit/Receive buffer settings*/
    C1TR01CONbits.TXEN0 = 0x1; // Buffer 0 is a Transmit Buffer 
    C1TR01CONbits.TXEN1 = 0x0; // Buffer 1 is a Receive Buffer 
    C1TR23CONbits.TXEN2 = 0x0; // Buffer 2 is a Receive Buffer 
    C1TR23CONbits.TXEN3 = 0x0; // Buffer 3 is a Receive Buffer 
    C1TR45CONbits.TXEN4 = 0x0; // Buffer 4 is a Receive Buffer 
    C1TR45CONbits.TXEN5 = 0x0; // Buffer 5 is a Receive Buffer 
    C1TR67CONbits.TXEN6 = 0x0; // Buffer 6 is a Receive Buffer 
    C1TR67CONbits.TXEN7 = 0x0; // Buffer 7 is a Receive Buffer 

    C1TR01CONbits.TX0PRI = 0x0; // Message Buffer 0 Priority Level
    C1TR01CONbits.TX1PRI = 0x0; // Message Buffer 1 Priority Level
    C1TR23CONbits.TX2PRI = 0x0; // Message Buffer 2 Priority Level
    C1TR23CONbits.TX3PRI = 0x0; // Message Buffer 3 Priority Level
    C1TR45CONbits.TX4PRI = 0x0; // Message Buffer 4 Priority Level
    C1TR45CONbits.TX5PRI = 0x0; // Message Buffer 5 Priority Level
    C1TR67CONbits.TX6PRI = 0x0; // Message Buffer 6 Priority Level
    C1TR67CONbits.TX7PRI = 0x0; // Message Buffer 7 Priority Level

    /* clear the buffer and overflow flags */   
    C1RXFUL1 = 0x0000;
    C1RXFUL2 = 0x0000;
    C1RXOVF1 = 0x0000;
    C1RXOVF2 = 0x0000;	

    /* configure the device to interrupt on the receive buffer full flag */
    /* clear the buffer full flags */ 	
    C1INTFbits.RBIF = 0;  

    /* put the module in normal mode */
    C1CTRL1bits.REQOP = CAN_NORMAL_OPERATION_MODE;
    while(C1CTRL1bits.OPMODE != CAN_NORMAL_OPERATION_MODE);	

    /* Initialize Interrupt Handler*/
    CAN1_SetBusWakeUpActivityInterruptHandler(&CAN1_DefaultBusWakeUpActivityHandler);

    /* Enable CAN1 Interrupt */
    IEC2bits.C1IE = 1;

}

void CAN1_TransmitEnable()
{
    /* setup channel 0 for peripheral indirect addressing mode 
    normal operation, word operation and select as Tx to peripheral */

    /* DMA_PeripheralIrqNumberSet and DMA_TransferCountSet would be done in the 
    DMA */
    
    /* setup the address of the peripheral CAN1 (C1TXD) */ 
    DMA_PeripheralAddressSet(CAN1_TX_DMA_CHANNEL, (uint16_t) &C1TXD);

    /* DPSRAM start address offset value */ 
    DMA_StartAddressASet(CAN1_TX_DMA_CHANNEL, __builtin_dmaoffset(&can1msgBuf));

    /* enable the channel */
    DMA_ChannelEnable(CAN1_TX_DMA_CHANNEL);
}

void CAN1_ReceiveEnable()
{
    /* setup DMA channel for peripheral indirect addressing mode 
    normal operation, word operation and select as Rx to peripheral */

    /* setup the address of the peripheral CAN1 (C1RXD) */     
    /* DMA_TransferCountSet and DMA_PeripheralIrqNumberSet would be set in 
    the DMA_Initialize function */

    DMA_PeripheralAddressSet(CAN1_RX_DMA_CHANNEL, (uint16_t) &C1RXD);

    /* DPSRAM start address offset value */ 
    DMA_StartAddressASet(CAN1_RX_DMA_CHANNEL, __builtin_dmaoffset(&can1msgBuf) );	  

    /* enable the channel */
    DMA_ChannelEnable(CAN1_RX_DMA_CHANNEL);
}

CAN_OP_MODE_STATUS CAN1_OperationModeSet(const CAN_OP_MODES requestMode) 
{
    CAN_OP_MODE_STATUS status = CAN_OP_MODE_REQUEST_SUCCESS;
    
    if((CAN_CONFIGURATION_MODE == CAN1_OperationModeGet()) || (requestMode == CAN_DISABLE_MODE)
            || (requestMode == CAN_CONFIGURATION_MODE))
    {
        C1CTRL1bits.REQOP = requestMode;
        while(C1CTRL1bits.OPMODE != requestMode);
    }
    else
    {
        status = CAN_OP_MODE_REQUEST_FAIL;
    }
    
    return status;
}

CAN_OP_MODES CAN1_OperationModeGet(void) 
{
    return C1CTRL1bits.OPMODE;
}

CAN_TX_MSG_REQUEST_STATUS CAN1_Transmit(CAN_TX_PRIOIRTY priority, CAN_MSG_OBJ *sendCanMsg)
{
    CAN_TX_MSG_REQUEST_STATUS txMsgStatus = CAN_TX_MSG_REQUEST_SUCCESS;
    CAN1_TX_CONTROLS * pTxControls = (CAN1_TX_CONTROLS*)&C1TR01CON;
    uint_fast8_t i;
    bool messageSent = false;
    
    // CAN 2.0 mode DLC supports upto 8 byte 
    if(sendCanMsg->field.dlc > CAN_DLC_8) 
    {
       txMsgStatus |= CAN_TX_MSG_REQUEST_DLC_ERROR;
    }
    
    if(CAN1_TX_BUFFER_COUNT > 0)
    {
        for(i=0; i<CAN1_TX_BUFFER_COUNT; i++)
        {
            if(pTxControls->transmit_enabled == 1)
            {
                if (pTxControls->send_request == 0)
                {
                    // CAN1 with DMA Errata workaround: configured CAN1 message object buffer to "Dual Port RAM"
                    // Possible loss of interrupts when DMA is used with CAN.
                    CAN1_MessageToBuffer(i, sendCanMsg);
                    pTxControls->priority = priority;

                    /* set the message for transmission */
                    pTxControls->send_request = 1; 

                    messageSent = true;
                    break;
                }
            }

            pTxControls++;
        }
    }
    
    if(messageSent == false)
    {
        txMsgStatus |= CAN_TX_MSG_REQUEST_BUFFER_FULL;
    }
    
    return txMsgStatus;
}

bool CAN1_Receive(CAN_MSG_OBJ *recCanMsg) 
{   
    /* We use a static buffer counter so we don't always check buffer 0 first
     * resulting in potential starvation of later buffers.
     */
    static uint_fast8_t currentDedicatedBuffer = 0;
    uint_fast8_t i;
    bool messageReceived = false;
    uint16_t receptionFlags;

    if(recCanMsg->data == NULL)
    {
        return messageReceived;
    }
    
    receptionFlags = C1RXFUL1;
	
    if (receptionFlags != 0)  
    {
        /* check which message buffer is free */  
        for (i=0 ; i < 16; i++)
        {
            if (((receptionFlags >> currentDedicatedBuffer ) & 0x1) == 0x1)
            {           
               CAN1_DMACopy(currentDedicatedBuffer, recCanMsg);
           
               C1RXFUL1 &= ~(1 << currentDedicatedBuffer);
  
               messageReceived = true;
            }
            
            currentDedicatedBuffer++;
            
            if(currentDedicatedBuffer >= 16)
            {
                currentDedicatedBuffer = 0;
            }
            
            if(messageReceived == true)
            {
                break;
            }
        }
    }
        
    return (messageReceived);
}

bool CAN1_IsBusOff() 
{
    return C1INTFbits.TXBO;	
}

bool CAN1_IsRXErrorPassive()
{
    return (C1INTFbits.RXBP);   
}

bool CAN1_IsRxErrorWarning(void)
{
    return (C1INTFbits.RXWAR);
}

bool CAN1_IsRxErrorActive(void)
{
    bool errorState = false;
    if((0 < C1ECbits.RERRCNT) && (C1ECbits.RERRCNT < 128)) 
    {
        errorState = true;
    }
    
    return errorState;
}

bool CAN1_IsTXErrorPassive()
{
    return (C1INTFbits.TXBP);
}

bool CAN1_IsTxErrorWarning(void) 
{
    return (C1INTFbits.TXWAR);
}

bool CAN1_IsTxErrorActive(void)
{
    bool errorState = false;
    if((0 < C1ECbits.TERRCNT) && (C1ECbits.TERRCNT < 128)) 
    {
        errorState = true;
    }
    
    return errorState;
}

uint8_t CAN1_ReceivedMessageCountGet() 
{
    uint_fast8_t messageCount;
    uint_fast8_t currentBuffer;
    uint16_t receptionFlags;
   
    messageCount = 0;

    /* Check any message in buffer 0 to buffer 15*/
    receptionFlags = C1RXFUL1;
    if (receptionFlags != 0) 
    {
        /* check whether a message is received */  
        for (currentBuffer=0 ; currentBuffer < 16; currentBuffer++)
        {
            if (((receptionFlags >> currentBuffer ) & 0x1) == 0x1)
            {
                messageCount++;
            }
        }
    }
            
    return (messageCount);
}

void CAN1_Sleep(void) 
{
    C1INTFbits.WAKIF = 0;
    C1INTEbits.WAKIE = 1;

    /* put the module in disable mode */
    C1CTRL1bits.REQOP = CAN_DISABLE_MODE;
    while(C1CTRL1bits.OPMODE != CAN_DISABLE_MODE);
    
    //Wake up from sleep should set the CAN1 module straight into Normal mode
}

void __attribute__((weak)) CAN1_DefaultBusWakeUpActivityHandler(void) 
{

}

void CAN1_SetBusWakeUpActivityInterruptHandler(void *handler)
{
    CAN1_BusWakeUpActivityInterruptHandler = handler;
}

void __attribute__((__interrupt__, no_auto_psv)) _C1Interrupt(void)
{
    if(C1INTFbits.WAKIF)
    {
        if(CAN1_BusWakeUpActivityInterruptHandler)
        {
            CAN1_BusWakeUpActivityInterruptHandler();
        }
	
        C1INTFbits.WAKIF = 0;
    }
    
    IFS2bits.C1IF = 0;
}

/*******************************************************************************

  !!! Deprecated Definitions and APIs !!!
  !!! These functions will not be supported in future releases !!!

*******************************************************************************/

/******************************************************************************
*                                                                             
*    Function:		CAN1_transmit
*    Description:       Transmits the message from user buffer to CAN1 buffer
*                       as per the buffer number allocated.
*                       Allocation of the buffer number is done by user 
*                                                                             
*    Arguments:		priority : priority of the message to be transmitted
*                       sendCanMsg: pointer to the message object
*                                             
*    Return Value:      true - Transmit successful
*                       false - Transmit failure                                                                              
******************************************************************************/
bool CAN1_transmit(CAN_TX_PRIOIRTY priority, uCAN_MSG *sendCanMsg) 
{
    uint8_t msgObjData[8] = {0};
    CAN_MSG_OBJ txCanMsg;
    txCanMsg.data = msgObjData;
    
    txCanMsg.msgId = sendCanMsg->frame.id;
    txCanMsg.field.idType = sendCanMsg->frame.idType;
    txCanMsg.field.frameType = sendCanMsg->frame.msgtype;
    txCanMsg.field.dlc = sendCanMsg->frame.dlc;
    txCanMsg.data[0] = sendCanMsg->frame.data0;
    txCanMsg.data[1] = sendCanMsg->frame.data1;
    txCanMsg.data[2] = sendCanMsg->frame.data2;
    txCanMsg.data[3] = sendCanMsg->frame.data3;
    txCanMsg.data[4] = sendCanMsg->frame.data4;
    txCanMsg.data[5] = sendCanMsg->frame.data5;
    txCanMsg.data[6] = sendCanMsg->frame.data6;
    txCanMsg.data[7] = sendCanMsg->frame.data7;
    
    return (CAN1_Transmit(priority, &txCanMsg));
}

/******************************************************************************
*                                                                             
*    Function:		CAN1_receive
*    Description:       Receives the message from CAN1 buffer to user buffer 
*                                                                             
*    Arguments:		recCanMsg: pointer to the message object
*                                             
*    Return Value:      true - Receive successful
*                       false - Receive failure                                                                              
******************************************************************************/
bool CAN1_receive(uCAN_MSG *recCanMsg) 
{   
    bool messageReceived = false;
    uint8_t msgObjData[8] = {0};
    CAN_MSG_OBJ rxCanMsg;
    rxCanMsg.data = msgObjData;
    
    if(true == CAN1_Receive(&rxCanMsg))
    {
        recCanMsg->frame.id = rxCanMsg.msgId;
        recCanMsg->frame.idType = rxCanMsg.field.idType;
        
        if(rxCanMsg.field.frameType == CAN_FRAME_RTR)
        {
            recCanMsg->frame.msgtype = CAN_MSG_RTR;
        }
        else
        {
            recCanMsg->frame.msgtype = CAN_MSG_DATA;
        }

        recCanMsg->frame.data0 = rxCanMsg.data[0];
        recCanMsg->frame.data1 = rxCanMsg.data[1];
        recCanMsg->frame.data2 = rxCanMsg.data[2];
        recCanMsg->frame.data3 = rxCanMsg.data[3];
        recCanMsg->frame.data4 = rxCanMsg.data[4];
        recCanMsg->frame.data5 = rxCanMsg.data[5];
        recCanMsg->frame.data6 = rxCanMsg.data[6];
        recCanMsg->frame.data7 = rxCanMsg.data[7];
        recCanMsg->frame.dlc = rxCanMsg.field.dlc;
        messageReceived = true;
    }
        
    return (messageReceived);
}

/******************************************************************************
*                                                                             
*    Function:		CAN1_isBusOff
*    Description:       Checks whether the transmitter in Bus off state
*                                                                             
                                             
*    Return Value:      true - Transmitter in Bus Off state
*                       false - Transmitter not in Bus Off state                                                                              
******************************************************************************/
bool CAN1_isBusOff() 
{
    return C1INTFbits.TXBO;	
}

/******************************************************************************
*                                                                             
*    Function:		CAN1_isRXErrorPassive
*    Description:       Checks whether the receive in error passive state
*                                             
*    Return Value:      true - Receiver in Error Passive state
*                       false - Receiver not in Error Passive state                                                                              
******************************************************************************/
bool CAN1_isRXErrorPassive()
{
    return C1INTFbits.RXBP;   
}

/******************************************************************************
*                                                                             
*    Function:		CAN1_isTXErrorPassive
*    Description:       Checks whether the transmitter in error passive state                                                                          
*                                             
*    Return Value:      true - Transmitter in Error Passive state
*                       false - Transmitter not in Error Passive state                                                                              
******************************************************************************/
bool CAN1_isTXErrorPassive()
{
    return (C1INTFbits.TXBP);
}

/******************************************************************************
*                                                                             
*    Function:		CAN1_messagesInBuffer
*    Description:       returns the number of messages that are received                                                                           
*                                             
*    Return Value:      Number of message received
******************************************************************************/
uint8_t CAN1_messagesInBuffer() 
{
    uint_fast8_t messageCount;
    uint_fast8_t currentBuffer;
    uint16_t receptionFlags;
   
    messageCount = 0;

    /* Check any message in buffer 0 to buffer 15*/
    receptionFlags = C1RXFUL1;
    if (receptionFlags != 0) 
    {
        /* check whether a message is received */  
        for (currentBuffer=0 ; currentBuffer < 16; currentBuffer++)
        {
            if (((receptionFlags >> currentBuffer ) & 0x1) == 0x1)
            {
                messageCount++;
            }
        }
    }
            
    return (messageCount);
}

/******************************************************************************
*                                                                             
*    Function:		CAN1_sleep
*    Description:       Puts CAN1 module in disable mode.
*                                                                       
******************************************************************************/
void CAN1_sleep(void) 
{
    C1INTFbits.WAKIF = 0;
    C1INTEbits.WAKIE = 1;

    /* put the module in disable mode */
    C1CTRL1bits.REQOP = CAN_DISABLE_MODE;
    while(C1CTRL1bits.OPMODE != CAN_DISABLE_MODE);
    
    //Wake up from sleep should set the CAN1 module straight into Normal mode
}

/**
 End of File
*/