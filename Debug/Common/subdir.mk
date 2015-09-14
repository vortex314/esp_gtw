################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Common/Main.cpp \
../Common/Pump.cpp \
../Common/Sender.cpp \
../Common/Sys.cpp \
../Common/stubs.cpp 

OBJS += \
./Common/Main.o \
./Common/Pump.o \
./Common/Sender.o \
./Common/Sys.o \
./Common/stubs.o 

CPP_DEPS += \
./Common/Main.d \
./Common/Pump.d \
./Common/Sender.d \
./Common/Sys.d \
./Common/stubs.d 


# Each subdirectory must supply rules for building sources it contributes
Common/%.o: ../Common/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	xtensa-lx106-elf-g++ -DICACHE_FLASH -D__ESP8266__ -I/home/lieven/esp-open-sdk/esp_iot_sdk_v1.3.0/include -I"/home/lieven/workspace/esp_gtw/include" -I"/home/lieven/workspace/Common/inc" -O0 -g3 -Wall -c -fmessage-length=0 -ffunction-sections -fno-rtti -fno-exceptions -mlongcalls -mtext-section-literals -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


