1hexmate r0-5FFF,..\..\dist\default\production\app.X.production.hex r6004-FFFFFFFF,..\..\dist\default\production\app.X.production.hex -O..\..\dist\default\production\temp_original_copy.X.production.hex -FILL=w1:0x00,0x00,0x00,0x00@0x6000:0x6003
hexmate r6000-FFFFFFFF,..\..\dist\default\production\temp_original_copy.X.production.hex -O..\..\dist\default\production\temp_checksum.X.production.hex -FILL=w1:0xFF,0xFF,0xFF,0x00@0x6000:0x56FFF +-CK=6000-56FFF@6000w-2g2
hexmate r6000-6003,..\..\dist\default\production\temp_checksum.X.production.hex r6004-FFFFFFFF,..\..\dist\default\production\temp_original_copy.X.production.hex r0-5FFF,..\..\dist\default\production\temp_original_copy.X.production.hex -O..\..\dist\default\production\app.X.production.hex
rm ..\..\dist\default\production\temp_checksum.X.production.hex
rm ..\..\dist\default\production\temp_original_copy.X.production.hex