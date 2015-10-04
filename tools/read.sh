../tools/esptool.py --p /dev/ttyUSB0 -b 921600 read_flash 0x78000 0x100 out.bin
# od -Ax -c out.bin
# od -Ax -x out.bin
od -A x -t x1z -v out.bin
