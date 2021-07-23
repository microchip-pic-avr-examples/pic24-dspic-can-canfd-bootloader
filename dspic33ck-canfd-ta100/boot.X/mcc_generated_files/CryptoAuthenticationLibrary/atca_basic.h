// This file intentionally cleared. To obtain the original source contact Microchip.
#include <stdint.h>
#include <stdbool.h>

#warning "TA100 FUNCTIONALITY DISABLED!!!! Contact Microchip to get Crypto Authentication Library support"

typedef struct
{ 
}ATCAIface;

typedef struct
{
    uint16_t iface_type;
    uint16_t devtype;
    uint16_t wake_delay;
    uint16_t rx_retries;
} ATCAIfaceCfg;

typedef struct atca_command
{
}atca_command;

typedef struct atca_iface 
{
}atca_iface;

typedef struct atca_device
{    
    atca_command *a;
    atca_iface *b;
}atca_device;

typedef enum
{
    ATCA_SUCCESS                = 0x00, 
    ATCA_COMM_FAIL              = 0x00, 
    ATCA_UNIMPLEMENTED          = 0x00,
} ATCA_STATUS;

#define ATCA_TRACE(s, m) s
#define ATCA_SPI_IFACE 0
#define TA100 0
#define ATCA_MAIN_PROCESSOR_RD_CSR 0
#define ATCA_FAST_CRYPTO_RD_FSR 0
#define ATCA_RX_FAIL 0
#define ATCA_SMALL_BUFFER 0

ATCA_STATUS atcab_init( void* i);
void atcab_lock_data_slot(int i);
ATCA_STATUS atcab_verify_extern(uint8_t *a, uint8_t *b, const uint8_t *c, bool *d);
