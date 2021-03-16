/**
  DMA Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    dma.c

  @Summary
    This is the generated driver implementation file for the DMA driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for DMA.
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

#include "dma.h"

void DMA_Initialize(void) 
{ 
    // Initialize channels which are enabled    

    // AMODE Peripheral Indirect Addressing mode; CHEN disabled; DIR Reads from RAM address, writes to peripheral address; HALF Initiates interrupt when all of the data has been moved; SIZE 16 bit; NULLW disabled; MODE Continuous, Ping-Pong modes are disabled; 
    DMA0CON= 0x2020 & 0x7FFF; //Enable DMA Channel later;
    // FORCE disabled; IRQSEL CAN1 TX; 
    DMA0REQ= 0x46;
    // STA 0; 
    DMA0STAH= 0x00;
    // STA 4096; 
    DMA0STAL= 0x1000;
    // STB 0; 
    DMA0STBH= 0x00;
    // STB 0; 
    DMA0STBL= 0x00;
    // PAD 0; 
    DMA0PAD= 0x00;
    // CNT 7; 
    DMA0CNT= 0x07;
    // Clearing Channel 0 Interrupt Flag;
    IFS0bits.DMA0IF = false;
    // Enabling Channel 0 Interrupt

    // AMODE Peripheral Indirect Addressing mode; CHEN disabled; SIZE 16 bit; DIR Reads from peripheral address, writes to RAM address; NULLW disabled; HALF Initiates interrupt when all of the data has been moved; MODE Continuous, Ping-Pong modes are disabled; 
    DMA1CON= 0x20 & 0x7FFF; //Enable DMA Channel later;
    // FORCE disabled; IRQSEL CAN1 RX; 
    DMA1REQ= 0x22;
    // STA 0; 
    DMA1STAH= 0x00;
    // STA 4096; 
    DMA1STAL= 0x1000;
    // STB 0; 
    DMA1STBH= 0x00;
    // STB 0; 
    DMA1STBL= 0x00;
    // PAD 0; 
    DMA1PAD= 0x00;
    // CNT 7; 
    DMA1CNT= 0x07;
    // Clearing Channel 1 Interrupt Flag;
    IFS0bits.DMA1IF = false;
    // Enabling Channel 1 Interrupt

    // AMODE Register Indirect with Post-Increment mode; CHEN disabled; SIZE 16 bit; DIR Reads from peripheral address, writes to RAM address; NULLW disabled; HALF Initiates interrupt when all of the data has been moved; MODE Continuous, Ping-Pong modes are disabled; 
    DMA2CON= 0x00 & 0x7FFF; //Enable DMA Channel later;
    // IRQSEL INT0; FORCE disabled; 
    DMA2REQ= 0x00;
    // STA 0; 
    DMA2STAH= 0x00;
    // STA 4096; 
    DMA2STAL= 0x1000;
    // STB 0; 
    DMA2STBH= 0x00;
    // STB 0; 
    DMA2STBL= 0x00;
    // PAD 0; 
    DMA2PAD= 0x00;
    // CNT 0; 
    DMA2CNT= 0x00;
    // Clearing Channel 2 Interrupt Flag;
    IFS1bits.DMA2IF = false;
    // Enabling Channel 2 Interrupt

    // MODE Continuous, Ping-Pong modes are disabled; AMODE Register Indirect with Post-Increment mode; CHEN disabled; HALF Initiates interrupt when all of the data has been moved; SIZE 16 bit; DIR Reads from peripheral address, writes to RAM address; NULLW disabled; 
    DMA3CON= 0x00 & 0x7FFF; //Enable DMA Channel later;
    // IRQSEL INT0; FORCE disabled; 
    DMA3REQ= 0x00;
    // STA 0; 
    DMA3STAH= 0x00;
    // STA 4096; 
    DMA3STAL= 0x1000;
    // STB 0; 
    DMA3STBH= 0x00;
    // STB 0; 
    DMA3STBL= 0x00;
    // PAD 0; 
    DMA3PAD= 0x00;
    // CNT 0; 
    DMA3CNT= 0x00;
    // Clearing Channel 3 Interrupt Flag;
    IFS2bits.DMA3IF = false;
    // Enabling Channel 3 Interrupt

    // MODE Continuous, Ping-Pong modes are disabled; HALF Initiates interrupt when all of the data has been moved; CHEN disabled; SIZE 16 bit; DIR Reads from peripheral address, writes to RAM address; AMODE Register Indirect with Post-Increment mode; NULLW disabled; 
    DMA4CON= 0x00 & 0x7FFF; //Enable DMA Channel later;
    // IRQSEL INT0; FORCE disabled; 
    DMA4REQ= 0x00;
    // STA 0; 
    DMA4STAH= 0x00;
    // STA 4096; 
    DMA4STAL= 0x1000;
    // STB 0; 
    DMA4STBH= 0x00;
    // STB 0; 
    DMA4STBL= 0x00;
    // PAD 0; 
    DMA4PAD= 0x00;
    // CNT 0; 
    DMA4CNT= 0x00;
    // Clearing Channel 4 Interrupt Flag;
    IFS2bits.DMA4IF = false;
    // Enabling Channel 4 Interrupt

    // SIZE 16 bit; MODE Continuous, Ping-Pong modes are disabled; CHEN disabled; HALF Initiates interrupt when all of the data has been moved; DIR Reads from peripheral address, writes to RAM address; AMODE Register Indirect with Post-Increment mode; NULLW disabled; 
    DMA5CON= 0x00 & 0x7FFF; //Enable DMA Channel later;
    // IRQSEL INT0; FORCE disabled; 
    DMA5REQ= 0x00;
    // STA 0; 
    DMA5STAH= 0x00;
    // STA 4096; 
    DMA5STAL= 0x1000;
    // STB 0; 
    DMA5STBH= 0x00;
    // STB 0; 
    DMA5STBL= 0x00;
    // PAD 0; 
    DMA5PAD= 0x00;
    // CNT 0; 
    DMA5CNT= 0x00;
    // Clearing Channel 5 Interrupt Flag;
    IFS3bits.DMA5IF = false;
    // Enabling Channel 5 Interrupt

    // SIZE 16 bit; MODE Continuous, Ping-Pong modes are disabled; HALF Initiates interrupt when all of the data has been moved; CHEN disabled; DIR Reads from peripheral address, writes to RAM address; NULLW disabled; AMODE Register Indirect with Post-Increment mode; 
    DMA6CON= 0x00 & 0x7FFF; //Enable DMA Channel later;
    // FORCE disabled; IRQSEL INT0; 
    DMA6REQ= 0x00;
    // STA 0; 
    DMA6STAH= 0x00;
    // STA 4096; 
    DMA6STAL= 0x1000;
    // STB 0; 
    DMA6STBH= 0x00;
    // STB 0; 
    DMA6STBL= 0x00;
    // PAD 0; 
    DMA6PAD= 0x00;
    // CNT 0; 
    DMA6CNT= 0x00;
    // Clearing Channel 6 Interrupt Flag;
    IFS4bits.DMA6IF = false;
    // Enabling Channel 6 Interrupt

    // HALF Initiates interrupt when all of the data has been moved; SIZE 16 bit; MODE Continuous, Ping-Pong modes are disabled; CHEN disabled; DIR Reads from peripheral address, writes to RAM address; NULLW disabled; AMODE Register Indirect with Post-Increment mode; 
    DMA7CON= 0x00 & 0x7FFF; //Enable DMA Channel later;
    // FORCE disabled; IRQSEL INT0; 
    DMA7REQ= 0x00;
    // STA 0; 
    DMA7STAH= 0x00;
    // STA 4096; 
    DMA7STAL= 0x1000;
    // STB 0; 
    DMA7STBH= 0x00;
    // STB 0; 
    DMA7STBL= 0x00;
    // PAD 0; 
    DMA7PAD= 0x00;
    // CNT 0; 
    DMA7CNT= 0x00;
    // Clearing Channel 7 Interrupt Flag;
    IFS4bits.DMA7IF = false;
    // Enabling Channel 7 Interrupt

    // HALF Initiates interrupt when all of the data has been moved; SIZE 16 bit; NULLW disabled; MODE Continuous, Ping-Pong modes are disabled; AMODE Register Indirect with Post-Increment mode; CHEN disabled; DIR Reads from peripheral address, writes to RAM address; 
    DMA8CON= 0x00 & 0x7FFF; //Enable DMA Channel later;
    // FORCE disabled; IRQSEL INT0; 
    DMA8REQ= 0x00;
    // STA 0; 
    DMA8STAH= 0x00;
    // STA 4096; 
    DMA8STAL= 0x1000;
    // STB 0; 
    DMA8STBH= 0x00;
    // STB 0; 
    DMA8STBL= 0x00;
    // PAD 0; 
    DMA8PAD= 0x00;
    // CNT 0; 
    DMA8CNT= 0x00;
    // Clearing Channel 8 Interrupt Flag;
    IFS7bits.DMA8IF = false;
    // Enabling Channel 8 Interrupt

    // NULLW disabled; HALF Initiates interrupt when all of the data has been moved; MODE Continuous, Ping-Pong modes are disabled; AMODE Register Indirect with Post-Increment mode; CHEN disabled; SIZE 16 bit; DIR Reads from peripheral address, writes to RAM address; 
    DMA9CON= 0x00 & 0x7FFF; //Enable DMA Channel later;
    // FORCE disabled; IRQSEL INT0; 
    DMA9REQ= 0x00;
    // STA 0; 
    DMA9STAH= 0x00;
    // STA 4096; 
    DMA9STAL= 0x1000;
    // STB 0; 
    DMA9STBH= 0x00;
    // STB 0; 
    DMA9STBL= 0x00;
    // PAD 0; 
    DMA9PAD= 0x00;
    // CNT 0; 
    DMA9CNT= 0x00;
    // Clearing Channel 9 Interrupt Flag;
    IFS7bits.DMA9IF = false;
    // Enabling Channel 9 Interrupt

    // SIZE 16 bit; AMODE Register Indirect with Post-Increment mode; CHEN disabled; HALF Initiates interrupt when all of the data has been moved; NULLW disabled; DIR Reads from peripheral address, writes to RAM address; MODE Continuous, Ping-Pong modes are disabled; 
    DMA10CON= 0x00 & 0x7FFF; //Enable DMA Channel later;
    // FORCE disabled; IRQSEL INT0; 
    DMA10REQ= 0x00;
    // STA 0; 
    DMA10STAH= 0x00;
    // STA 4096; 
    DMA10STAL= 0x1000;
    // STB 0; 
    DMA10STBH= 0x00;
    // STB 0; 
    DMA10STBL= 0x00;
    // PAD 0; 
    DMA10PAD= 0x00;
    // CNT 0; 
    DMA10CNT= 0x00;
    // Clearing Channel 10 Interrupt Flag;
    IFS7bits.DMA10IF = false;
    // Enabling Channel 10 Interrupt

    // SIZE 16 bit; NULLW disabled; CHEN disabled; HALF Initiates interrupt when all of the data has been moved; AMODE Register Indirect with Post-Increment mode; DIR Reads from peripheral address, writes to RAM address; MODE Continuous, Ping-Pong modes are disabled; 
    DMA11CON= 0x00 & 0x7FFF; //Enable DMA Channel later;
    // FORCE disabled; IRQSEL INT0; 
    DMA11REQ= 0x00;
    // STA 0; 
    DMA11STAH= 0x00;
    // STA 4096; 
    DMA11STAL= 0x1000;
    // STB 0; 
    DMA11STBH= 0x00;
    // STB 0; 
    DMA11STBL= 0x00;
    // PAD 0; 
    DMA11PAD= 0x00;
    // CNT 0; 
    DMA11CNT= 0x00;
    // Clearing Channel 11 Interrupt Flag;
    IFS7bits.DMA11IF = false;
    // Enabling Channel 11 Interrupt

    // HALF Initiates interrupt when all of the data has been moved; SIZE 16 bit; CHEN disabled; NULLW disabled; AMODE Register Indirect with Post-Increment mode; MODE Continuous, Ping-Pong modes are disabled; DIR Reads from peripheral address, writes to RAM address; 
    DMA12CON= 0x00 & 0x7FFF; //Enable DMA Channel later;
    // FORCE disabled; IRQSEL INT0; 
    DMA12REQ= 0x00;
    // STA 0; 
    DMA12STAH= 0x00;
    // STA 4096; 
    DMA12STAL= 0x1000;
    // STB 0; 
    DMA12STBH= 0x00;
    // STB 0; 
    DMA12STBL= 0x00;
    // PAD 0; 
    DMA12PAD= 0x00;
    // CNT 0; 
    DMA12CNT= 0x00;
    // Clearing Channel 12 Interrupt Flag;
    IFS8bits.DMA12IF = false;
    // Enabling Channel 12 Interrupt

    // HALF Initiates interrupt when all of the data has been moved; SIZE 16 bit; AMODE Register Indirect with Post-Increment mode; NULLW disabled; CHEN disabled; MODE Continuous, Ping-Pong modes are disabled; DIR Reads from peripheral address, writes to RAM address; 
    DMA13CON= 0x00 & 0x7FFF; //Enable DMA Channel later;
    // FORCE disabled; IRQSEL INT0; 
    DMA13REQ= 0x00;
    // STA 0; 
    DMA13STAH= 0x00;
    // STA 4096; 
    DMA13STAL= 0x1000;
    // STB 0; 
    DMA13STBH= 0x00;
    // STB 0; 
    DMA13STBL= 0x00;
    // PAD 0; 
    DMA13PAD= 0x00;
    // CNT 0; 
    DMA13CNT= 0x00;
    // Clearing Channel 13 Interrupt Flag;
    IFS8bits.DMA13IF = false;
    // Enabling Channel 13 Interrupt

    // DIR Reads from peripheral address, writes to RAM address; HALF Initiates interrupt when all of the data has been moved; SIZE 16 bit; AMODE Register Indirect with Post-Increment mode; NULLW disabled; MODE Continuous, Ping-Pong modes are disabled; CHEN disabled; 
    DMA14CON= 0x00 & 0x7FFF; //Enable DMA Channel later;
    // FORCE disabled; IRQSEL INT0; 
    DMA14REQ= 0x00;
    // STA 0; 
    DMA14STAH= 0x00;
    // STA 4096; 
    DMA14STAL= 0x1000;
    // STB 0; 
    DMA14STBH= 0x00;
    // STB 0; 
    DMA14STBL= 0x00;
    // PAD 0; 
    DMA14PAD= 0x00;
    // CNT 0; 
    DMA14CNT= 0x00;
    // Clearing Channel 14 Interrupt Flag;
    IFS8bits.DMA14IF = false;
    // Enabling Channel 14 Interrupt

}
void __attribute__ ((weak)) DMA_Channel0_CallBack(void)
{
    // Add your custom callback code here
}

void DMA_Channel0_Tasks( void )
{
	if(IFS0bits.DMA0IF)
	{
		// DMA Channel0 callback function 
		DMA_Channel0_CallBack();
		
		IFS0bits.DMA0IF = 0;
	}
}
void __attribute__ ((weak)) DMA_Channel1_CallBack(void)
{
    // Add your custom callback code here
}

void DMA_Channel1_Tasks( void )
{
	if(IFS0bits.DMA1IF)
	{
		// DMA Channel1 callback function 
		DMA_Channel1_CallBack();
		
		IFS0bits.DMA1IF = 0;
	}
}
void __attribute__ ((weak)) DMA_Channel2_CallBack(void)
{
    // Add your custom callback code here
}

void DMA_Channel2_Tasks( void )
{
	if(IFS1bits.DMA2IF)
	{
		// DMA Channel2 callback function 
		DMA_Channel2_CallBack();
		
		IFS1bits.DMA2IF = 0;
	}
}
void __attribute__ ((weak)) DMA_Channel3_CallBack(void)
{
    // Add your custom callback code here
}

void DMA_Channel3_Tasks( void )
{
	if(IFS2bits.DMA3IF)
	{
		// DMA Channel3 callback function 
		DMA_Channel3_CallBack();
		
		IFS2bits.DMA3IF = 0;
	}
}
void __attribute__ ((weak)) DMA_Channel4_CallBack(void)
{
    // Add your custom callback code here
}

void DMA_Channel4_Tasks( void )
{
	if(IFS2bits.DMA4IF)
	{
		// DMA Channel4 callback function 
		DMA_Channel4_CallBack();
		
		IFS2bits.DMA4IF = 0;
	}
}
void __attribute__ ((weak)) DMA_Channel5_CallBack(void)
{
    // Add your custom callback code here
}

void DMA_Channel5_Tasks( void )
{
	if(IFS3bits.DMA5IF)
	{
		// DMA Channel5 callback function 
		DMA_Channel5_CallBack();
		
		IFS3bits.DMA5IF = 0;
	}
}
void __attribute__ ((weak)) DMA_Channel6_CallBack(void)
{
    // Add your custom callback code here
}

void DMA_Channel6_Tasks( void )
{
	if(IFS4bits.DMA6IF)
	{
		// DMA Channel6 callback function 
		DMA_Channel6_CallBack();
		
		IFS4bits.DMA6IF = 0;
	}
}
void __attribute__ ((weak)) DMA_Channel7_CallBack(void)
{
    // Add your custom callback code here
}

void DMA_Channel7_Tasks( void )
{
	if(IFS4bits.DMA7IF)
	{
		// DMA Channel7 callback function 
		DMA_Channel7_CallBack();
		
		IFS4bits.DMA7IF = 0;
	}
}
void __attribute__ ((weak)) DMA_Channel8_CallBack(void)
{
    // Add your custom callback code here
}

void DMA_Channel8_Tasks( void )
{
	if(IFS7bits.DMA8IF)
	{
		// DMA Channel8 callback function 
		DMA_Channel8_CallBack();
		
		IFS7bits.DMA8IF = 0;
	}
}
void __attribute__ ((weak)) DMA_Channel9_CallBack(void)
{
    // Add your custom callback code here
}

void DMA_Channel9_Tasks( void )
{
	if(IFS7bits.DMA9IF)
	{
		// DMA Channel9 callback function 
		DMA_Channel9_CallBack();
		
		IFS7bits.DMA9IF = 0;
	}
}
void __attribute__ ((weak)) DMA_Channel10_CallBack(void)
{
    // Add your custom callback code here
}

void DMA_Channel10_Tasks( void )
{
	if(IFS7bits.DMA10IF)
	{
		// DMA Channel10 callback function 
		DMA_Channel10_CallBack();
		
		IFS7bits.DMA10IF = 0;
	}
}
void __attribute__ ((weak)) DMA_Channel11_CallBack(void)
{
    // Add your custom callback code here
}

void DMA_Channel11_Tasks( void )
{
	if(IFS7bits.DMA11IF)
	{
		// DMA Channel11 callback function 
		DMA_Channel11_CallBack();
		
		IFS7bits.DMA11IF = 0;
	}
}
void __attribute__ ((weak)) DMA_Channel12_CallBack(void)
{
    // Add your custom callback code here
}

void DMA_Channel12_Tasks( void )
{
	if(IFS8bits.DMA12IF)
	{
		// DMA Channel12 callback function 
		DMA_Channel12_CallBack();
		
		IFS8bits.DMA12IF = 0;
	}
}
void __attribute__ ((weak)) DMA_Channel13_CallBack(void)
{
    // Add your custom callback code here
}

void DMA_Channel13_Tasks( void )
{
	if(IFS8bits.DMA13IF)
	{
		// DMA Channel13 callback function 
		DMA_Channel13_CallBack();
		
		IFS8bits.DMA13IF = 0;
	}
}
void __attribute__ ((weak)) DMA_Channel14_CallBack(void)
{
    // Add your custom callback code here
}

void DMA_Channel14_Tasks( void )
{
	if(IFS8bits.DMA14IF)
	{
		// DMA Channel14 callback function 
		DMA_Channel14_CallBack();
		
		IFS8bits.DMA14IF = 0;
	}
}

/**
  End of File
*/
