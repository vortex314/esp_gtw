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
../utils.c 

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
./utils.o 

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
./utils.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	xtensa-lx106-elf-gcc -DICACHE_FLASH -D__ESP8266__ -DSTA_SSID='"Merckx"' -DSTA_PASS='"LievenMarletteEwoutRonald"' -DESP_COREDUMP -I"/home/lieven/workspace/Common" -I"/home/lieven/workspace/Common/inc" -I/home/lieven/esp_iot_sdk_v1.4.0/include -I/home/lieven/esp-open-sdk/lx106-hal/include/ -I"/home/lieven/workspace/esp_cpp/include" -I"/home/lieven/workspace/esp_cpp/core_dump" -I"/home/lieven/workspace/esp_cpp/cpp" -O0 -Og -g -w -Wall -c -fmessage-length=0 -Os -Wpointer-arith -Wundef -Werror -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -fno-jump-tables -D__ets__ -DICACHE_FLASH -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


