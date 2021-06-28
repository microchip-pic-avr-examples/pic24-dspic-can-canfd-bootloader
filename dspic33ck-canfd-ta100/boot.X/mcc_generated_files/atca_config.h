/* Auto-generated config file atca_config.h */
#ifndef ATCA_CONFIG_H
#define ATCA_CONFIG_H

/* Included HALS */
//#cmakedefine ATCA_HAL_KIT_HID
//#cmakedefine ATCA_HAL_KIT_CDC
//#define ATCA_HAL_I2C
#define ATCA_HAL_SPI
//#cmakedefine ATCA_HAL_CUSTOM

/* Included device support */
//#cmakedefine ATCA_ATSHA204A_SUPPORT
//#cmakedefine ATCA_ATSHA206A_SUPPORT
//#cmakedefine ATCA_ATECC108A_SUPPORT
//#cmakedefine ATCA_ATECC508A_SUPPORT
//#define ATCA_ATECC608_SUPPORT
#define ATCA_TA100_SUPPORT

/** Device Override - Library Assumes ATECC608B support in checks */
//#cmakedefine ATCA_ATECC608A_SUPPORT

#ifdef ATCA_TA100_SUPPORT
//#define ATCA_TA100_AES_AUTH_SUPPORT
#endif

/** Define if cryptoauthlib is to use the maximum execution time method */
#define ATCA_NO_POLL


/* \brief How long to wait after an initial wake failure for the POST to
 *         complete.
 * If Power-on self test (POST) is enabled, the self test will run on waking
 * from sleep or during power-on, which delays the wake reply.
 */
#ifndef ATCA_POST_DELAY_MSEC
#define ATCA_POST_DELAY_MSEC 25
#endif

/***************** Diagnostic & Test Configuration Section *****************/

/** Enable debug messages */
//#define ATCA_PRINTF

/** Enable to build in test hooks */
//#cmakedefine ATCA_TESTS_ENABLED

/******************** Features Configuration Section ***********************/

/** Define certificate templates to be supported. */
//#cmakedefine ATCA_TNGTLS_SUPPORT
//#cmakedefine ATCA_TNGLORA_SUPPORT
//#cmakedefine ATCA_TFLEX_SUPPORT
//#cmakedefine ATCA_TNG_LEGACY_SUPPORT

/** Define Software Crypto Library to Use - if none are defined use the
    cryptoauthlib version where applicable */
//#cmakedefine ATCA_MBEDTLS
//#define ATCA_OPENSSL
//#cmakedefine ATCA_WOLFSSL

/** Additional Runtime Configuration */
//#cmakedefine ATCA_LIBRARY_CONF  "@ATCA_LIBRARY_CONF@"

/** Define to build atcab_ functions rather that defining them as macros */
//#cmakedefine ATCA_USE_ATCAB_FUNCTIONS

/******************** Platform Configuration Section ***********************/

/** Define if the library is not to use malloc/free */
#define ATCA_NO_HEAP 1

/** Define platform malloc/free */
//#define ATCA_PLATFORM_MALLOC    @ATCA_PLATFORM_MALLOC@
//#define ATCA_PLATFORM_FREE      @ATCA_PLATFORM_FREE@

#include "../delay.h"
#define atca_delay_ms   DELAY_milliseconds
#define atca_delay_us   DELAY_microseconds

#endif // ATCA_CONFIG_H
