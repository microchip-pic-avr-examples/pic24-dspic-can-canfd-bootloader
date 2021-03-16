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

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "can_tp_phy_adaptor.h"
#include "can_tp.h"
#include "can_tp_config.h"

#define MINIMUM_EXTENDED_LENGTH 4096u
#define MAXIMUM_CAN_DATA_LENGTH 64u
#define MAXIMUM_SEPARATION_TIME_MS 127u

/* ISO 15765-2:2016 Table 21 defines the network timing parameters */

/* We define these as (1000+1) because we don't know what portion of the tick
 * period we are in.  We could get the next tick 1us after setting this value, 
 * thus we need +1 so make sure we have "at least" the required time.
 */
#define TIMEOUT_N_AS_IN_MS (1000+1)
#define TIMEOUT_N_AR_IN_MS (1000+1)
#define TIMEOUT_N_BS_IN_MS (1000+1)
#define TIMEOUT_N_CR_IN_MS (1000+1)

#ifndef CAN_TP_MESSAGE_ID_DATA
#define CAN_TP_MESSAGE_ID_DATA      0xA2
#endif

#ifndef CAN_TP_MESSAGE_ID_FLOW_CONTROL
#define CAN_TP_MESSAGE_ID_FLOW_CONTROL 0xA2
#endif

#ifndef CAN_TP_DEFAULT_SEPARATION_TIME_MS
#define CAN_TP_DEFAULT_SEPARATION_TIME_MS 50
#endif

#ifndef CAN_TP_DEFAULT_BLOCK_SIZE
#define CAN_TP_DEFAULT_BLOCK_SIZE 0
#endif

#ifndef CAN_TP_PADDING
/* ISO 15765-2:2016 10.4.2.1 specifies the default padding value of 0xCC unless
 * otherwise specified. */
#define CAN_TP_PADDING 0xCC
#endif

#ifndef CAN_TP_PAUSE_ON_FIRST_FRAME
#define CAN_TP_PAUSE_ON_FIRST_FRAME false
#endif

#if !defined(CAN_TP_ADDRESSING_MODE_EXTENDED) && !defined(CAN_TP_ADDRESSING_MODE_NORMAL)
#define CAN_TP_ADDRESSING_MODE_NORMAL
#endif

#if (CAN_TP_DEFAULT_SEPERATION_TIME_MS > 127)
#error CAN_TP_DEFAULT_SEPERATION_TIME must be between [0-127]
#endif

#ifndef CAN_TP_TRANSMISSION_DATA_LENGTH_MAX
#define CAN_TP_TRANSMISSION_DATA_LENGTH_MAX 8
#endif

#ifndef CAN_TP_MESSAGE_ID_IS_EXTENDED
#define CAN_TP_MESSAGE_ID_IS_EXTENDED false
#endif

#if (CAN_TP_TRANSMISSION_DATA_LENGTH_MAX != 8)  && \
    (CAN_TP_TRANSMISSION_DATA_LENGTH_MAX != 12) && \
    (CAN_TP_TRANSMISSION_DATA_LENGTH_MAX != 16) && \
    (CAN_TP_TRANSMISSION_DATA_LENGTH_MAX != 20) && \
    (CAN_TP_TRANSMISSION_DATA_LENGTH_MAX != 24) && \
    (CAN_TP_TRANSMISSION_DATA_LENGTH_MAX != 32) && \
    (CAN_TP_TRANSMISSION_DATA_LENGTH_MAX != 48) && \
    (CAN_TP_TRANSMISSION_DATA_LENGTH_MAX != 64)
#error "Valid values for CAN_TP_TRANSMISSION_DATA_LENGTH_MAX are 8, 12, 16, 20, 24, 32, 48 or 64."
#endif

#define CAN_TP_FLOW_CONTROL_CONTINUE_TO_SEND    0u
#define CAN_TP_FLOW_CONTROL_WAIT                1u
#define CAN_TP_FLOW_CONTROL_OVERFLOW            2u
#define CAN_TP_FLOW_CONTROL_ABORT               2u

#define MESSAGE_TYPE_SINGLE_VALUE               0u
#define MESSAGE_TYPE_FIRST_VALUE                1u
#define MESSAGE_TYPE_CONSECUTIVE_VALUE          2u
#define MESSAGE_TYPE_FLOW_VALUE                 3u

enum MESSAGE_TYPE
{
    MESSAGE_TYPE_SINGLE = MESSAGE_TYPE_SINGLE_VALUE,
    MESSAGE_TYPE_FIRST = MESSAGE_TYPE_FIRST_VALUE,
    MESSAGE_TYPE_CONSECUTIVE = MESSAGE_TYPE_CONSECUTIVE_VALUE,
    MESSAGE_TYPE_FLOW = MESSAGE_TYPE_FLOW_VALUE
};

enum INCOMING_MESSAGE_STATE
{
    INCOMING_MESSAGE_STATE_IDLE,
    INCOMING_MESSAGE_STATE_RECEIVING,
    INCOMING_MESSAGE_STATE_COMPLETE
};

enum OUTGOING_MESSAGE_STATE
{
    OUTGOING_MESSAGE_STATE_IDLE,
    OUTGOING_MESSAGE_STATE_SENDING,
    OUTGOING_MESSAGE_STATE_FRAME_SENT,
    OUTGOING_MESSAGE_STATE_FLOW_CONTROL_PENDING
};

struct FLOW_CONTROL_SETTINGS
{
    uint8_t separationTime;
    uint8_t blockSize;
    uint8_t status;
    bool updateRequired;
    bool updatePending;
    uint8_t blockCount;
};

struct FLOW_CONTROL_SETTINGS_CHANGE_REQUEST
{
    bool changeRequested;
    uint8_t blockSize;
    uint8_t separationTime;
};

struct INCOMING_MESSAGE
{
    enum INCOMING_MESSAGE_STATE state;
    bool paused;
    uint8_t *buffer;
    size_t bufferSize;
    size_t dataReceived;
    uint32_t totalLength; /* spec calls this SF_DL or FF_DL depending on if it is a segmented message or not. */
    uint8_t canDataLength; /* spec calls this CAN_DL */
    uint8_t nextSequenceNumber;
    struct FLOW_CONTROL_SETTINGS flowControl;
    struct FLOW_CONTROL_SETTINGS_CHANGE_REQUEST flowControlChangeRequest;
};

struct OUTGOING_MESSAGE
{
    enum OUTGOING_MESSAGE_STATE state;
    const uint8_t *buffer;
    size_t dataSent;
    size_t dataRemaining;
    uint8_t dataLastFrame;
    bool complete;
    uint8_t nextSequenceNumber;
    struct FLOW_CONTROL_SETTINGS flowControl;
    struct FLOW_CONTROL_SETTINGS_CHANGE_REQUEST flowControlChangeRequest;
};

struct FRAME
{
    bool valid;
    uint8_t dataLength;
    uint8_t *data;
    uint8_t sequenceNumber;
    bool lastFrame;
    size_t messageLength;
};

static struct INCOMING_MESSAGE incomingMessage;
static struct OUTGOING_MESSAGE outgoingMessage;

static uint8_t rxFrameData[MAXIMUM_CAN_DATA_LENGTH];
static uint8_t rxFrameLength = 0;

static uint8_t txFrameData[MAXIMUM_CAN_DATA_LENGTH];
static uint8_t txFrameLength = 0;

static struct FRAME frame;

static volatile unsigned int txSeparationTimeRemaining;
static volatile unsigned int txTransmitTimeoutCounter;
static volatile unsigned int flowcontrolTransmitTimeoutCounter;
static volatile unsigned int flowcontrolReceiveTimeoutCounter;
static volatile unsigned int rxTimeoutCounter;

static CAN_TP_EventCallback callback = NULL;

#ifdef CAN_TP_RUNTIME_CONFIGURABLE_DATA_LENGTH
#define MODIFIABILITY
#else
#define MODIFIABILITY const
#endif

static MODIFIABILITY uint8_t canMaximumFrameLength = CAN_TP_TRANSMISSION_DATA_LENGTH_MAX;

static void FlowControlStatusUpdate(uint8_t status)
{
    incomingMessage.flowControl.status = status;
    incomingMessage.flowControl.updateRequired = true;
}

void CAN_TP_Initialize(void)
{
    incomingMessage.dataReceived = 0;
    incomingMessage.totalLength = 0;

    rxFrameLength = 0;

    incomingMessage.flowControl.updateRequired = false;
    incomingMessage.flowControl.blockCount = 0;
    incomingMessage.flowControl.blockSize = CAN_TP_DEFAULT_BLOCK_SIZE;
    incomingMessage.flowControl.separationTime = CAN_TP_DEFAULT_SEPARATION_TIME_MS;
    incomingMessage.flowControl.status = CAN_TP_FLOW_CONTROL_WAIT;
    incomingMessage.flowControlChangeRequest.changeRequested = false;
    incomingMessage.flowControlChangeRequest.blockSize = incomingMessage.flowControl.blockSize;
    incomingMessage.flowControlChangeRequest.separationTime = incomingMessage.flowControl.separationTime;
    incomingMessage.state = INCOMING_MESSAGE_STATE_IDLE;
    incomingMessage.paused = false;

    incomingMessage.buffer = NULL;
    callback = NULL;

    outgoingMessage.state = OUTGOING_MESSAGE_STATE_IDLE;

    #ifdef CAN_TP_RUNTIME_CONFIGURABLE_DATA_LENGTH
        canMaximumFrameLength = CAN_TP_TRANSMISSION_DATA_LENGTH_MAX;
    #endif
}

void CAN_TP_RxSeparationTimeSet(uint8_t separationTime)
{
    /* ISO 15765-2:2016 9.6.5.6 - a receiver of data should use the same block
     * size and separation time as the flow control of the first frame - i.e.
     * the separation time is not allowed to change in the middle of a message
     * reception.  So we queue the request for the next FIRST FRAME - the only
     * flow control packet where it is useful. */
    incomingMessage.flowControlChangeRequest.changeRequested = true;

    if (separationTime > MAXIMUM_SEPARATION_TIME_MS)
    {
        incomingMessage.flowControlChangeRequest.separationTime = MAXIMUM_SEPARATION_TIME_MS;
    }
    else
    {
        incomingMessage.flowControlChangeRequest.separationTime = separationTime;
    }
}

void CAN_TP_RxBlockSizeSet(uint8_t blockSize)
{
    /* ISO 15765-2:2016 9.6.5.6 - a receiver of data should use the same block
     * size and separation time as the flow control of the first frame - i.e.
     * the separation time is not allowed to change in the middle of a message
     * reception.  So we queue the request for the next FIRST FRAME - the only
     * flow control packet where it is useful. */
    incomingMessage.flowControlChangeRequest.changeRequested = true;
    incomingMessage.flowControlChangeRequest.blockSize = blockSize;
}

void CAN_TP_RxResume(void)
{
    if (incomingMessage.paused == true)
    {
        incomingMessage.paused = false;

        if (incomingMessage.state == INCOMING_MESSAGE_STATE_RECEIVING)
        {
            FlowControlStatusUpdate(CAN_TP_FLOW_CONTROL_CONTINUE_TO_SEND);
        }
    }
}

void CAN_TP_RxPause(void)
{
    incomingMessage.paused = true;
}

bool CAN_TP_MaxFrameLengthSet(uint8_t newMax)
{
#ifdef CAN_TP_RUNTIME_CONFIGURABLE_DATA_LENGTH
    switch (newMax)
    {
    case 8:
    case 12:
    case 16:
    case 20:
    case 24:
    case 32:
    case 48:
    case 64:
        canMaximumFrameLength = newMax;
        break;
    default:
        break;
    }
#endif   
    return (canMaximumFrameLength == newMax);
}

/* Specified in ISO 15765-2:2016 9.6.3.1 Table 14. */
static uint8_t FirstFrameDataLengthMinGet(void)
{
    /* This is incomplete.  This will only work for CAN and not CAN-FD when the
     * CAN-FD uses a frame length >8. */

    /* This will also only work in normal addressing mode and not mixed or 
     * extended addressing mode. */
    return 8;
}

void CAN_TP_EventCallbackSet(CAN_TP_EventCallback callbackParam)
{
    callback = callbackParam;
}

static void IssueEvent(
                       enum CAN_TP_EVENT event,
                       struct CAN_TP_SERVICE_HEADER* header,
                       void* data
                       )
{
    if (callback != NULL)
    {
        callback(event, header, data);
    }
}

void CAN_TP_RxMessageBufferSet(uint8_t *buffer, size_t bufferSize)
{
    incomingMessage.buffer = buffer;
    incomingMessage.bufferSize = bufferSize;
}

bool CAN_TP_IsMessageReady(void)
{
    return (incomingMessage.state == INCOMING_MESSAGE_STATE_COMPLETE);
}

size_t CAN_TP_MessageLengthGet(void)
{
    return CAN_TP_IsMessageReady() ? incomingMessage.totalLength : 0u;
}

bool CAN_TP_MessageGet(uint8_t *buffer)
{
    bool messageReady = CAN_TP_IsMessageReady();

    if (messageReady)
    {
        if (buffer != NULL)
        {
            memcpy(buffer, incomingMessage.buffer, incomingMessage.totalLength);
        }

        incomingMessage.state = INCOMING_MESSAGE_STATE_IDLE;
    }

    return messageReady;
}

static void IncomingMessageComplete(enum CAN_TP_RESULT result)
{
    struct CAN_TP_EVENT_INDICATION_DATA indicationData;

    /* ISO 15765-2:2016 8.2.4 - the messageData and length fields are only
     * valid if the result == OK, thus can save space and not set them. */
    if (result == CAN_TP_RESULT_OK)
    {
        indicationData.messageData = incomingMessage.buffer;
        indicationData.length = incomingMessage.dataReceived;
    }
    indicationData.result = result;

    IssueEvent(CAN_TP_EVENT_INDICATION, NULL, &indicationData);
}

static void OutgoingMessageComplete(enum CAN_TP_RESULT result)
{
    struct CAN_TP_EVENT_CONFIRM_DATA confirmData;

    confirmData.result = result;

    IssueEvent(CAN_TP_EVENT_DATA_CONFIRM, NULL, &confirmData);

    outgoingMessage.state = OUTGOING_MESSAGE_STATE_IDLE;
}

static void TransmitPendingFlowControl()
{
    if (incomingMessage.flowControl.updateRequired == true)
    {
        txFrameData[0] = (uint8_t) (MESSAGE_TYPE_FLOW_VALUE << 4) | ((uint8_t) incomingMessage.flowControl.status & 0x0Ful);
        txFrameData[1] = incomingMessage.flowControl.blockSize;
        txFrameData[2] = incomingMessage.flowControl.separationTime;

        if (CAN_PHY_Transmit(CAN_TP_MESSAGE_ID_FLOW_CONTROL, CAN_TP_MESSAGE_ID_IS_EXTENDED, txFrameData, 3) == true)
        {
            incomingMessage.flowControl.updateRequired = false;
            incomingMessage.flowControl.updatePending = true;
            flowcontrolTransmitTimeoutCounter = TIMEOUT_N_AR_IN_MS;
        }
    }

    if (incomingMessage.flowControl.updatePending == true)
    {
        if (CAN_PHY_TransmitStatusGet() == CAN_PHY_TRANSMIT_STATUS_SUCCESS)
        {
            incomingMessage.flowControl.updatePending = false;
            rxTimeoutCounter = TIMEOUT_N_CR_IN_MS;
        }
        else
        {
            if (flowcontrolTransmitTimeoutCounter == 0u)
            {
                incomingMessage.flowControl.updatePending = false;

                IncomingMessageComplete(CAN_TP_RESULT_TIMEOUT_A);
            }
        }

    }
}

static inline enum MESSAGE_TYPE GetFrameType()
{
    return (rxFrameData[0] >> 4);
}

static void CopyRxFrameDataIntoMessageBuffer(void)
{
    memcpy(&incomingMessage.buffer[incomingMessage.dataReceived], frame.data, frame.dataLength);
    incomingMessage.dataReceived += frame.dataLength;
}

static inline void RxSegmentedMessageTerminate(void)
{
    if (incomingMessage.state != INCOMING_MESSAGE_STATE_IDLE)
    {
        IncomingMessageComplete(CAN_TP_RESULT_UNEXPECTED_PROTOCOL_DATA_UNIT);
    }
}

static void SingleFrameParse(void)
{
    frame.valid = true;

    /* This implements ISO 15765-2:2016 9.6.1 Table 9 */
    if (rxFrameLength <= 8u)
    {
        frame.dataLength = (rxFrameData[0] & 0x0Fu);
        frame.data = &rxFrameData[1];

        /* ISO 15765-2:2016 9.6.2.2 - if a Single Frame is received with a
         * length of 0, it should be ignored. */
        if (frame.dataLength == 0u)
        {
            frame.valid = false;
        }

        /* ISO 15765-2:2016 9.6.2.2 - if a single frame data length is greater
         * than the can frame length in normal addressing mode, then it should
         * ignored. */
        if (frame.dataLength > (rxFrameLength - 1u))
        {
            frame.valid = false;
        }
    }
    else
    {
        frame.data = &rxFrameData[2];
        frame.dataLength = rxFrameData[1];

        /* ISO 15765-2:2016 9.5.3 - the upper nibble of the first byte of a 
         * SingleFrame with a CAN_DL > 8 (CAN-FD) must be 0 or it should be
         * ignored. */
        if ((rxFrameData[0] & 0x0Fu) != 0u)
        {
            frame.valid = false;
        }
    }
}

static void SingleFrameProcess()
{
    SingleFrameParse();

    if (frame.valid == true)
    {
        /* ISO 15765-2:2016 9.8.3 states that if a single frame comes in while 
         * receiving a segmented message, that the segmented message should be 
         * terminated and the single frame should be received. */
        RxSegmentedMessageTerminate();

        incomingMessage.dataReceived = 0;

        CopyRxFrameDataIntoMessageBuffer();

        incomingMessage.totalLength = frame.dataLength;

        IncomingMessageComplete(CAN_TP_RESULT_OK);

        incomingMessage.state = INCOMING_MESSAGE_STATE_COMPLETE;
    }
}

static void FirstFrameParse(void)
{
    frame.valid = true;

    frame.messageLength = (rxFrameData[0] & 0x0Fu);
    frame.messageLength <<= 8;
    frame.messageLength |= rxFrameData[1];

    /* This implements ISO 15765-2:2016 9.6.1 Table 9 */
    /* An extended message is indicated by a standard length of 0. */
    if (frame.messageLength == 0u)
    {
        frame.messageLength = rxFrameData[2];
        frame.messageLength <<= 8;
        frame.messageLength |= rxFrameData[3];
        frame.messageLength <<= 8;
        frame.messageLength |= rxFrameData[4];
        frame.messageLength <<= 8;
        frame.messageLength |= rxFrameData[5];

        /* Per ISO 15765-2:2016 9.6.3.2 we should ignore any first frame that
         * has an extended/escaped FF_DL but is less than 4096 */
        if (frame.messageLength < MINIMUM_EXTENDED_LENGTH)
        {
            frame.valid = false;
        }

        frame.dataLength = rxFrameLength - 6u;
        frame.data = &rxFrameData[6];
    }
    else
    {
        frame.dataLength = rxFrameLength - 2u;
        frame.data = &rxFrameData[2];
    }

    /* Per ISO 15765-2:2016 9.6.3.2 we should ignore any first frame that
     * has a CAN_DL of less than 8. */
    if (rxFrameLength < 8u)
    {
        frame.valid = false;
    }

    /* Per ISO 15765-2:2016 9.6.3.2 we should ignore any first frame that
     * has a FF_DL less than FF_DLmin. */
    if (frame.messageLength < FirstFrameDataLengthMinGet())
    {
        frame.valid = false;
    }
}

static void FirstFrameProcess()
{
    struct CAN_TP_EVENT_FIRST_FRAME_INDICATION_DATA firstFrameData = {0};

    FirstFrameParse();

    if (frame.valid == true)
    {
        /* ISO 15765-2:2016 9.8.3 states that if a first frame comes in while 
         * receiving a segmented message, that the segmented message should be 
         * terminated and the first frame should be received. */
        RxSegmentedMessageTerminate();

        if (incomingMessage.flowControlChangeRequest.changeRequested == true)
        {
            incomingMessage.flowControlChangeRequest.changeRequested = false;
            incomingMessage.flowControl.blockSize = incomingMessage.flowControlChangeRequest.blockSize;
            incomingMessage.flowControl.separationTime = incomingMessage.flowControlChangeRequest.separationTime;
        }

        incomingMessage.canDataLength = rxFrameLength;
        incomingMessage.totalLength = frame.messageLength;
        firstFrameData.length = frame.messageLength;

        incomingMessage.state = INCOMING_MESSAGE_STATE_RECEIVING;

        IssueEvent(CAN_TP_EVENT_FIRST_FRAME_INDICATION, NULL, &firstFrameData);

        if (incomingMessage.totalLength > incomingMessage.bufferSize)
        {
            /* Specification requires message transfer to be terminated if there is
             * insufficient buffer space to store the message. ISO 15765-2:2016 9.6.3.2 */
            FlowControlStatusUpdate(CAN_TP_FLOW_CONTROL_ABORT);
            IssueEvent(CAN_TP_EVENT_OVERFLOW, NULL, &firstFrameData);

            incomingMessage.state = INCOMING_MESSAGE_STATE_IDLE;
        }
        else
        {
            incomingMessage.paused |= CAN_TP_PAUSE_ON_FIRST_FRAME;
            
            /* ISO 15765-2:2016 9.6.4.3 - the sequence number will start at 1
             * for the first consecutive frame and then increment wrapping at
             * 16 back to 0. */
            incomingMessage.nextSequenceNumber = 1;

            incomingMessage.flowControl.blockCount = 0;
            incomingMessage.dataReceived = 0;

            CopyRxFrameDataIntoMessageBuffer();

            if (incomingMessage.paused == true)
            {
                FlowControlStatusUpdate(CAN_TP_FLOW_CONTROL_WAIT);
            }
            else
            {
                FlowControlStatusUpdate(CAN_TP_FLOW_CONTROL_CONTINUE_TO_SEND);
            }
        }
    }
}

static void ConsecutiveFrameParse(void)
{
    frame.sequenceNumber = rxFrameData[0] & 0x0Fu;
    frame.valid = true;
    frame.data = &rxFrameData[1];

    if (frame.sequenceNumber != incomingMessage.nextSequenceNumber)
    {
        frame.valid = false;
    }

    frame.dataLength = rxFrameLength - 1u;

    if ((incomingMessage.dataReceived + frame.dataLength) >= incomingMessage.totalLength)
    {
        /* the frame can be smaller or the same size as CAN_DL but not larger.
         */
        if (rxFrameLength > incomingMessage.canDataLength)
        {
            frame.valid = false;
        }
        else
        {
            frame.lastFrame = true;

            /* data might be padded.  Strip off padding. */
            frame.dataLength = incomingMessage.totalLength - incomingMessage.dataReceived;
        }
    }
    else
    {
        if (rxFrameLength != incomingMessage.canDataLength)
        {
            frame.valid = false;
        }
        else
        {
            frame.lastFrame = false;
        }
    }
}

static void ConsecutiveFrameProcess()
{
    ConsecutiveFrameParse();

    if (frame.valid)
    {
        if (incomingMessage.flowControl.blockSize != 0u)
        {
            incomingMessage.flowControl.blockCount++;

            if (incomingMessage.flowControl.blockCount == incomingMessage.flowControl.blockSize)
            {
                incomingMessage.paused = true;

                FlowControlStatusUpdate(CAN_TP_FLOW_CONTROL_WAIT);
            }
        }

        CopyRxFrameDataIntoMessageBuffer();

        if (frame.lastFrame == true)
        {
            IncomingMessageComplete(CAN_TP_RESULT_OK);

            incomingMessage.state = INCOMING_MESSAGE_STATE_COMPLETE;
        }
        else
        {
            incomingMessage.nextSequenceNumber++;
            if (incomingMessage.nextSequenceNumber > 15u)
            {
                incomingMessage.nextSequenceNumber = 0;
            }
        }
    }
    else
    {
        /* ISO 15765-2:2016 9.6.4.4 - If a sequenceNumber is received out of order, 
         * the message reception is terminated and the higher level is notified. */
        if (frame.sequenceNumber != incomingMessage.nextSequenceNumber)
        {
            FlowControlStatusUpdate(CAN_TP_FLOW_CONTROL_ABORT);

            IncomingMessageComplete(CAN_TP_RESULT_WRONG_SEQUENCE_NUMBER);
        }
    }
}

/* ISO 15765-2:2016 9.6.5.4 Table 20. */
static uint16_t SeparationTimeToTicks(uint8_t separationTime)
{
    uint16_t ticks = 0;

    if (separationTime < 128u)
    {
        ticks = (uint16_t) separationTime;
    }
    else if ((separationTime >= 0xF1u) && (separationTime <= 0xF9u))
    {
        /* We have a 1ms tick so our minimum possible granularity is 1ms.
         * 0xF1-0xF9 correspond to 100us-900us so we will use 1ms.  The maximum 
         * allowed value is specified in ISO 15765-2:2016 Table 21 as 900ms so
         * 1 ms is fine. */
        ticks = 1;
    }
    else
    {
        /* ISO 15765-2:2016 section 9.6.5.5 states that if an invalid separation 
         * time is specific in the flow control frame, that the maximum possible 
         * separation time permitted in the spec should be used (127ms). */
        ticks = MAXIMUM_SEPARATION_TIME_MS;
    }

    /* If we need to wait, then we need to wait for this number of milliseconds
     * plus one since we don't know where in the 1ms we are and we need to wait
     * "at least" this time so we need at least one full tick. */
    if (ticks != 0u)
    {
        ticks += 1u;
    }

    return ticks;
}

static void FlowcontrolFrameProcess()
{
    uint8_t status = (rxFrameData[0] & 0x0Fu);

    switch (status)
    {
    case CAN_TP_FLOW_CONTROL_CONTINUE_TO_SEND:
        outgoingMessage.flowControlChangeRequest.blockSize = rxFrameData[1];
        outgoingMessage.flowControlChangeRequest.separationTime = rxFrameData[2];
        outgoingMessage.flowControlChangeRequest.changeRequested = true;
        break;

    case CAN_TP_FLOW_CONTROL_WAIT:
        break;

    case CAN_TP_FLOW_CONTROL_OVERFLOW:
        OutgoingMessageComplete(CAN_TP_RESULT_BUFFER_OVERFLOW);
        break;

    default:
        break;
    }
}

static bool ReceptionTimedOut(void)
{
    return (rxTimeoutCounter == 0u) &&
            (incomingMessage.state != INCOMING_MESSAGE_STATE_IDLE) &&
            (incomingMessage.flowControl.updatePending == false);
}

static void ProcessIncomingFrame(void)
{
    /* Can only process a new message if there isn't an existing message and 
     * we have a valid buffer to read it into. */
    if ((CAN_TP_IsMessageReady() == false) &&
            (incomingMessage.buffer != NULL))
    {
        if (CAN_PHY_Receive(rxFrameData, &rxFrameLength) == false)
        {
            if (ReceptionTimedOut() == true)
            {
                incomingMessage.state = INCOMING_MESSAGE_STATE_IDLE;

                IncomingMessageComplete(CAN_TP_RESULT_TIMEOUT_CR);
            }
        }
        else
        {
            switch (GetFrameType())
            {
            case MESSAGE_TYPE_SINGLE:
                SingleFrameProcess();
                break;
            case MESSAGE_TYPE_FIRST:
                FirstFrameProcess();
                break;
            case MESSAGE_TYPE_CONSECUTIVE:
                ConsecutiveFrameProcess();
                break;
            case MESSAGE_TYPE_FLOW:
                FlowcontrolFrameProcess();
                break;
            default:
                break;
            }
        }
    }
}

static inline void OutputMessageLastFrameStatusCheck(void)
{
    switch (CAN_PHY_TransmitStatusGet())
    {
    case CAN_PHY_TRANSMIT_STATUS_PENDING:
        if (txTransmitTimeoutCounter == 0u)
        {
            OutgoingMessageComplete(CAN_TP_RESULT_TIMEOUT_A);
        }
        break;

    case CAN_PHY_TRANSMIT_STATUS_SUCCESS:
        outgoingMessage.dataRemaining -= outgoingMessage.dataLastFrame;
        outgoingMessage.dataSent += outgoingMessage.dataLastFrame;

        if (outgoingMessage.dataRemaining == 0u)
        {
            OutgoingMessageComplete(CAN_TP_RESULT_OK);
        }
        else
        {
            outgoingMessage.state = OUTGOING_MESSAGE_STATE_SENDING;

            txSeparationTimeRemaining = SeparationTimeToTicks(outgoingMessage.flowControl.separationTime);

            outgoingMessage.nextSequenceNumber++;

            if (outgoingMessage.nextSequenceNumber == 16u)
            {
                outgoingMessage.nextSequenceNumber = 0;
            }

            if (outgoingMessage.flowControl.blockSize != 0u)
            {
                outgoingMessage.flowControl.blockCount++;
                if (outgoingMessage.flowControl.blockCount >= outgoingMessage.flowControl.blockSize)
                {
                    flowcontrolReceiveTimeoutCounter = TIMEOUT_N_BS_IN_MS;
                    outgoingMessage.state = OUTGOING_MESSAGE_STATE_FLOW_CONTROL_PENDING;
                }
            }
        }
        break;

    case CAN_PHY_TRANSMIT_STATUS_ERROR:
    default:
        break;
    }
}

static void TransmitTxFrame(void)
{
    if (CAN_PHY_Transmit(CAN_TP_MESSAGE_ID_DATA, CAN_TP_MESSAGE_ID_IS_EXTENDED, txFrameData, txFrameLength) == true)
    {
        txTransmitTimeoutCounter = TIMEOUT_N_AS_IN_MS;
        outgoingMessage.state = OUTGOING_MESSAGE_STATE_FRAME_SENT;
    }
}

static uint8_t AdjustFrameSize(uint8_t frameDataLength)
{
    uint8_t newFrameLength;
    
    if(frameDataLength > 48u)
    {
        newFrameLength = 64u;
    }
    else if(frameDataLength > 32u)
    {
        newFrameLength = 48u;
    }
    else if(frameDataLength > 24u)
    {
        newFrameLength = 32u;
    }
    else if(frameDataLength > 20u)
    {
        newFrameLength = 24u;
    }
    else if(frameDataLength > 16u)
    {
        newFrameLength = 20u;
    }
    else if(frameDataLength > 12u)
    {
        newFrameLength = 16u;
    }
    else if(frameDataLength > 8u)
    {
        newFrameLength = 12u;
    }
    else
    {
        newFrameLength = frameDataLength;
    }
    
    return newFrameLength;
}

static void SendSingleFrame()
{
    /* Format the first frame with the correct length - table 9 section 9.6.1
     * of ISO 15765-2:2016 */
    outgoingMessage.flowControl.blockSize = 0;
    outgoingMessage.flowControl.separationTime = 0;

    (void)memset(txFrameData, CAN_TP_PADDING, sizeof(txFrameData));
    
    if (outgoingMessage.dataRemaining <= 7u)
    {
        memcpy(&txFrameData[1], outgoingMessage.buffer, outgoingMessage.dataRemaining);
        txFrameData[0] = (MESSAGE_TYPE_SINGLE_VALUE << 4) | (uint8_t) outgoingMessage.dataRemaining;
        txFrameLength = outgoingMessage.dataRemaining + 1u;
    }
    else
    {
        memcpy(&txFrameData[2], outgoingMessage.buffer, outgoingMessage.dataRemaining);
        txFrameData[0] = (MESSAGE_TYPE_SINGLE_VALUE << 4) | 0u;
        txFrameData[1] = (uint8_t) outgoingMessage.dataRemaining;
        txFrameLength = AdjustFrameSize(outgoingMessage.dataRemaining + 2u);
    }
    
    outgoingMessage.dataLastFrame = outgoingMessage.dataRemaining;

    TransmitTxFrame();
}

static void SendFirstFrame()
{
    /* Format the first frame with the correct length - table 9 section 9.6.1
     * of ISO 15765-2:2016 */
    if (outgoingMessage.dataRemaining <= 4095u)
    {
        outgoingMessage.dataLastFrame = canMaximumFrameLength - 2u;

        txFrameData[0] = (MESSAGE_TYPE_FIRST_VALUE << 4) | (outgoingMessage.dataRemaining >> 8);
        txFrameData[1] = (uint8_t) (outgoingMessage.dataRemaining & 0xFFu);
        memcpy(&txFrameData[2], outgoingMessage.buffer, outgoingMessage.dataLastFrame);
    }
    else
    {
        outgoingMessage.dataLastFrame = canMaximumFrameLength - 6u;

        txFrameData[0] = (MESSAGE_TYPE_FIRST_VALUE << 4);
        txFrameData[1] = 0x00;
        txFrameData[2] = (uint8_t) ((uint32_t)outgoingMessage.dataRemaining >> 24);
        txFrameData[3] = (uint8_t) ((uint32_t)outgoingMessage.dataRemaining >> 16);
        txFrameData[4] = (uint8_t) (outgoingMessage.dataRemaining >> 8);
        txFrameData[5] = (uint8_t) (outgoingMessage.dataRemaining >> 0);
        memcpy(&txFrameData[6], outgoingMessage.buffer, outgoingMessage.dataLastFrame);
    }

    txFrameLength = canMaximumFrameLength;
    outgoingMessage.flowControl.blockSize = 1;
    outgoingMessage.nextSequenceNumber = 0;

    TransmitTxFrame();
}

static void SendConsecutiveFrame(void)
{
    if (txSeparationTimeRemaining == 0u)
    {
        (void)memset(txFrameData, CAN_TP_PADDING, sizeof(txFrameData));
    
        if (outgoingMessage.dataRemaining <= (canMaximumFrameLength - 1u))
        {
            outgoingMessage.dataLastFrame = outgoingMessage.dataRemaining;
        }
        else
        {
            outgoingMessage.dataLastFrame = (canMaximumFrameLength - 1u);
        }

        txFrameLength = AdjustFrameSize(outgoingMessage.dataLastFrame + 1u);
        txFrameData[0] = (MESSAGE_TYPE_CONSECUTIVE_VALUE << 4) | outgoingMessage.nextSequenceNumber;
        memcpy(&txFrameData[1], &outgoingMessage.buffer[outgoingMessage.dataSent], outgoingMessage.dataLastFrame);

        TransmitTxFrame();
    }
}

static bool CanDataFitInOneFrame(void)
{
    return (outgoingMessage.dataRemaining <= (canMaximumFrameLength - 2u)) ? true : (outgoingMessage.dataRemaining == 7u);
}

static inline void ProcessOutgoingFlowControlChangeRequest(void)
{
    outgoingMessage.flowControl.blockCount = 0;
    outgoingMessage.flowControl.blockSize = outgoingMessage.flowControlChangeRequest.blockSize;
    outgoingMessage.flowControl.separationTime = outgoingMessage.flowControlChangeRequest.separationTime;
    outgoingMessage.flowControlChangeRequest.changeRequested = false;
}

static void ProcessOutgoingMessage(void)
{
    switch (outgoingMessage.state)
    {
    case OUTGOING_MESSAGE_STATE_IDLE:
        break;

    case OUTGOING_MESSAGE_STATE_FRAME_SENT:
        OutputMessageLastFrameStatusCheck();
        break;

    case OUTGOING_MESSAGE_STATE_FLOW_CONTROL_PENDING:
        if (outgoingMessage.flowControlChangeRequest.changeRequested == true)
        {
            ProcessOutgoingFlowControlChangeRequest();

            txSeparationTimeRemaining = SeparationTimeToTicks(outgoingMessage.flowControl.separationTime);

            outgoingMessage.state = OUTGOING_MESSAGE_STATE_SENDING;
        }
        else
        {
            if (flowcontrolReceiveTimeoutCounter == 0u)
            {
                OutgoingMessageComplete(CAN_TP_RESULT_TIMEOUT_BS);
            }
        }
        break;

    case OUTGOING_MESSAGE_STATE_SENDING:
        if (outgoingMessage.dataSent == 0u)
        {
            if (CanDataFitInOneFrame() == true)
            {
                SendSingleFrame();
            }
            else
            {
                SendFirstFrame();
            }
        }
        else
        {
            SendConsecutiveFrame();
        }
        break;

    default:
        break;
    }
}

void CAN_TP_Tasks(void)
{
    ProcessIncomingFrame();

    TransmitPendingFlowControl();

    ProcessOutgoingMessage();
}

void CAN_TP_Tick(void)
{
    if (txSeparationTimeRemaining != 0u)
    {
        txSeparationTimeRemaining--;
    }

    if (txTransmitTimeoutCounter != 0u)
    {
        txTransmitTimeoutCounter--;
    }

    if (flowcontrolTransmitTimeoutCounter != 0u)
    {
        flowcontrolTransmitTimeoutCounter--;
    }

    if (flowcontrolReceiveTimeoutCounter != 0u)
    {
        flowcontrolReceiveTimeoutCounter--;
    }

    if (rxTimeoutCounter != 0u)
    {
        rxTimeoutCounter--;
    }
}

bool CAN_TP_MessageSend(const uint8_t *buffer, size_t length)
{
    bool sent = false;

    if (outgoingMessage.state == OUTGOING_MESSAGE_STATE_IDLE)
    {
        outgoingMessage.dataSent = 0;
        outgoingMessage.buffer = buffer;
        outgoingMessage.dataRemaining = length;
        outgoingMessage.state = OUTGOING_MESSAGE_STATE_SENDING;

        sent = true;
    }

    return sent;
}