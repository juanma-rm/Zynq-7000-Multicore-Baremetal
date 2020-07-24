################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/main.c \
../src/matrix_operation.c \
../src/platform.c \
../src/zynq7000_baremetal.c 

OBJS += \
./src/main.o \
./src/matrix_operation.o \
./src/platform.o \
./src/zynq7000_baremetal.o 

C_DEPS += \
./src/main.d \
./src/matrix_operation.d \
./src/platform.d \
./src/zynq7000_baremetal.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../PlatformHW_1_GPIO_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


