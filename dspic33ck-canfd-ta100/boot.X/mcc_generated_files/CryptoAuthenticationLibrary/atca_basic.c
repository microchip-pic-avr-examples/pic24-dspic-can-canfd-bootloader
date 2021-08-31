// This file intentionally cleared. To obtain the original source contact Microchip.
#include "cryptoauthlib.h"
#include <stddef.h>

ATCA_STATUS atcab_init( void* i)
{
    return !ATCA_SUCCESS;
}

void atcab_lock_data_slot(int i)
{
    return;
}

ATCA_STATUS atcab_verify_extern(uint8_t *a, uint8_t *b, const uint8_t *c, bool *d)
{
    return !ATCA_SUCCESS;
}

ATCAIfaceCfg* atgetifacecfg(ATCAIface i)
{
    return NULL;
}