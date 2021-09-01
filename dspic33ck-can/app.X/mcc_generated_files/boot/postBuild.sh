if [ "$5" = "true" ]; then
exit 0
fi
hexmate r0-5FFF,"$2/$3/$4" r6004-FFFFFFFF,"$2/$3/$4" -O"$2/$3/temp_original_copy.X.production.hex" -FILL=w1:0x00,0x00,0x00,0x00@0x6000:0x6003
hexmate r6000-FFFFFFFF,"$2/$3/temp_original_copy.X.production.hex" -O"$2/$3/temp_checksum.X.production.hex" -FILL=w1:0xFF,0xFF,0xFF,0x00@0x6000:0x56FFF +-CK=6000-56FFF@6000w-2g2
hexmate r6000-6003,"$2/$3/temp_checksum.X.production.hex" r6004-FFFFFFFF,"$2/$3/temp_original_copy.X.production.hex" r0-5FFF,"$2/$3/temp_original_copy.X.production.hex" -O"$2/$3/$4"
rm "$2/$3/temp_checksum.X.production.hex"
rm "$2/$3/temp_original_copy.X.production.hex"