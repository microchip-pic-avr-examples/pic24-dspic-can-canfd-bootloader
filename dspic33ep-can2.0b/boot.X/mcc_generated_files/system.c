/**
  @Generated PIC24 / dsPIC33 / PIC32MM MCUs Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    system.h

  @Summary:
    This is the sysetm.h file generated using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  dsPIC33EP512MU810
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB             :  MPLAB X v5.45
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

// Configuration bits: selected in the GUI

// FGS
#pragma config GWRP = OFF    //General Segment Write-Protect bit->General Segment may be written
#pragma config GSS = OFF    //General Segment Code-Protect bit->General Segment Code protect is disabled
#pragma config GSSK = OFF    //General Segment Key bits->General Segment Write Protection and Code Protection is Disabled

// FOSCSEL
#pragma config FNOSC = FRC    //Initial Oscillator Source Selection Bits->FRC
#pragma config IESO = OFF    //Two-speed Oscillator Start-up Enable bit->Start up with user-selected oscillator source

// FOSC
#pragma config POSCMD = XT    //Primary Oscillator Mode Select bits->XT Crystal Oscillator Mode
#pragma config OSCIOFNC = ON    //OSC2 Pin Function bit->OSC2 is general purpose digital I/O pin
#pragma config IOL1WAY = OFF    //Peripheral pin select configuration->Allow multiple reconfigurations
#pragma config FCKSM = CSECMD    //Clock Switching Mode bits->Clock switching is enabled,Fail-safe Clock Monitor is disabled

// FWDT
#pragma config WDTPOST = PS32768    //Watchdog Timer Postscaler Bits->1:32768
#pragma config WDTPRE = PR128    //Watchdog Timer Prescaler bit->1:128
#pragma config PLLKEN = ON    //PLL Lock Wait Enable bit->Clock switch to PLL source will wait until the PLL lock signal is valid.
#pragma config WINDIS = OFF    //Watchdog Timer Window Enable bit->Watchdog Timer in Non-Window mode
#pragma config FWDTEN = OFF    //Watchdog Timer Enable bit->Watchdog timer enabled/disabled by user software

// FPOR
#pragma config FPWRT = PWR128    //Power-on Reset Timer Value Select bits->128ms
#pragma config BOREN = ON    //Brown-out Reset (BOR) Detection Enable bit->BOR is enabled
#pragma config ALTI2C1 = ON    //Alternate I2C pins for I2C1->ASDA1/ASCK1 pins are selected as the I/O pins for I2C1
#pragma config ALTI2C2 = OFF    //Alternate I2C pins for I2C2->SDA2/SCK2 pins are selected as the I/O pins for I2C2

// FICD
#pragma config ICS = PGD1    //ICD Communication Channel Select bits->Communicate on PGEC1 and PGED1
#pragma config RSTPRI = PF    //Reset Target Vector Select bit->Device will obtain reset instruction from Primary flash
#pragma config JTAGEN = OFF    //JTAG Enable bit->JTAG is disabled

// FAS
#pragma config AWRP = OFF    //Auxiliary Segment Write-protect bit->Auxiliary program memory is not write-protected
#pragma config APL = OFF    //Auxiliary Segment Code-protect bit->Aux Flash Code protect is disabled
#pragma config APLK = OFF    //Auxiliary Segment Key bits->Aux Flash Write Protection and Code Protection is Disabled

#include "pin_manager.h"
#include "clock.h"
#include "system.h"
#include "system_types.h"
#include "boot/boot_process.h"
#include "interrupt_manager.h"
#include "traps.h"
#include "delay.h"
#include "can1.h"
#include "memory/flash.h"
#include "dma.h"
#include "uart1.h"
#include "tmr1.h"

void SYSTEM_Initialize(void)
{
    BOOT_Initialize();
    PIN_MANAGER_Initialize();
    INTERRUPT_Initialize();
    CLOCK_Initialize();
    UART1_Initialize();
    CAN1_Initialize();
    DMA_Initialize();
    TMR1_Initialize();
    INTERRUPT_GlobalEnable();
    SYSTEM_CORCONModeOperatingSet(CORCON_MODE_PORVALUES);
}

/**
 End of File
*/