################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../config.c \
../gpio16.c \
../hw_timer.c \
../mqtt.c \
../mqtt_msg.c \
../mutex.c \
../proto.c \
../queue.c \
../rboot_loc.c \
../ringbuf.c \
../uart.c \
../user_main.c \
../util.c \
../utils.c \
../wifi.c 

OBJS += \
./config.o \
./gpio16.o \
./hw_timer.o \
./mqtt.o \
./mqtt_msg.o \
./mutex.o \
./proto.o \
./queue.o \
./rboot_loc.o \
./ringbuf.o \
./uart.o \
./user_main.o \
./util.o \
./utils.o \
./wifi.o 

C_DEPS += \
./config.d \
./gpio16.d \
./hw_timer.d \
./mqtt.d \
./mqtt_msg.d \
./mutex.d \
./proto.d \
./queue.d \
./rboot_loc.d \
./ringbuf.d \
./uart.d \
./user_main.d \
./util.d \
./utils.d \
./wifi.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	xtensa-lx106-elf-gcc -DICACHE_FLASH -D__ESP8266__ -DSTA_SSID='"Merckx"' -DSTA_PASS='"LievenMarletteEwoutRonald"' -I"/home/lieven/workspace/Common" -I/home/lieven/esp-open-sdk/esp_iot_sdk_v1.3.0/include -I"/home/lieven/workspace/esp_gtw/include" -I"/home/lieven/workspace/Common/inc" -Os -w -Wall -c -fmessage-length=0 -Os -Wpointer-arith -Wundef -Werror -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals  -D__ets__ -DICACHE_FLASH -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


