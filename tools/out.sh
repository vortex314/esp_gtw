pwd
USB=/dev/ttyUSB0
PROJECT=$1
echo $PROJECT
../tools/esptool.py --port $USB  read_mac
../tools/esptool.py elf2image ../Debug/$PROJECT
./esptool2 -debug -bin -boot2 -4096 -dio -40 ../Debug/$PROJECT $PROJECT.bin .text .data .rodata
../tools/esptool.py --p $USB -b 921600 write_flash  -ff 40m -fm dio -fs 32m 0x00000 rboot.bin 0x02000 esp_gtw.bin 
./reset $USB
mv minicom.log minicom.1.log
minicom  -D $USB -C minicom.log
