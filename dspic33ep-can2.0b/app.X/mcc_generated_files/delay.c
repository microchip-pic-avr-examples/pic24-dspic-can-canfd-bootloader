/**
\file
\addtogroup doc_driver_delay_code
\brief This file contains the functions to generate delays in the millisecond and microsecond ranges.
\copyright (c) 2020 Microchip Technology Inc. and its subsidiaries.
\page License
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
**/


#ifndef FCY
#define FCY (_XTAL_FREQ/2)
#endif
#include "clock.h"
#include <libpic30.h>
#include <stdint.h>

/**
*  \ingroup doc_driver_delay_code
*  Call this function to delay execution of the program for a certain number of milliseconds
@param milliseconds - number of milliseconds to delay
*/
void DELAY_milliseconds(uint16_t milliseconds) {
    while(milliseconds--){ 
        __delay_ms(1); 
    }
}

/**
*  \ingroup doc_driver_delay_code
*  Call this function to delay execution of the program for a certain number of microseconds
@param microseconds - number of microseconds to delay
*/
void DELAY_microseconds(uint16_t microseconds) {
    while( microseconds >= 32)
    {
        __delay_us(32);
        microseconds -= 32;
    }
    
    while(microseconds--)
    {
        __delay_us(1);
    }
}
