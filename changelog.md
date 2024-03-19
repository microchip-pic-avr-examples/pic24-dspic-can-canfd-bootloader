# PIC24/dsPIC CAN / CAN-FD Bootloader Firmware v1.3.0
### Release Highlights

- Demo has been deprecated.  Use demos at https://github.com/microchip-pic-avr-examples/dspic33_dsc_bootloader_code_examples

# PIC24/dsPIC CAN / CAN-FD Bootloader Firmware v1.2.0
### Release Highlights

- Update TA100 Trust Anchor external HSM to Crypto Authentication Library to 3.3.2

### Features Added\Updated

- Support for TA100 ECDSA verify on Crypto Authentication Library 3.3.2 required rewritten HAL

# PIC24/dsPIC CAN / CAN-FD Bootloader Firmware v1.1.0
### Release Highlights

- Fixes project loading issues.
- ADD Demo using TA100 Trust Anchor external HSM for ECDSA verify of CAN bootloader

### Features Added\Updated

- Support for TA100 ECDSA verify
- Bug fixes to use externally released MCC modules rather than internal builds.  The previous version would give import warnings to user due to it being unable to find the specified version.

# PIC24/dsPIC CAN / CAN-FD Bootloader Firmware v1.0.0
### Release Highlights

- CAN and CAN-FD bootloader firmware
- Requires Unified Bootloader Host Application 1.17.0

### Features Added\Updated

- Initial Release
