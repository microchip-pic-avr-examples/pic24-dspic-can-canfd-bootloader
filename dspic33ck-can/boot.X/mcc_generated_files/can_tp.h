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

#ifndef CAN_TP_H
#define CAN_TP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
    
enum CAN_TP_EVENT{
    CAN_TP_EVENT_FIRST_FRAME_INDICATION,
    CAN_TP_EVENT_OVERFLOW,
    CAN_TP_EVENT_INDICATION,
    CAN_TP_EVENT_DATA_CONFIRM
};

enum CAN_TP_MESSAGE_TYPE{
    CAN_TP_MESSAGE_TYPE_DIAGNOSTICS,
    CAN_TP_MESSAGE_TYPE_REMOTE_DIAGNOSTICS
};

enum CAN_TP_NETWORK_TARGET_ADDRESS_TYPE{
    CAN_TP_NETWORK_TARGET_ADDRESS_TYPE_1,
    CAN_TP_NETWORK_TARGET_ADDRESS_TYPE_2,
    CAN_TP_NETWORK_TARGET_ADDRESS_TYPE_3,
    CAN_TP_NETWORK_TARGET_ADDRESS_TYPE_4,
    CAN_TP_NETWORK_TARGET_ADDRESS_TYPE_5,
    CAN_TP_NETWORK_TARGET_ADDRESS_TYPE_6,
    CAN_TP_NETWORK_TARGET_ADDRESS_TYPE_7,
    CAN_TP_NETWORK_TARGET_ADDRESS_TYPE_8
};

enum CAN_TP_RESULT {
    CAN_TP_RESULT_OK,
    CAN_TP_RESULT_TIMEOUT_A,
    CAN_TP_RESULT_TIMEOUT_BS,
    CAN_TP_RESULT_TIMEOUT_CR,
    CAN_TP_RESULT_WRONG_SEQUENCE_NUMBER,
    CAN_TP_RESULT_INVALID_FLOW_STATUS,
    CAN_TP_RESULT_UNEXPECTED_PROTOCOL_DATA_UNIT,
    CAN_TP_RESULT_WFT_OVERUN,
    CAN_TP_RESULT_BUFFER_OVERFLOW,
    CAN_TP_RESULT_ERROR
};

struct CAN_TP_SERVICE_HEADER{
    enum CAN_TP_MESSAGE_TYPE messageType; 
    uint8_t networkSourceAddress; 
    uint8_t networkTargetAddress; 
    enum CAN_TP_NETWORK_TARGET_ADDRESS_TYPE networkTargetAddressType;
    uint8_t networkAddressExtension;
};

struct CAN_TP_EVENT_FIRST_FRAME_INDICATION_DATA {
    uint32_t length;
};

struct CAN_TP_EVENT_INDICATION_DATA {
    uint8_t *messageData;
    uint32_t length;
    enum CAN_TP_RESULT result;
};

struct CAN_TP_EVENT_CONFIRM_DATA {
    enum CAN_TP_RESULT result;
};

typedef void (*CAN_TP_EventCallback)(
    enum CAN_TP_EVENT event, 
    struct CAN_TP_SERVICE_HEADER *header,
    void *payload
);

/**
  @Summary
    Initializes the CAN-TP layer

  @Description
    Initializes the CAN-TP layer

  @Preconditions
    None

  @Param
    None

  @Returns
    None
*/
void CAN_TP_Initialize(void);

/**
  @Summary
    Sets and event callback handler for the CAN-TP events to go to.

  @Description
    Sets and event callback handler for the CAN-TP events to go to.  The
    specified function will be called whenever there are events that the 
    ISO 15765-2 specification says the upper layer should be notified.
 
    This is typically called in the system configuration and will not be used
    during the normal operation.
  
    The callback will have 3 parameters:
     - The event of CAN_TP_EVENT type
     - The addressing information in the CAN_TP_SERVICE_HEADER struct
     - The event data payload in the *void pointer

    The event data payload will be a pointer to a structure that has the
    data for that specific type of event.  For example, the 
    CAN_TP_EVENT_INDICATION event will use the CAN_TP_EVENT_INDICATION_DATA
    structure for the data field.  

  @Preconditions
    CAN_TP_Initialize function should have been called 
    before calling this function. 

  @Param
    CAN_TP_EventCallback callback - function to call on an event

  @Returns
    None
*/
void CAN_TP_EventCallbackSet(CAN_TP_EventCallback callback);

/**
  @Summary
    Provides the CAN-TP with a buffer to receive messages into.

  @Description
    Provides the CAN-TP with a buffer to receive messages into.  The CAN-TP
    layer will use this buffer to receive the messages and then notify the upper
    layer via the event callback when the message is complete or terminated.

    This is typically called in the system configuration and will not be used
    during the normal operation.
 
  @Preconditions
    CAN_TP_Initialize function should have been called 
    before calling this function. 

  @Param
    uint8_t *buffer - pointer to data buffer to use for message storage
    size_t bufferSize - size of the buffer that is being provided

  @Returns
    A data byte received by the driver.
*/
void CAN_TP_RxMessageBufferSet(uint8_t *buffer, size_t bufferSize);

/**
  @Summary
    Tasks routine that keeps the CAN-TP layer running

  @Description
    Tasks routine that keeps the CAN-TP layer running.  This should be called
    frequently to keep the CAN-TP layer running correctly.  This needs to be
    called at least as frequently as the minimum CAN-TP timeout of the system
    which is typically at least once a second (though more frequent is 
    preferred).

  @Preconditions
    CAN_TP_Initialize function should have been called 
    before calling this function. 

  @Param
    None

  @Returns
    None
*/
void CAN_TP_Tasks(void);

/**
  @Summary
    Function that handles all the timed events.

  @Description
    Function that handles all the timed events.  This function
	must be called by the user once every 1 millisecond to keep
	the internal timing correct.  The precision of the call is
	not very critical but should be once per millisecond.  The
	time spent in this function is minimal.  A few variables
	are changed and then it returns and the events that are
	handled in the Tasks() function.

  @Preconditions
    CAN_TP_Initialize function should have been called 
    before calling this function. 

  @Param
    None

  @Returns
    None
*/
void CAN_TP_Tick(void);

/**
  @Summary
    Indicates if a message is complete and ready to read.

  @Description
    Indicates if a message is complete and ready to read.

  @Preconditions
    CAN_TP_Initialize function should have been called 
    before calling this function. 

  @Param
    None

  @Returns
    bool - true if message is ready, false message is not available
*/
bool CAN_TP_IsMessageReady(void);

/**
  @Summary
    Gets the message received by the CAN-TP layer

  @Description
    Gets the message received by the CAN-TP layer. This buffer must be at least
    as large as the message in the buffer.  The CAN_TP_MessageLengthGet()
    function can be used to determine the length if required.

  @Preconditions
    CAN_TP_Initialize function should have been called 
    before calling this function. 

  @Param
    uint8_t *buffer - buffer to copy the message into.  if NULL, the message
    will be expired without copying it.

  @Returns
    bool - true if message was available, false if no message available.
*/
bool CAN_TP_MessageGet(uint8_t *buffer);

/**
  @Summary
    Gets the length of the available message

  @Description
    Gets the length of the available message.  0 if no message available.

  @Preconditions
    CAN_TP_Initialize function should have been called 
    before calling this function.

  @Param
    None

  @Returns
    size_t - length of the available message.  0 if no message available.
*/
size_t CAN_TP_MessageLengthGet(void);

/**
  @Summary
    Sends a message

  @Description
    Sends the specified message through the CAN-TP layer.

  @Preconditions
    CAN_TP_Initialize function should have been called 
    before calling this function. 

  @Param
    uint8_t *buffer - message to send
    size_t length - length of the message

  @Returns
    bool - true if able to start sending message, false if couldn't start
*/
bool CAN_TP_MessageSend(const uint8_t *buffer, size_t length);

/**
  @Summary
    Sets the requested separation time between RX frames.

  @Description
    Sets the requested separation time between RX frames.  This is specified to
    the transmitter in the flow control packet.

  @Preconditions
    CAN_TP_Initialize function should have been called 
    before calling this function. 

  @Param
    uint8_t separationTime - number of requested milliseconds between RX frames.
    Valid range [0-127].  Numbers above 127 will be truncated to 127. 

  @Returns
    None
*/
void CAN_TP_RxSeparationTimeSet(uint8_t separationTime);

/**
  @Summary
    Sets number of frames to receive in a segmented message before pausing the 
    reception.

  @Description
    Sets number of frames to receive in a segmented message before pausing the 
    reception.  This is the number of consecutive frames to receive after the 
    first frame, thus the first frame is not included in this count. 
  
    Must be set before the start of a segmented message reception.  This can be
    called as late as the FIRST_FRAME event and still take affect.  After the
    return of the FIRST_FRAME event, this function will have no effect until the
    next segmented message. 
 
    The default size if never called is 0, which indicates no pause (continuous
    reception until message is complete). 

  @Preconditions
    CAN_TP_Initialize function should have been called 
    before calling this function. 

  @Param
    size_t bytesToReceiveBeforePause - number of bytes to receive before pausing
    the incoming reception.  Setting to 0 will make the reception never pause.

  @Returns
    None
*/
void CAN_TP_RxBlockSizeSet(uint8_t blockSize);

/**
  @Summary
    Allows reception to continue after paused because of the block size limit.

  @Description
    Allows reception to continue after paused because of the block size limit.

  @Preconditions
    CAN_TP_Initialize function should have been called 
    before calling this function. 

  @Param
    None

  @Returns
    None
*/
void CAN_TP_RxResume(void);

/**
  @Summary
    Pause reception of a segmented at the next FlowControl or FirstFrame packet

  @Description
    Pause reception of a segmented at the next FlowControl or FirstFrame packet

  @Preconditions
    CAN_TP_Initialize function should have been called 
    before calling this function. 

  @Param
    None

  @Returns
    None
*/
void CAN_TP_RxPause(void);

#ifdef __cplusplus
}
#endif

#endif /* CAN_TP_H */

