pwd
PROJECT=$1
echo $PROJECT
../tools/esptool.py --port /dev/ttyUSB0  read_mac
../tools/esptool.py elf2image ../Debug/$PROJECT
./esptool2 -quiet -bin -boot2 ../Debug/$PROJECT $PROJECT.bin .text .data .rodata
../tools/esptool.py --p /dev/ttyUSB0 -b 921600 write_flash  -ff 40m -fm dio -fs 32m 0x00000 rboot.bin 0x02000 esp_gtw.bin
# ../tools/esptool.py --p /dev/ttyUSB0 -b 19200  write_flash  -ff 40m -fm dio -fs 32m 0x02010 ../Debug/$PROJECT-0x02010.bin
sleep 1
# ../tools/esptool.py run	
# minicom -D /dev/ttyUSB0
