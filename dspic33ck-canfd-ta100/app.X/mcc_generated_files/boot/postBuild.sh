# Blank signature location
hexmate r0-FFFF,..\..\dist\default\production\app.X.production.hex r10080-FFFFFFFF,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\temp_original_copy.X.production.hex -FILL=w1:0x00,0x00,0x00,0x00@0x10000:0x1007F

# Fill in unimplemented flash locations
hexmate r0-FFFFFFFF,..\..\dist\default\production\temp_original_copy.X.production.hex -O..\..\dist\default\production\temp_original_copy.X.production.hex -FILL=w1:0xFF,0xFF,0xFF,0x00@0x10000:0x56FFF

# Generate application binary image
hexmate r10000-56FFFs-10000,..\..\dist\default\production\temp_original_copy.X.production.hex -O..\..\dist\default\production\temp_original_copy.X.production.hex
$1/xc16-objcopy -I ihex -O binary ..\..\dist\default\production\temp_original_copy.X.production.hex ..\..\dist\default\production\app.X.production.bin

# Sign binary file
java -jar signing_tool.jar -sign ..\..\dist\default\production\app.X.production.bin ..\..\..\boot.X\private_key.pem ..\..\dist\default\production\app.X.production.bin.signature.der

# Export signature value
java -jar signing_tool.jar -export ..\..\dist\default\production\app.X.production.bin.signature.der ..\..\dist\default\production\app.X.production.bin.signature.bin

# Covert signature to .hex format
$1/xc16-objcopy -I binary -O ihex ..\..\dist\default\production\app.X.production.bin.signature.bin ..\..\dist\default\production\app.X.production.bin.signature.hex

# Copy signature .hex into application .hex file application header
hexmate r0-1s10000,..\..\dist\default\production\app.X.production.bin.signature.hex r10002-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-FFFF,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r2-3s10002,..\..\dist\default\production\app.X.production.bin.signature.hex r10006-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-10003,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r4-5s10004,..\..\dist\default\production\app.X.production.bin.signature.hex r1000A-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-10007,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r6-7s10006,..\..\dist\default\production\app.X.production.bin.signature.hex r1000E-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-1000B,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r8-9s10008,..\..\dist\default\production\app.X.production.bin.signature.hex r10012-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-1000F,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate rA-Bs1000A,..\..\dist\default\production\app.X.production.bin.signature.hex r10016-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-10013,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate rC-Ds1000C,..\..\dist\default\production\app.X.production.bin.signature.hex r1001A-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-10017,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate rE-Fs1000E,..\..\dist\default\production\app.X.production.bin.signature.hex r1001E-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-1001B,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r10-11s10010,..\..\dist\default\production\app.X.production.bin.signature.hex r10022-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-1001F,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r12-13s10012,..\..\dist\default\production\app.X.production.bin.signature.hex r10026-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-10023,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r14-15s10014,..\..\dist\default\production\app.X.production.bin.signature.hex r1002A-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-10027,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r16-17s10016,..\..\dist\default\production\app.X.production.bin.signature.hex r1002E-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-1002B,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r18-19s10018,..\..\dist\default\production\app.X.production.bin.signature.hex r10032-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-1002F,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r1A-1Bs1001A,..\..\dist\default\production\app.X.production.bin.signature.hex r10036-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-10033,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r1C-1Ds1001C,..\..\dist\default\production\app.X.production.bin.signature.hex r1003A-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-10037,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r1E-1Fs1001E,..\..\dist\default\production\app.X.production.bin.signature.hex r1003E-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-1003B,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r20-21s10020,..\..\dist\default\production\app.X.production.bin.signature.hex r10042-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-1003F,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r22-23s10022,..\..\dist\default\production\app.X.production.bin.signature.hex r10046-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-10043,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r24-25s10024,..\..\dist\default\production\app.X.production.bin.signature.hex r1004A-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-10047,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r26-27s10026,..\..\dist\default\production\app.X.production.bin.signature.hex r1004E-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-1004B,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r28-29s10028,..\..\dist\default\production\app.X.production.bin.signature.hex r10052-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-1004F,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r2A-2Bs1002A,..\..\dist\default\production\app.X.production.bin.signature.hex r10056-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-10053,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r2C-2Ds1002C,..\..\dist\default\production\app.X.production.bin.signature.hex r1005A-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-10057,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r2E-2Fs1002E,..\..\dist\default\production\app.X.production.bin.signature.hex r1005E-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-1005B,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r30-31s10030,..\..\dist\default\production\app.X.production.bin.signature.hex r10062-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-1005F,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r32-33s10032,..\..\dist\default\production\app.X.production.bin.signature.hex r10066-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-10063,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r34-35s10034,..\..\dist\default\production\app.X.production.bin.signature.hex r1006A-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-10067,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r36-37s10036,..\..\dist\default\production\app.X.production.bin.signature.hex r1006E-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-1006B,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r38-39s10038,..\..\dist\default\production\app.X.production.bin.signature.hex r10072-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-1006F,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r3A-3Bs1003A,..\..\dist\default\production\app.X.production.bin.signature.hex r10076-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-10073,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r3C-3Ds1003C,..\..\dist\default\production\app.X.production.bin.signature.hex r1007A-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-10077,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex
hexmate r3E-3Fs1003E,..\..\dist\default\production\app.X.production.bin.signature.hex r1007E-FFFFFFFF,..\..\dist\default\production\app.X.production.hex r0-1007B,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\app.X.production.hex

rm ..\..\dist\default\production\temp_original_copy.X.production.hex