################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include tools/subdir.mk
-include cpp/subdir.mk
-include core_dump/subdir.mk
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
all: esp_cpp

dependents:
	-cd /home/lieven/workspace/Common/Debug && $(MAKE) all

# Tool invocations
esp_cpp: $(OBJS) $(USER_OBJS) /home/lieven/workspace/Common/Debug/libCommon.a
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	xtensa-lx106-elf-g++ -nostartfiles -nodefaultlibs -nostdlib -L"/home/lieven/workspace/Common/Debug" -L/home/lieven/esp_iot_sdk_v1.4.0/lib -Lgcc -u call_user_start -Wl,-static -T../ld/link.ld  -Wl,--gc-sections -mlongcalls -Xlinker -L/home/lieven/esp-open-sdk/sdk/lib -Xlinker -lssl -lmain -lhal -lphy -lpp -lnet80211 -llwip -lwpa -lssl -lmain -lcirom -Xlinker --gc-sections -Xlinker --no-undefined -o "esp_cpp" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) esp_cpp
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/lieven/workspace/Common/Debug/libCommon.a:

-include ../makefile.targets
