/**
  DMA Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    dma.h

  @Summary
    This is the generated header file for the DMA driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides APIs for driver for DMA.
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

#ifndef DMA_H
#define DMA_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
        
/**
  Section: Data Types
*/

/** DMA Channel Definition
 
 @Summary 
   Defines the channels available for DMA
 
 @Description
   This routine defines the channels that are available for the module to use.
 
 Remarks:
   None
 */
typedef enum 
{
    DMA_CHANNEL_0 =  0,       
    DMA_CHANNEL_1 =  1,       
    DMA_CHANNEL_2 =  2,       
    DMA_CHANNEL_3 =  3,       
    DMA_CHANNEL_4 =  4,       
    DMA_CHANNEL_5 =  5,       
    DMA_CHANNEL_6 =  6,       
    DMA_CHANNEL_7 =  7,       
    DMA_CHANNEL_8 =  8,       
    DMA_CHANNEL_9 =  9,       
    DMA_CHANNEL_10 =  10,       
    DMA_CHANNEL_11 =  11,       
    DMA_CHANNEL_12 =  12,       
    DMA_CHANNEL_13 =  13,       
    DMA_CHANNEL_14 =  14,       
    DMA_NUMBER_OF_CHANNELS = 15
} DMA_CHANNEL;

/** DMA Peripheral IRQ Number
 
 @Summary 
   Defines the DMA Peripheral IRQ number.
 
 @Description
   This routine defines the DMA Peripheral IRQ number.
 
 Remarks:
   None
 */
typedef enum 
{
    DMA_PERIPHERAL_IRQ_ADC2 = 0x15,       
    DMA_PERIPHERAL_IRQ_PMP = 0x2d,       
    DMA_PERIPHERAL_IRQ_UART3_TX = 0x53,       
    DMA_PERIPHERAL_IRQ_UART3_RX = 0x52,       
    DMA_PERIPHERAL_IRQ_UART4_TX = 0x59,       
    DMA_PERIPHERAL_IRQ_UART4_RX = 0x58,       
    DMA_PERIPHERAL_IRQ_DCI = 0x3c,       
    DMA_PERIPHERAL_IRQ_SPI3 = 0x5b,       
    DMA_PERIPHERAL_IRQ_SPI4 = 0x7b,       
    DMA_PERIPHERAL_IRQ_CAN2_TX = 0x47,       
    DMA_PERIPHERAL_IRQ_CAN1_TX = 0x46,       
    DMA_PERIPHERAL_IRQ_CAN2_RX = 0x37,       
    DMA_PERIPHERAL_IRQ_IC4 = 0x26,       
    DMA_PERIPHERAL_IRQ_IC3 = 0x25,       
    DMA_PERIPHERAL_IRQ_CAN1_RX = 0x22,       
    DMA_PERIPHERAL_IRQ_SPI2 = 0x21,       
    DMA_PERIPHERAL_IRQ_UART2_TX = 0x1f,       
    DMA_PERIPHERAL_IRQ_UART2_RX = 0x1e,       
    DMA_PERIPHERAL_IRQ_TMR5 = 0x1c,       
    DMA_PERIPHERAL_IRQ_TMR4 = 0x1b,       
    DMA_PERIPHERAL_IRQ_OC4 = 0x1a,       
    DMA_PERIPHERAL_IRQ_OC3 = 0x19,       
    DMA_PERIPHERAL_IRQ_ADC1 = 0xd,       
    DMA_PERIPHERAL_IRQ_UART1_TX = 0xc,       
    DMA_PERIPHERAL_IRQ_UART1_RX = 0xb,       
    DMA_PERIPHERAL_IRQ_SPI1 = 0xa,       
    DMA_PERIPHERAL_IRQ_TMR3 = 0x8,       
    DMA_PERIPHERAL_IRQ_TMR2 = 0x7,       
    DMA_PERIPHERAL_IRQ_OC2 = 0x6,       
    DMA_PERIPHERAL_IRQ_IC2 = 0x5,       
    DMA_PERIPHERAL_IRQ_OC1 = 0x2,       
    DMA_PERIPHERAL_IRQ_IC1 = 0x1,       
    DMA_PERIPHERAL_IRQ_INT0 = 0x0,       
} DMA_PERIPHERAL_IRQ_NUMBER;

/**
  Section: Interface Routines
*/

/**
  @Summary
    This function initializes DMA instance : 1

  @Description
    This routine initializes the DMA driver instance for : 1
    index, making it ready for clients to open and use it. It also initializes any
    internal data structures.
    This routine must be called before any other DMA routine is called. 

  @Preconditions
    None.

  @Param
    None.

  @Returns
    None.

  @Comment
   
 
  @Example
    <code>
        unsigned short int srcArray[100];
        unsigned short int dstArray[100];
        int i;
        int count;
        for (i=0; i<100; i++)
        {
            srcArray[i] = i+1;
            dstArray[i] = 0;
        }
        
        DMA_Initialize();
        DMA_SoftwareTriggerEnable(CHANNEL1);
        
        count = DMA_TransferCountGet;
        while(count > 0)
        {
        while(
        DMA_SoftwareTriggerEnable(CHANNEL1);
        }
    </code>

*/
void DMA_Initialize(void);

/**
  @Summary
    Clears the interrupt request flag.

  @Description
    This routine is used to clear the interrupt request flag. This routine sets the value of the DMAIF bit to 0.
 
  @Preconditions
    DMA_Initializer() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    None
  
  @Example
    Refer to DMA_Initializer(); for an example
 */
inline static void DMA_FlagInterruptClear(DMA_CHANNEL  channel)
{    
    switch(channel) { 
        case DMA_CHANNEL_0:
                IFS0bits.DMA0IF = 0;                
                break;       
        case DMA_CHANNEL_1:
                IFS0bits.DMA1IF = 0;                
                break;       
        case DMA_CHANNEL_2:
                IFS1bits.DMA2IF = 0;                
                break;       
        case DMA_CHANNEL_3:
                IFS2bits.DMA3IF = 0;                
                break;       
        case DMA_CHANNEL_4:
                IFS2bits.DMA4IF = 0;                
                break;       
        case DMA_CHANNEL_5:
                IFS3bits.DMA5IF = 0;                
                break;       
        case DMA_CHANNEL_6:
                IFS4bits.DMA6IF = 0;                
                break;       
        case DMA_CHANNEL_7:
                IFS4bits.DMA7IF = 0;                
                break;       
        case DMA_CHANNEL_8:
                IFS7bits.DMA8IF = 0;                
                break;       
        case DMA_CHANNEL_9:
                IFS7bits.DMA9IF = 0;                
                break;       
        case DMA_CHANNEL_10:
                IFS7bits.DMA10IF = 0;                
                break;       
        case DMA_CHANNEL_11:
                IFS7bits.DMA11IF = 0;                
                break;       
        case DMA_CHANNEL_12:
                IFS8bits.DMA12IF = 0;                
                break;       
        case DMA_CHANNEL_13:
                IFS8bits.DMA13IF = 0;                
                break;       
        case DMA_CHANNEL_14:
                IFS8bits.DMA14IF = 0;                
                break;       
        default:break;
         
    }
}

/**
@Summary
    Enables the interrupt for a DMA channel

  @Description
    This routine is used to enable an interrupt for a DMA channel. This routine
    sets the value of the DMAIE bit to 1.
 
  @Preconditions
    DMA_Initializer() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    None
  
  @Example
    Refer to DMA_Initializer(); for an example
 */ 
inline static void DMA_InterruptEnable(DMA_CHANNEL  channel)
{    
    switch(channel) { 
        case DMA_CHANNEL_0:
                IEC0bits.DMA0IE = 1;                
                break;       
        case DMA_CHANNEL_1:
                IEC0bits.DMA1IE = 1;                
                break;       
        case DMA_CHANNEL_2:
                IEC1bits.DMA2IE = 1;                
                break;       
        case DMA_CHANNEL_3:
                IEC2bits.DMA3IE = 1;                
                break;       
        case DMA_CHANNEL_4:
                IEC2bits.DMA4IE = 1;                
                break;       
        case DMA_CHANNEL_5:
                IEC3bits.DMA5IE = 1;                
                break;       
        case DMA_CHANNEL_6:
                IEC4bits.DMA6IE = 1;                
                break;       
        case DMA_CHANNEL_7:
                IEC4bits.DMA7IE = 1;                
                break;       
        case DMA_CHANNEL_8:
                IEC7bits.DMA8IE = 1;                
                break;       
        case DMA_CHANNEL_9:
                IEC7bits.DMA9IE = 1;                
                break;       
        case DMA_CHANNEL_10:
                IEC7bits.DMA10IE = 1;                
                break;       
        case DMA_CHANNEL_11:
                IEC7bits.DMA11IE = 1;                
                break;       
        case DMA_CHANNEL_12:
                IEC8bits.DMA12IE = 1;                
                break;       
        case DMA_CHANNEL_13:
                IEC8bits.DMA13IE = 1;                
                break;       
        case DMA_CHANNEL_14:
                IEC8bits.DMA14IE = 1;                
                break;       
        default:break;
         
    }
}
/**
@Summary
 Disables the interrupt for a DMA channel

@Description
 This routine is used to disable an interrupt for a DMA channel. This routine
 sets the value of the DMAIE bit to 0.
 
@Preconditions
  DMA_Initializer() function should have been 
  called before calling this function.
 
@Returns
 None

@Param
 None
  
@Example
 Refer to DMA_Initializer(); for an example
*/  
inline static void DMA_InterruptDisable(DMA_CHANNEL  channel)
{    
    switch(channel) {        
        case DMA_CHANNEL_0:
                IEC0bits.DMA0IE = 0;                
                break;       
        case DMA_CHANNEL_1:
                IEC0bits.DMA1IE = 0;                
                break;       
        case DMA_CHANNEL_2:
                IEC1bits.DMA2IE = 0;                
                break;       
        case DMA_CHANNEL_3:
                IEC2bits.DMA3IE = 0;                
                break;       
        case DMA_CHANNEL_4:
                IEC2bits.DMA4IE = 0;                
                break;       
        case DMA_CHANNEL_5:
                IEC3bits.DMA5IE = 0;                
                break;       
        case DMA_CHANNEL_6:
                IEC4bits.DMA6IE = 0;                
                break;       
        case DMA_CHANNEL_7:
                IEC4bits.DMA7IE = 0;                
                break;       
        case DMA_CHANNEL_8:
                IEC7bits.DMA8IE = 0;                
                break;       
        case DMA_CHANNEL_9:
                IEC7bits.DMA9IE = 0;                
                break;       
        case DMA_CHANNEL_10:
                IEC7bits.DMA10IE = 0;                
                break;       
        case DMA_CHANNEL_11:
                IEC7bits.DMA11IE = 0;                
                break;       
        case DMA_CHANNEL_12:
                IEC8bits.DMA12IE = 0;                
                break;       
        case DMA_CHANNEL_13:
                IEC8bits.DMA13IE = 0;                
                break;       
        case DMA_CHANNEL_14:
                IEC8bits.DMA14IE = 0;                
                break;       
        default:break;
    }
}
    
/**
 @Summary
  Enables the channel in the DMA

 @Description
  This routine is used to enable a channel in the DMA. This routine
  sets the value of the CHEN bit to 1.
 
 @Preconditions
  DMA_Initializer() function should have been 
  called before calling this function.
 
 @Returns
  None

 @Param
  None
  
 @Example
 Refer to DMA_Initializer(); for an example
 */
inline static void DMA_ChannelEnable(DMA_CHANNEL  channel)
{
    switch(channel) {
        case DMA_CHANNEL_0:
                DMA0CONbits.CHEN = 1;
                break;     
        case DMA_CHANNEL_1:
                DMA1CONbits.CHEN = 1;
                break;     
        case DMA_CHANNEL_2:
                DMA2CONbits.CHEN = 1;
                break;     
        case DMA_CHANNEL_3:
                DMA3CONbits.CHEN = 1;
                break;     
        case DMA_CHANNEL_4:
                DMA4CONbits.CHEN = 1;
                break;     
        case DMA_CHANNEL_5:
                DMA5CONbits.CHEN = 1;
                break;     
        case DMA_CHANNEL_6:
                DMA6CONbits.CHEN = 1;
                break;     
        case DMA_CHANNEL_7:
                DMA7CONbits.CHEN = 1;
                break;     
        case DMA_CHANNEL_8:
                DMA8CONbits.CHEN = 1;
                break;     
        case DMA_CHANNEL_9:
                DMA9CONbits.CHEN = 1;
                break;     
        case DMA_CHANNEL_10:
                DMA10CONbits.CHEN = 1;
                break;     
        case DMA_CHANNEL_11:
                DMA11CONbits.CHEN = 1;
                break;     
        case DMA_CHANNEL_12:
                DMA12CONbits.CHEN = 1;
                break;     
        case DMA_CHANNEL_13:
                DMA13CONbits.CHEN = 1;
                break;     
        case DMA_CHANNEL_14:
                DMA14CONbits.CHEN = 1;
                break;     
        default: break;
    }
}
    
/**
 @Summary
 Disables the channel in the DMA

 @Description
  This routine is used to disable a channel in the DMA. This routine
  sets the value of the CHEN bit to 0.
 
@Preconditions
 DMA_Initializer() function should have been 
 called before calling this function.
 
@Returns
 None

@Param
 None
  
@Example
 Refer to DMA_Initializer(); for an example
*/
inline static void DMA_ChannelDisable(DMA_CHANNEL  channel)
{
    switch(channel) {
        case DMA_CHANNEL_0:
                DMA0CONbits.CHEN = 0;
                break;    
        case DMA_CHANNEL_1:
                DMA1CONbits.CHEN = 0;
                break;    
        case DMA_CHANNEL_2:
                DMA2CONbits.CHEN = 0;
                break;    
        case DMA_CHANNEL_3:
                DMA3CONbits.CHEN = 0;
                break;    
        case DMA_CHANNEL_4:
                DMA4CONbits.CHEN = 0;
                break;    
        case DMA_CHANNEL_5:
                DMA5CONbits.CHEN = 0;
                break;    
        case DMA_CHANNEL_6:
                DMA6CONbits.CHEN = 0;
                break;    
        case DMA_CHANNEL_7:
                DMA7CONbits.CHEN = 0;
                break;    
        case DMA_CHANNEL_8:
                DMA8CONbits.CHEN = 0;
                break;    
        case DMA_CHANNEL_9:
                DMA9CONbits.CHEN = 0;
                break;    
        case DMA_CHANNEL_10:
                DMA10CONbits.CHEN = 0;
                break;    
        case DMA_CHANNEL_11:
                DMA11CONbits.CHEN = 0;
                break;    
        case DMA_CHANNEL_12:
                DMA12CONbits.CHEN = 0;
                break;    
        case DMA_CHANNEL_13:
                DMA13CONbits.CHEN = 0;
                break;    
        case DMA_CHANNEL_14:
                DMA14CONbits.CHEN = 0;
                break;    
        default: break;
    }
}
/**
  @Summary
    Sets the transfer count of the DMA

  @Description
    This routine is used to set the DMA transfer count. This routine sets the
    value of the DMACNT register. 
 
  @Preconditions
    DMA_Initializer() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    None
  
  @Example
    Refer to DMA_Initializer(); for an example
 */
inline static void DMA_TransferCountSet(DMA_CHANNEL channel, uint16_t transferCount)
{
    switch(channel) {
        case DMA_CHANNEL_0:
                DMA0CNT = transferCount;
                break;
        case DMA_CHANNEL_1:
                DMA1CNT = transferCount;
                break;
        case DMA_CHANNEL_2:
                DMA2CNT = transferCount;
                break;
        case DMA_CHANNEL_3:
                DMA3CNT = transferCount;
                break;
        case DMA_CHANNEL_4:
                DMA4CNT = transferCount;
                break;
        case DMA_CHANNEL_5:
                DMA5CNT = transferCount;
                break;
        case DMA_CHANNEL_6:
                DMA6CNT = transferCount;
                break;
        case DMA_CHANNEL_7:
                DMA7CNT = transferCount;
                break;
        case DMA_CHANNEL_8:
                DMA8CNT = transferCount;
                break;
        case DMA_CHANNEL_9:
                DMA9CNT = transferCount;
                break;
        case DMA_CHANNEL_10:
                DMA10CNT = transferCount;
                break;
        case DMA_CHANNEL_11:
                DMA11CNT = transferCount;
                break;
        case DMA_CHANNEL_12:
                DMA12CNT = transferCount;
                break;
        case DMA_CHANNEL_13:
                DMA13CNT = transferCount;
                break;
        case DMA_CHANNEL_14:
                DMA14CNT = transferCount;
                break;
        default: break;
    }
}
/**
 @Summary
  Returns the transfer count of the DMA

@Description
 This routine is used to determine the DMA transfer count. This routine
 returns the value of the DMACNT register. 
 
@Preconditions
  DMA_Initializer() function should have been 
  called before calling this function.
 
@Returns
 Returns the transfer count of the DMA

@Param
 None
  
@Example
 Refer to DMA_Initializer(); for an example
*/
inline static uint16_t DMA_TransferCountGet(DMA_CHANNEL channel)
    {
    switch(channel) {
            case DMA_CHANNEL_0:
                return (DMA0CNT);               
            case DMA_CHANNEL_1:
                return (DMA1CNT);               
            case DMA_CHANNEL_2:
                return (DMA2CNT);               
            case DMA_CHANNEL_3:
                return (DMA3CNT);               
            case DMA_CHANNEL_4:
                return (DMA4CNT);               
            case DMA_CHANNEL_5:
                return (DMA5CNT);               
            case DMA_CHANNEL_6:
                return (DMA6CNT);               
            case DMA_CHANNEL_7:
                return (DMA7CNT);               
            case DMA_CHANNEL_8:
                return (DMA8CNT);               
            case DMA_CHANNEL_9:
                return (DMA9CNT);               
            case DMA_CHANNEL_10:
                return (DMA10CNT);               
            case DMA_CHANNEL_11:
                return (DMA11CNT);               
            case DMA_CHANNEL_12:
                return (DMA12CNT);               
            case DMA_CHANNEL_13:
                return (DMA13CNT);               
            case DMA_CHANNEL_14:
                return (DMA14CNT);               
        default: return 0;
    }
}

/**
 @Summary
  Initiates a transfer on the requested DMA channel.

 @Description
  This routine is used to initiate a transfer on the requested DMA channel. When a transfer on the requested channel is initiated the routine
  returns the value of the FORCE bit. It returns 0 otherwise.
 
 @Preconditions
   DMA_Initializer() function should have been 
   called before calling this function.
 
 @Returns
  Returns true if the transfer on the requested channel is initiated 

 @Param
  None
  
 @Example
  Refer to DMA_Initializer(); for an example
 */
inline static void DMA_SoftwareTriggerEnable(DMA_CHANNEL channel )
{
    switch(channel) {
        case DMA_CHANNEL_0: 
                DMA0REQbits.FORCE = 1;
                break;
        case DMA_CHANNEL_1: 
                DMA1REQbits.FORCE = 1;
                break;
        case DMA_CHANNEL_2: 
                DMA2REQbits.FORCE = 1;
                break;
        case DMA_CHANNEL_3: 
                DMA3REQbits.FORCE = 1;
                break;
        case DMA_CHANNEL_4: 
                DMA4REQbits.FORCE = 1;
                break;
        case DMA_CHANNEL_5: 
                DMA5REQbits.FORCE = 1;
                break;
        case DMA_CHANNEL_6: 
                DMA6REQbits.FORCE = 1;
                break;
        case DMA_CHANNEL_7: 
                DMA7REQbits.FORCE = 1;
                break;
        case DMA_CHANNEL_8: 
                DMA8REQbits.FORCE = 1;
                break;
        case DMA_CHANNEL_9: 
                DMA9REQbits.FORCE = 1;
                break;
        case DMA_CHANNEL_10: 
                DMA10REQbits.FORCE = 1;
                break;
        case DMA_CHANNEL_11: 
                DMA11REQbits.FORCE = 1;
                break;
        case DMA_CHANNEL_12: 
                DMA12REQbits.FORCE = 1;
                break;
        case DMA_CHANNEL_13: 
                DMA13REQbits.FORCE = 1;
                break;
        case DMA_CHANNEL_14: 
                DMA14REQbits.FORCE = 1;
                break;
        default: break;
    }
}  
    
/**
  @Summary
    Sets the address for register A in the DMA

  @Description
    This routine is used to set the address in register A for a DMA channel. 
 
  @Preconditions
    DMA_Initializer() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    None
  
  @Example
    Refer to DMA_Initializer(); for an example
 */
inline static void DMA_StartAddressASet(DMA_CHANNEL  channel, uint16_t address) 
{
    switch(channel) {
        case DMA_CHANNEL_0:
                DMA0STAL = address;
                DMA0STAH = 0;
                break;        
        case DMA_CHANNEL_1:
                DMA1STAL = address;
                DMA1STAH = 0;
                break;        
        case DMA_CHANNEL_2:
                DMA2STAL = address;
                DMA2STAH = 0;
                break;        
        case DMA_CHANNEL_3:
                DMA3STAL = address;
                DMA3STAH = 0;
                break;        
        case DMA_CHANNEL_4:
                DMA4STAL = address;
                DMA4STAH = 0;
                break;        
        case DMA_CHANNEL_5:
                DMA5STAL = address;
                DMA5STAH = 0;
                break;        
        case DMA_CHANNEL_6:
                DMA6STAL = address;
                DMA6STAH = 0;
                break;        
        case DMA_CHANNEL_7:
                DMA7STAL = address;
                DMA7STAH = 0;
                break;        
        case DMA_CHANNEL_8:
                DMA8STAL = address;
                DMA8STAH = 0;
                break;        
        case DMA_CHANNEL_9:
                DMA9STAL = address;
                DMA9STAH = 0;
                break;        
        case DMA_CHANNEL_10:
                DMA10STAL = address;
                DMA10STAH = 0;
                break;        
        case DMA_CHANNEL_11:
                DMA11STAL = address;
                DMA11STAH = 0;
                break;        
        case DMA_CHANNEL_12:
                DMA12STAL = address;
                DMA12STAH = 0;
                break;        
        case DMA_CHANNEL_13:
                DMA13STAL = address;
                DMA13STAH = 0;
                break;        
        case DMA_CHANNEL_14:
                DMA14STAL = address;
                DMA14STAH = 0;
                break;        
        default: break;
    }    
    }
    
/**
  @Summary
    Sets the address for register B in the DMA

  @Description
    This routine is used to set the address in register B for a DMA channel. 
 
  @Preconditions
    DMA_Initializer() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    None
  
  @Example
    Refer to DMA_Initializer(); for an example
 */
 
inline static void DMA_StartAddressBSet(DMA_CHANNEL  channel, uint16_t address) 
{
    switch(channel) {
        case DMA_CHANNEL_0:
                DMA0STBL = address;
                DMA0STBH = 0;
                break;       
        case DMA_CHANNEL_1:
                DMA1STBL = address;
                DMA1STBH = 0;
                break;       
        case DMA_CHANNEL_2:
                DMA2STBL = address;
                DMA2STBH = 0;
                break;       
        case DMA_CHANNEL_3:
                DMA3STBL = address;
                DMA3STBH = 0;
                break;       
        case DMA_CHANNEL_4:
                DMA4STBL = address;
                DMA4STBH = 0;
                break;       
        case DMA_CHANNEL_5:
                DMA5STBL = address;
                DMA5STBH = 0;
                break;       
        case DMA_CHANNEL_6:
                DMA6STBL = address;
                DMA6STBH = 0;
                break;       
        case DMA_CHANNEL_7:
                DMA7STBL = address;
                DMA7STBH = 0;
                break;       
        case DMA_CHANNEL_8:
                DMA8STBL = address;
                DMA8STBH = 0;
                break;       
        case DMA_CHANNEL_9:
                DMA9STBL = address;
                DMA9STBH = 0;
                break;       
        case DMA_CHANNEL_10:
                DMA10STBL = address;
                DMA10STBH = 0;
                break;       
        case DMA_CHANNEL_11:
                DMA11STBL = address;
                DMA11STBH = 0;
                break;       
        case DMA_CHANNEL_12:
                DMA12STBL = address;
                DMA12STBH = 0;
                break;       
        case DMA_CHANNEL_13:
                DMA13STBL = address;
                DMA13STBH = 0;
                break;       
        case DMA_CHANNEL_14:
                DMA14STBL = address;
                DMA14STBH = 0;
                break;       
        default: break;
    }    
}

  /**
  @Summary
    Gets the address for register A in the DMA

  @Description
    This routine is used to get the address in register A for a DMA channel. 
 
  @Preconditions
    DMA_Initializer() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    None
  
  @Example
    Refer to DMA_Initializer(); for an example
 */
inline static uint16_t DMA_StartAddressAGet(DMA_CHANNEL  channel)
{    
    uint16_t address;

    switch(channel) {
        case DMA_CHANNEL_0:    
                address= DMA0STAL;       
                break;       
        case DMA_CHANNEL_1:    
                address= DMA1STAL;       
                break;       
        case DMA_CHANNEL_2:    
                address= DMA2STAL;       
                break;       
        case DMA_CHANNEL_3:    
                address= DMA3STAL;       
                break;       
        case DMA_CHANNEL_4:    
                address= DMA4STAL;       
                break;       
        case DMA_CHANNEL_5:    
                address= DMA5STAL;       
                break;       
        case DMA_CHANNEL_6:    
                address= DMA6STAL;       
                break;       
        case DMA_CHANNEL_7:    
                address= DMA7STAL;       
                break;       
        case DMA_CHANNEL_8:    
                address= DMA8STAL;       
                break;       
        case DMA_CHANNEL_9:    
                address= DMA9STAL;       
                break;       
        case DMA_CHANNEL_10:    
                address= DMA10STAL;       
                break;       
        case DMA_CHANNEL_11:    
                address= DMA11STAL;       
                break;       
        case DMA_CHANNEL_12:    
                address= DMA12STAL;       
                break;       
        case DMA_CHANNEL_13:    
                address= DMA13STAL;       
                break;       
        case DMA_CHANNEL_14:    
                address= DMA14STAL;       
                break;       
        default:
                address = 0;
                break;
    }
    return address;            
}

  /**
  @Summary
    Gets the address for register B in the DMA

  @Description
    This routine is used to get the address in register B for a DMA channel. 
 
  @Preconditions
    DMA_Initializer() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    None
  
  @Example
    Refer to DMA_Initializer(); for an example
 */ 
inline static uint16_t DMA_StartAddressBGet(DMA_CHANNEL  channel)  
{        
    uint16_t address;

    switch(channel) {
        case DMA_CHANNEL_0:   
                address= DMA0STBL;       
                break;        
        case DMA_CHANNEL_1:   
                address= DMA1STBL;       
                break;        
        case DMA_CHANNEL_2:   
                address= DMA2STBL;       
                break;        
        case DMA_CHANNEL_3:   
                address= DMA3STBL;       
                break;        
        case DMA_CHANNEL_4:   
                address= DMA4STBL;       
                break;        
        case DMA_CHANNEL_5:   
                address= DMA5STBL;       
                break;        
        case DMA_CHANNEL_6:   
                address= DMA6STBL;       
                break;        
        case DMA_CHANNEL_7:   
                address= DMA7STBL;       
                break;        
        case DMA_CHANNEL_8:   
                address= DMA8STBL;       
                break;        
        case DMA_CHANNEL_9:   
                address= DMA9STBL;       
                break;        
        case DMA_CHANNEL_10:   
                address= DMA10STBL;       
                break;        
        case DMA_CHANNEL_11:   
                address= DMA11STBL;       
                break;        
        case DMA_CHANNEL_12:   
                address= DMA12STBL;       
                break;        
        case DMA_CHANNEL_13:   
                address= DMA13STBL;       
                break;        
        case DMA_CHANNEL_14:   
                address= DMA14STBL;       
                break;        
        default:
                address = 0;
                break;
    }
        return address;
}

    /**
    @Summary
    Sets the peripheral address in the DMA

  @Description
    This routine is used to set the peripheral address for a DMA channel. 
 
  @Preconditions
    DMA_Initializer() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    None
  
  @Example
    Refer to DMA_Initializer(); for an example
 */
 inline static void DMA_PeripheralAddressSet(DMA_CHANNEL channel, uint16_t address)
    {
    switch(channel) {
        case DMA_CHANNEL_0:
                DMA0PAD = address;
                break;   
        case DMA_CHANNEL_1:
                DMA1PAD = address;
                break;   
        case DMA_CHANNEL_2:
                DMA2PAD = address;
                break;   
        case DMA_CHANNEL_3:
                DMA3PAD = address;
                break;   
        case DMA_CHANNEL_4:
                DMA4PAD = address;
                break;   
        case DMA_CHANNEL_5:
                DMA5PAD = address;
                break;   
        case DMA_CHANNEL_6:
                DMA6PAD = address;
                break;   
        case DMA_CHANNEL_7:
                DMA7PAD = address;
                break;   
        case DMA_CHANNEL_8:
                DMA8PAD = address;
                break;   
        case DMA_CHANNEL_9:
                DMA9PAD = address;
                break;   
        case DMA_CHANNEL_10:
                DMA10PAD = address;
                break;   
        case DMA_CHANNEL_11:
                DMA11PAD = address;
                break;   
        case DMA_CHANNEL_12:
                DMA12PAD = address;
                break;   
        case DMA_CHANNEL_13:
                DMA13PAD = address;
                break;   
        case DMA_CHANNEL_14:
                DMA14PAD = address;
                break;   
        default: break;
    }
}
/**
  @Summary
    Returns true when there is a Peripheral Write Collision Event

  @Description
    This routine is used to determine if there is a Peripheral Write Collision Event. This routine
    returns the value of the PWCOL bit in DMAPWC register. When there is a Peripheral Write Collision Event, the routine
    returns 1. It returns 0 otherwise.
 
  @Preconditions
    DMA_Initializer() function should have been 
    called before calling this function.
 
  @Returns
    Returns true if there is a Peripheral Write Collision Event

  @Param
    None
  
  @Example
    Refer to DMA_Initializer(); for an example
 */
inline static bool DMA_IsPeripheralWriteCollision(uint16_t dmaChannel )
{
    return DMAPWC & (1 << dmaChannel);
}

/**
  @Summary
    Returns true when there is a Request Collision Event

  @Description
    This routine is used to determine if there is a Request Collision Event. This routine
    returns the value of the RQCOL bit in DMARQC register. When there is a Request Collision Event, the routine
    returns 1. It returns 0 otherwise.
 
  @Preconditions
    DMA_Initializer() function should have been 
    called before calling this function.
 
  @Returns
    Returns true if there is a Request Collision Event

  @Param
    None
  
  @Example
    Refer to DMA_Initializer(); for an example
 */
inline static bool DMA_IsRequestCollision(uint16_t dmaChannel )
{
    return DMARQC & (1 << dmaChannel);
}

/**
  @Summary
    Sets the requested DMA Channel IRQ Select register with the requested peripheral IRQ number.

  @Description
    This routine is used to set the requested DMA Channel IRQ Select register with the requested peripheral IRQ number.
 
  @Preconditions
    DMA_Initializer() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    None
  
  @Example
    Refer to DMA_Initializer(); for an example
 */
inline static void DMA_PeripheralIrqNumberSet(DMA_CHANNEL channel, DMA_PERIPHERAL_IRQ_NUMBER irqNumber)
{
    switch(channel) {
        case DMA_CHANNEL_0:
                DMA0REQbits.IRQSEL = irqNumber;
                break;   
        case DMA_CHANNEL_1:
                DMA1REQbits.IRQSEL = irqNumber;
                break;   
        case DMA_CHANNEL_2:
                DMA2REQbits.IRQSEL = irqNumber;
                break;   
        case DMA_CHANNEL_3:
                DMA3REQbits.IRQSEL = irqNumber;
                break;   
        case DMA_CHANNEL_4:
                DMA4REQbits.IRQSEL = irqNumber;
                break;   
        case DMA_CHANNEL_5:
                DMA5REQbits.IRQSEL = irqNumber;
                break;   
        case DMA_CHANNEL_6:
                DMA6REQbits.IRQSEL = irqNumber;
                break;   
        case DMA_CHANNEL_7:
                DMA7REQbits.IRQSEL = irqNumber;
                break;   
        case DMA_CHANNEL_8:
                DMA8REQbits.IRQSEL = irqNumber;
                break;   
        case DMA_CHANNEL_9:
                DMA9REQbits.IRQSEL = irqNumber;
                break;   
        case DMA_CHANNEL_10:
                DMA10REQbits.IRQSEL = irqNumber;
                break;   
        case DMA_CHANNEL_11:
                DMA11REQbits.IRQSEL = irqNumber;
                break;   
        case DMA_CHANNEL_12:
                DMA12REQbits.IRQSEL = irqNumber;
                break;   
        case DMA_CHANNEL_13:
                DMA13REQbits.IRQSEL = irqNumber;
                break;   
        case DMA_CHANNEL_14:
                DMA14REQbits.IRQSEL = irqNumber;
                break;   
        default: break;
    }
}

/**
  @Summary
    Callback for DMA Channel0.

  @Description
    This routine is callback for DMA Channel0

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to DMA_Initialize(); for an example
*/
void DMA_Channel0_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    DMA_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to DMA_Initialize(); for an example
    
*/
void DMA_Channel0_Tasks(void);
/**
  @Summary
    Callback for DMA Channel1.

  @Description
    This routine is callback for DMA Channel1

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to DMA_Initialize(); for an example
*/
void DMA_Channel1_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    DMA_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to DMA_Initialize(); for an example
    
*/
void DMA_Channel1_Tasks(void);
/**
  @Summary
    Callback for DMA Channel2.

  @Description
    This routine is callback for DMA Channel2

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to DMA_Initialize(); for an example
*/
void DMA_Channel2_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    DMA_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to DMA_Initialize(); for an example
    
*/
void DMA_Channel2_Tasks(void);
/**
  @Summary
    Callback for DMA Channel3.

  @Description
    This routine is callback for DMA Channel3

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to DMA_Initialize(); for an example
*/
void DMA_Channel3_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    DMA_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to DMA_Initialize(); for an example
    
*/
void DMA_Channel3_Tasks(void);
/**
  @Summary
    Callback for DMA Channel4.

  @Description
    This routine is callback for DMA Channel4

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to DMA_Initialize(); for an example
*/
void DMA_Channel4_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    DMA_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to DMA_Initialize(); for an example
    
*/
void DMA_Channel4_Tasks(void);
/**
  @Summary
    Callback for DMA Channel5.

  @Description
    This routine is callback for DMA Channel5

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to DMA_Initialize(); for an example
*/
void DMA_Channel5_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    DMA_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to DMA_Initialize(); for an example
    
*/
void DMA_Channel5_Tasks(void);
/**
  @Summary
    Callback for DMA Channel6.

  @Description
    This routine is callback for DMA Channel6

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to DMA_Initialize(); for an example
*/
void DMA_Channel6_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    DMA_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to DMA_Initialize(); for an example
    
*/
void DMA_Channel6_Tasks(void);
/**
  @Summary
    Callback for DMA Channel7.

  @Description
    This routine is callback for DMA Channel7

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to DMA_Initialize(); for an example
*/
void DMA_Channel7_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    DMA_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to DMA_Initialize(); for an example
    
*/
void DMA_Channel7_Tasks(void);
/**
  @Summary
    Callback for DMA Channel8.

  @Description
    This routine is callback for DMA Channel8

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to DMA_Initialize(); for an example
*/
void DMA_Channel8_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    DMA_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to DMA_Initialize(); for an example
    
*/
void DMA_Channel8_Tasks(void);
/**
  @Summary
    Callback for DMA Channel9.

  @Description
    This routine is callback for DMA Channel9

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to DMA_Initialize(); for an example
*/
void DMA_Channel9_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    DMA_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to DMA_Initialize(); for an example
    
*/
void DMA_Channel9_Tasks(void);
/**
  @Summary
    Callback for DMA Channel10.

  @Description
    This routine is callback for DMA Channel10

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to DMA_Initialize(); for an example
*/
void DMA_Channel10_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    DMA_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to DMA_Initialize(); for an example
    
*/
void DMA_Channel10_Tasks(void);
/**
  @Summary
    Callback for DMA Channel11.

  @Description
    This routine is callback for DMA Channel11

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to DMA_Initialize(); for an example
*/
void DMA_Channel11_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    DMA_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to DMA_Initialize(); for an example
    
*/
void DMA_Channel11_Tasks(void);
/**
  @Summary
    Callback for DMA Channel12.

  @Description
    This routine is callback for DMA Channel12

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to DMA_Initialize(); for an example
*/
void DMA_Channel12_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    DMA_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to DMA_Initialize(); for an example
    
*/
void DMA_Channel12_Tasks(void);
/**
  @Summary
    Callback for DMA Channel13.

  @Description
    This routine is callback for DMA Channel13

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to DMA_Initialize(); for an example
*/
void DMA_Channel13_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    DMA_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to DMA_Initialize(); for an example
    
*/
void DMA_Channel13_Tasks(void);
/**
  @Summary
    Callback for DMA Channel14.

  @Description
    This routine is callback for DMA Channel14

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to DMA_Initialize(); for an example
*/
void DMA_Channel14_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    DMA_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to DMA_Initialize(); for an example
    
*/
void DMA_Channel14_Tasks(void);

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
    
#endif  // DMA.h


/**
  End of File
*/
