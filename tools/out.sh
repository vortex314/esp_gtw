pwd
mv minicom1.log minicom2.log
mv minicom.log minicom1.log
USB=/dev/ttyUSB0
PROJECT=$1
echo "---------------" $PROJECT "------------------------"
./reset $USB
../tools/esptool.py --port $USB  read_mac
# ../tools/esptool.py --port $USB  erase_flash
../tools/esptool.py elf2image ../Debug/$PROJECT
./esptool2 -debug -bin -boot2 -1024 -dio -40 ../Debug/$PROJECT $PROJECT.bin .text .data .rodata # was 4096
../tools/esptool.py --p $USB -b 576000 write_flash  -ff 40m -fm dio -fs 8m 0x00000 rboot.bin 0x02000 $PROJECT.bin \
	0xFC000 esp_init_data_default.bin 0xFE000 blank.bin 
 # was 32m, # esp_init_data_default.bin, was 7C and 7E 8m 
./reset $USB
minicom  -D $USB -C minicom.log
