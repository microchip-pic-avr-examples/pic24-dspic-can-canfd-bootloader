if %5=="true" (
exit 0
)
REM Blank signature location
hexmate r0-FFFF,"%~2\%~3\%~4" r10080-FFFFFFFF,"%~2\%~3\%~4" -O"%~2\%~3\temp_original_copy.X.production.hex" -FILL=w1:0x00,0x00,0x00,0x00@0x10000:0x1007F

REM Fill in unimplemented flash locations
hexmate r0-FFFFFFFF,"%~2\%~3\temp_original_copy.X.production.hex" -O"%~2\%~3\temp_original_copy.X.production.hex" -FILL=w1:0xFF,0xFF,0xFF,0x00@0x10000:0x56FFF

REM Generate application binary image
hexmate r10000-56FFFs-10000,"%~2\%~3\temp_original_copy.X.production.hex" -O"%~2\%~3\temp_original_copy.X.production.hex"
%1\xc16-objcopy -I ihex -O binary "%~2\%~3\temp_original_copy.X.production.hex" "%~2\%~3\app.X.production.bin"

REM Sign binary file
java -jar signing_tool.jar -sign "%~2\%~3\app.X.production.bin" ..\..\..\boot.X\private_key.pem "%~2\%~3\app.X.production.bin.signature.der"

REM Export signature value
java -jar signing_tool.jar -export "%~2\%~3\app.X.production.bin.signature.der" "%~2\%~3\app.X.production.bin.signature.bin"

REM Covert signature to .hex format
%1\xc16-objcopy -I binary -O ihex "%~2\%~3\app.X.production.bin.signature.bin" "%~2\%~3\app.X.production.bin.signature.hex"

REM Copy signature .hex into application .hex file application header
hexmate r0-1s10000,"%~2\%~3\app.X.production.bin.signature.hex" r10002-FFFFFFFF,"%~2\%~3\%~4" r0-FFFF,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r2-3s10002,"%~2\%~3\app.X.production.bin.signature.hex" r10006-FFFFFFFF,"%~2\%~3\%~4" r0-10003,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r4-5s10004,"%~2\%~3\app.X.production.bin.signature.hex" r1000A-FFFFFFFF,"%~2\%~3\%~4" r0-10007,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r6-7s10006,"%~2\%~3\app.X.production.bin.signature.hex" r1000E-FFFFFFFF,"%~2\%~3\%~4" r0-1000B,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r8-9s10008,"%~2\%~3\app.X.production.bin.signature.hex" r10012-FFFFFFFF,"%~2\%~3\%~4" r0-1000F,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate rA-Bs1000A,"%~2\%~3\app.X.production.bin.signature.hex" r10016-FFFFFFFF,"%~2\%~3\%~4" r0-10013,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate rC-Ds1000C,"%~2\%~3\app.X.production.bin.signature.hex" r1001A-FFFFFFFF,"%~2\%~3\%~4" r0-10017,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate rE-Fs1000E,"%~2\%~3\app.X.production.bin.signature.hex" r1001E-FFFFFFFF,"%~2\%~3\%~4" r0-1001B,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r10-11s10010,"%~2\%~3\app.X.production.bin.signature.hex" r10022-FFFFFFFF,"%~2\%~3\%~4" r0-1001F,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r12-13s10012,"%~2\%~3\app.X.production.bin.signature.hex" r10026-FFFFFFFF,"%~2\%~3\%~4" r0-10023,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r14-15s10014,"%~2\%~3\app.X.production.bin.signature.hex" r1002A-FFFFFFFF,"%~2\%~3\%~4" r0-10027,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r16-17s10016,"%~2\%~3\app.X.production.bin.signature.hex" r1002E-FFFFFFFF,"%~2\%~3\%~4" r0-1002B,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r18-19s10018,"%~2\%~3\app.X.production.bin.signature.hex" r10032-FFFFFFFF,"%~2\%~3\%~4" r0-1002F,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r1A-1Bs1001A,"%~2\%~3\app.X.production.bin.signature.hex" r10036-FFFFFFFF,"%~2\%~3\%~4" r0-10033,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r1C-1Ds1001C,"%~2\%~3\app.X.production.bin.signature.hex" r1003A-FFFFFFFF,"%~2\%~3\%~4" r0-10037,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r1E-1Fs1001E,"%~2\%~3\app.X.production.bin.signature.hex" r1003E-FFFFFFFF,"%~2\%~3\%~4" r0-1003B,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r20-21s10020,"%~2\%~3\app.X.production.bin.signature.hex" r10042-FFFFFFFF,"%~2\%~3\%~4" r0-1003F,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r22-23s10022,"%~2\%~3\app.X.production.bin.signature.hex" r10046-FFFFFFFF,"%~2\%~3\%~4" r0-10043,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r24-25s10024,"%~2\%~3\app.X.production.bin.signature.hex" r1004A-FFFFFFFF,"%~2\%~3\%~4" r0-10047,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r26-27s10026,"%~2\%~3\app.X.production.bin.signature.hex" r1004E-FFFFFFFF,"%~2\%~3\%~4" r0-1004B,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r28-29s10028,"%~2\%~3\app.X.production.bin.signature.hex" r10052-FFFFFFFF,"%~2\%~3\%~4" r0-1004F,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r2A-2Bs1002A,"%~2\%~3\app.X.production.bin.signature.hex" r10056-FFFFFFFF,"%~2\%~3\%~4" r0-10053,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r2C-2Ds1002C,"%~2\%~3\app.X.production.bin.signature.hex" r1005A-FFFFFFFF,"%~2\%~3\%~4" r0-10057,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r2E-2Fs1002E,"%~2\%~3\app.X.production.bin.signature.hex" r1005E-FFFFFFFF,"%~2\%~3\%~4" r0-1005B,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r30-31s10030,"%~2\%~3\app.X.production.bin.signature.hex" r10062-FFFFFFFF,"%~2\%~3\%~4" r0-1005F,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r32-33s10032,"%~2\%~3\app.X.production.bin.signature.hex" r10066-FFFFFFFF,"%~2\%~3\%~4" r0-10063,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r34-35s10034,"%~2\%~3\app.X.production.bin.signature.hex" r1006A-FFFFFFFF,"%~2\%~3\%~4" r0-10067,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r36-37s10036,"%~2\%~3\app.X.production.bin.signature.hex" r1006E-FFFFFFFF,"%~2\%~3\%~4" r0-1006B,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r38-39s10038,"%~2\%~3\app.X.production.bin.signature.hex" r10072-FFFFFFFF,"%~2\%~3\%~4" r0-1006F,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r3A-3Bs1003A,"%~2\%~3\app.X.production.bin.signature.hex" r10076-FFFFFFFF,"%~2\%~3\%~4" r0-10073,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r3C-3Ds1003C,"%~2\%~3\app.X.production.bin.signature.hex" r1007A-FFFFFFFF,"%~2\%~3\%~4" r0-10077,"%~2\%~3\%~4" -O"%~2\%~3\%~4"
hexmate r3E-3Fs1003E,"%~2\%~3\app.X.production.bin.signature.hex" r1007E-FFFFFFFF,"%~2\%~3\%~4" r0-1007B,"%~2\%~3\%~4" -O"%~2\%~3\%~4"

del "%~2\%~3\temp_original_copy.X.production.hex"