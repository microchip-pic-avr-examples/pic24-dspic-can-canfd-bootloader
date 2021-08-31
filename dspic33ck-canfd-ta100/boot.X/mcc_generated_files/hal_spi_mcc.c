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
#include "talib/talib_fce.h"
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

/** \brief initialize an SPI interface using given config
 * \param[in] hal - opaque ptr to HAL data
 * \param[in] cfg - interface configuration
 * \return ATCA_SUCCESS on success, otherwise an error code.
 */
ATCA_STATUS hal_spi_init(ATCAIface iface, ATCAIfaceCfg *cfg)
{
    if(false == spiMaster->spiOpen())
    {
        return ATCA_COMM_FAIL;
    }
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
    ATCAIfaceCfg* cfg = atgetifacecfg(iface);

    if (!cfg)
    {
        return ATCA_TRACE(ATCA_BAD_PARAM, "");
    }

    spiMaster->writeBlock(txdata, txlength);
        
    return ATCA_SUCCESS;
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
    ATCAIfaceCfg* cfg = atgetifacecfg(iface);

    if ((NULL == cfg) || (NULL == rxlength) || (NULL == rxdata))
    {
        return ATCA_TRACE(ATCA_INVALID_POINTER, "NULL pointer encountered");
    }

    /* read status register/length bytes to know number of bytes to read */
    spiMaster->readBlock(rxdata, *rxlength);

    return ATCA_SUCCESS;
}


/** \brief Perform control operations for the kit protocol
 * \param[in]     iface          Interface to interact with.
 * \param[in]     option         Control parameter identifier
 * \param[in]     param          Optional pointer to parameter value
 * \param[in]     paramlen       Length of the parameter
 * \return ATCA_SUCCESS on success, otherwise an error code.
 */
ATCA_STATUS hal_spi_control(ATCAIface iface, uint8_t option, void* param, size_t paramlen)
{
    (void)param;
    (void)paramlen;

    if (iface && iface->mIfaceCFG)
    {
        switch (option)
        {
        case ATCA_HAL_CONTROL_SELECT:
            TA100_ChipSelect(true);
            return ATCA_SUCCESS;
        case ATCA_HAL_CONTROL_DESELECT:
            TA100_ChipSelect(false);
            return ATCA_SUCCESS;
        default:
            return ATCA_UNIMPLEMENTED;
        }
    }
    return ATCA_BAD_PARAM;
}

/** \brief manages reference count on given bus and releases resource if no more refences exist
 * \param[in] hal_data - opaque pointer to hal data structure - known only to the HAL implementation
 * \return ATCA_SUCCESS on success, otherwise an error code.
 */
ATCA_STATUS hal_spi_release(void *hal_data)
{
    spiMaster->spiClose();
    return ATCA_SUCCESS;
}

/** @} */
