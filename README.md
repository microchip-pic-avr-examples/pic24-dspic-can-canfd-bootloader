![image](images/microchip.jpg) 

## PIC24/dsPIC CAN/CAN-FD Bootloader Demo

## Summary

An example bootloader using the 16-bit MCC bootloader and the CAN or CAN-FD peripheral.

## Related Documentation

https://www.microchip.com/16-bit-bootloader

## Setup And Operation

There are 4 configurations provided in this demo:
1) Using CAN classic on a device with the CAN-FD module
2) Using CAN-FD on a device with the CAN-FD module
3) Using CAN classic on a device with the enhanced CAN controller
4) Using CAN-FD on a device with the CAN-FD module and using TA100 for external ECDSA verify

The hardware and software setup is different for each of these use cases.  There are readme*.html files associated with each folder that describe the required hardware and software as well as how to setup and run each of the corresponding demos.

[dspic33ck-can](/dspic33ck-can "dsPIC33CK256MP508 CAN based bootloader example project")

[dspic33ck-canfd](/dspic33ck-canfd "dsPIC33CK256MP508 CAN-FD based bootloader example project")

[dspic33ep-can2.0b](/dspic33ep-can2.0b "dsPIC33EP512MU810 CAN based bootloader example project")

[dspic33ck-canfd-ta100](/dspic33ck-canfd-ta100 "dsPIC33CK256MP508 CAN-FD based bootloader example project w TA100 Verify")

