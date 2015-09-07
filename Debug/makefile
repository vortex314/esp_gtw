################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include tools/subdir.mk
-include Common/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: esp_gtw

# Tool invocations
esp_gtw: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	xtensa-lx106-elf-g++ -nostartfiles -nodefaultlibs -nostdlib -L/home/lieven/esp-open-sdk/sdk/lib -u call_user_start -Wl,-static -T/home/lieven/esp-open-sdk/esp_iot_sdk_v1.3.0/ld/eagle.app.v6.ld  -Wl,--gc-sections -mlongcalls -Xlinker -L/home/lieven/esp-open-sdk/sdk/lib -Xlinker -lssl -lmain -lhal -lphy -lpp -lnet80211 -llwip -lwpa -lssl -lmain -lcirom -Xlinker --gc-sections -Xlinker --no-undefined -o "esp_gtw" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) esp_gtw
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
