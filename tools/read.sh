../tools/esptool.py --p /dev/ttyUSB0 -b 921600 read_flash 0x78000 0x100 out.bin
od -c out.bin
od -x out.bin
