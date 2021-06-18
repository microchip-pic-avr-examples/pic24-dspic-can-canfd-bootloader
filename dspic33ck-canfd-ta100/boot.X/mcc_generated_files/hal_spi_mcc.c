/**
 * \file
 * \brief ATCA Hardware abstraction layer for SPI over Harmony PLIB.
 *
 * This code is structured in two parts.  Part 1 is the connection of the ATCA HAL API to the physical SPI
 * implementation. Part 2 is the Harmony SPI primitives to set up the interface.
 *
 * Prerequisite: add SERCOM SPI Master Interrupt support to application in Mplab Harmony 3
 *
 * \copyright (c) 2015-2020 Microchip Technology Inc. and its subsidiaries.
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip software
 * and any derivatives exclusively with Microchip products. It is your
 * responsibility to comply with third party license terms applicable to your
 * use of third party software (including open source software) that may
 * accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT,
 * SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE
 * OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF
 * MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE
 * FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL
 * LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED
 * THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR
 * THIS SOFTWARE.
 */
#include <string.h>
#include <stdio.h>

#include "atca_config.h"
#include "cryptoauthlib.h"
#include "hal/atca_hal.h"
#include "atca_device.h"
#include "talib/talib_defines.h"
#include "drivers/spi_master.h"

#include <xc.h>
#include <stdlib.h>

static void TA100_ChipSelect(bool selected)
{
    TRISDbits.TRISD3 = 0;
    
    if(selected == true)
    {
        LATDbits.LATD3 = 0;
    }
    else
    {
        LATDbits.LATD3 = 1;
    }
}

/** \defgroup hal_ Hardware abstraction layer (hal_)
 *
 * \brief
 * These methods define the hardware abstraction layer for communicating with a TA100 device
 *
   @{ */



/* Notes:
    - this HAL implementation assumes you've included the Harmony SPI libraries in your project, otherwise,
    the HAL layer will not compile because the Harmony SPI drivers are a dependency *
 */

/** \brief discover spi buses available for this hardware
 * this maintains a list of logical to physical bus mappings freeing the application
 * of the a-priori knowledge
 * \param[in] spi_buses - an array of logical bus numbers
 * \param[in] max_buses - maximum number of buses the app wants to attempt to discover
 * \return ATCA_SUCCESS
 */

ATCA_STATUS hal_spi_discover_buses(int spi_buses[], int max_buses)
{
    spi_buses[0] = 0;
    return ATCA_SUCCESS;
}

/** \brief discover any TA100 devices on a given logical bus number
 * \param[in]  bus_num  logical bus number on which to look for TA100 devices
 * \param[out] cfg     pointer to head of an array of interface config structures which get filled in by this method
 * \param[out] found   number of devices found on this bus
 * \return ATCA_SUCCESS
 */

ATCA_STATUS hal_spi_discover_devices(int bus_num, ATCAIfaceCfg cfg[], int *found)
{
    return ATCA_UNIMPLEMENTED;
}

/** \brief initialize an SPI interface using given config
 * \param[in] hal - opaque ptr to HAL data
 * \param[in] cfg - interface configuration
 * \return ATCA_SUCCESS on success, otherwise an error code.
 */
ATCA_STATUS hal_spi_init(void *hal, ATCAIfaceCfg *cfg)
{
    return ATCA_SUCCESS;
}

/** \brief HAL implementation of SPI post init
 * \param[in] iface  instance
 * \return ATCA_SUCCESS
 */
ATCA_STATUS hal_spi_post_init(ATCAIface iface)
{
    return ATCA_SUCCESS;
}

/** \brief HAL implementation of SPI send over Harmony
 * \param[in] iface         instance
 * \param[in] word_address  device transaction type
 * \param[in] txdata        pointer to space to bytes to send
 * \param[in] txlength      number of bytes to send
 * \return ATCA_SUCCESS on success, otherwise an error code.
 */

ATCA_STATUS hal_spi_send(ATCAIface iface, uint8_t word_address, uint8_t *txdata, int txlength)
{
    ATCA_STATUS status = ATCA_SUCCESS;

    if(false == spiMaster->spiOpen())
    {
        return ATCA_COMM_FAIL;
    }
    TA100_ChipSelect(true);

    do
    {
        if (0xFF != word_address)
        {
            txdata[0] = word_address; // insert the Word Address Value, Command token
            txlength++;               // account for word address value byte.
        }


        spiMaster->writeBlock(txdata, txlength);
    }
    while (0);

    TA100_ChipSelect(false);
    spiMaster->spiClose();
    
    return status;
}

/** \brief HAL implementation of SPI receive function for HARMONY SPI
 * \param[in]    iface          Device to interact with.
 * \param[in]    word_address   device transaction type
 * \param[out]   rxdata         Data received will be returned here.
 * \param[in,out] rxlength      As input, the size of the rxdata buffer.
 *                              As output, the number of bytes received.
 * \return ATCA_SUCCESS on success, otherwise an error code.
 */
ATCA_STATUS hal_spi_receive(ATCAIface iface, uint8_t word_address, uint8_t *rxdata, uint16_t *rxlength)
{
    ATCA_STATUS status = ATCA_SUCCESS;

    uint16_t rxdata_max_size;
    uint16_t read_length = 2;

    rxdata_max_size = *rxlength;
    *rxlength = 0;

    if(false == spiMaster->spiOpen())
    {
        return ATCA_COMM_FAIL;
    }
    TA100_ChipSelect(true);

    do
    {
        /*Set read length.. Check for register reads or 1 byte reads*/
        if ((ATCA_MAIN_PROCESSOR_RD_CSR == word_address) || (ATCA_FAST_CRYPTO_RD_FSR == word_address )
            || ( 1  == rxdata_max_size))
        {
            read_length = 1;
        }

        /*Send Word address to device...*/
        spiMaster->writeBlock(&word_address, sizeof(word_address));

        /* read status register/length bytes to know number of bytes to read */
        spiMaster->readBlock(rxdata, read_length);
        
        if (1 == read_length)
        {
            ATCA_TRACE(status, "1 byte read completed");
            break;
        }
        
        /*Calculate bytes to read based on device response*/
        read_length = ((uint16_t)rxdata[0] * 256) + rxdata[1];

        if (read_length > rxdata_max_size)
        {
            status = ATCA_TRACE(ATCA_SMALL_BUFFER, "rxdata is small buffer");
            break;
        }

        if (read_length < 5)
        {
            status = ATCA_TRACE(ATCA_RX_FAIL, "packet size is invalid");
            break;
        }

        /* Read given length bytes from device */
        spiMaster->readBlock(&rxdata[2], read_length - 2);
    }
    while (0);

    TA100_ChipSelect(false);
    spiMaster->spiClose();

    *rxlength = read_length;
    return status;
}



/** \brief wake up TA100 device using SPI bus
 * \param[in] iface  interface to logical device to wakeup
 * \return ATCA_SUCCESS on success, otherwise an error code.
 */

ATCA_STATUS hal_spi_wake(ATCAIface iface)
{
    return ATCA_UNIMPLEMENTED;
}

/** \brief idle TA100 device using SPI bus
 * \param[in] iface  interface to logical device to idle
 * \return ATCA_SUCCESS on success, otherwise an error code.
 */

ATCA_STATUS hal_spi_idle(ATCAIface iface)
{
    return ATCA_UNIMPLEMENTED;
}

/** \brief sleep TA100 device using SPI bus
 * \param[in] iface  interface to logical device to sleep
 * \return ATCA_SUCCESS on success, otherwise an error code.
 */

ATCA_STATUS hal_spi_sleep(ATCAIface iface)
{
    return ATCA_UNIMPLEMENTED;
}

/** \brief manages reference count on given bus and releases resource if no more refences exist
 * \param[in] hal_data - opaque pointer to hal data structure - known only to the HAL implementation
 * \return ATCA_SUCCESS on success, otherwise an error code.
 */

ATCA_STATUS hal_spi_release(void *hal_data)
{
    return ATCA_SUCCESS;
}

/** @} */
